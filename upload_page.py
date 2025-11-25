#!/usr/bin/env python3
import os
import re
import sys
import requests
import subprocess
import os
from urllib.parse import quote
from os.path import basename

# ------------------------------------------------------
# Load environment variables from tokens.sh
# ------------------------------------------------------
def load_env():
    command = ['bash', '-c', 'source tokens.sh && env']
    proc = subprocess.Popen(command, stdout=subprocess.PIPE, universal_newlines=True)

    for line in proc.stdout:
        key, _, value = line.partition("=")
        if key in ["CANVAS_API_TOKEN", "CANVAS_BASE_URL", "CANVAS_COURSE_ID"]:
            os.environ[key] = value.strip()

    proc.communicate()


# ------------------------------------------------------
# Extract <title>...</title> from the HTML
# ------------------------------------------------------
def extract_title(html_text):
    match = re.search(r"<title>(.*?)</title>", html_text, re.IGNORECASE | re.DOTALL)
    if not match:
        raise ValueError("HTML file has no <title> tag.")
    return match.group(1).strip()

# ------------------------------------------------------
# Inject local style.css into HTML <head>
# ------------------------------------------------------
def inject_css_into_html(html_text, html_dir):
    css_path = os.path.join(html_dir, "style.css")
    if not os.path.exists(css_path):
        print("No style.css found; skipping CSS embedding.")
        return html_text

    with open(css_path, "r", encoding="utf-8") as f:
        css = f.read()

    css_comment = "\n/* --- Injected from style.css --- */\n"
    injected_css = css_comment + css + "\n"

    # CASE 1: There is an existing <style> ... </style>
    style_match = re.search(r"<style[^>]*>(.*?)</style>", html_text, flags=re.IGNORECASE | re.DOTALL)
    if style_match:
        original_style_block = style_match.group(0)
        original_style_content = style_match.group(1)

        # Merge into existing style block
        new_style_content = original_style_content + injected_css
        new_style_block = re.sub(
            r"<style[^>]*>.*?</style>",
            f"<style>{new_style_content}</style>",
            original_style_block,
            flags=re.DOTALL
        )

        html_text = html_text.replace(original_style_block, new_style_block)
        return html_text

    # CASE 2: No <style> block → inject at top of <head>
    return html_text.replace(
        "</head>",
        f"<style>{injected_css}</style></head>"
    )

# ------------------------------------------------------
# Slugify title for Canvas page URL
# ------------------------------------------------------
def slugify(title):
    slug = title.lower()
    slug = re.sub(r"[^a-z0-9]+", "-", slug)
    slug = slug.strip("-")
    return slug


# ------------------------------------------------------
# Upload a file (image) to Canvas course files
# Returns the final public URL of the uploaded file
# ------------------------------------------------------
def upload_file_to_canvas(image_path, base_url, course_id, token):
    file_name = basename(image_path)
    headers = {"Authorization": f"Bearer {token}"}

    # Step 1: Start upload
    init_url = f"{base_url}/api/v1/courses/{course_id}/files"
    init_params = {
        "name": file_name,
        "parent_folder_path": "uploaded_html_images",   # put all images in a folder
        "on_duplicate": "rename"
    }

    init_resp = requests.post(init_url, headers=headers, data=init_params)
    if init_resp.status_code != 200:
        raise RuntimeError(f"Could not initiate file upload for {file_name}: {init_resp.text}")

    upload_url = init_resp.json()['upload_url']
    upload_params = init_resp.json()['upload_params']

    # Step 2: Actual upload
    with open(image_path, "rb") as f:
        upload_resp = requests.post(upload_url, data=upload_params, files={"file": f})

    if upload_resp.status_code not in (200, 201, 302):
        raise RuntimeError(f"Error uploading {file_name}: {upload_resp.text}")

    # Step 3: Canvas returns file info
    # Canvas sometimes returns redirect → follow it
    if upload_resp.status_code == 302:
        final_json = requests.get(upload_resp.headers['Location'], headers=headers).json()
    else:
        final_json = upload_resp.json()

    return final_json["url"]


# ------------------------------------------------------
# Locate all <img src="..."> paths
# ------------------------------------------------------
def find_image_paths(html_text):
    return re.findall(r'<img[^>]+src="([^"]+)"', html_text, flags=re.IGNORECASE)


# ------------------------------------------------------
# Replace local image paths with Canvas-hosted URLs
# ------------------------------------------------------
def replace_image_refs(html_text, rewrite_map):
    for original_path, canvas_url in rewrite_map.items():
        html_text = html_text.replace(original_path, canvas_url)
    return html_text


# ------------------------------------------------------
# Main upload function
# ------------------------------------------------------
def upload_canvas_page(html_path):
    load_env()

    token = os.getenv("CANVAS_API_TOKEN")
    base_url = os.getenv("CANVAS_BASE_URL")
    course_id = os.getenv("CANVAS_COURSE_ID")

    if not token or not base_url or not course_id:
        raise RuntimeError("Missing Canvas environment variables in tokens.sh")

    # Read HTML
    with open(html_path, "r", encoding="utf-8") as f:
        html_content = f.read()

    # Inject CSS before processing images
    html_dir = os.path.dirname(html_path)
    html_content = inject_css_into_html(html_content, html_dir)

    # DEBUG: write modified HTML to a temp file for inspection
    with open("temp.html", "w", encoding="utf-8") as debug_out:
        debug_out.write(html_content)
    print("Wrote debug output to temp.html")

    # Get page title
    page_title = extract_title(html_content)
    page_slug = slugify(page_title)

    # Find images
    html_dir = os.path.dirname(html_path)
    image_refs = find_image_paths(html_content)

    rewrite_map = {}

    for img_src in image_refs:
        # Support relative paths only
        local_path = os.path.join(html_dir, img_src)
        if not os.path.exists(local_path):
            print(f"WARNING: Image not found: {local_path}")
            continue

        print(f"Uploading image: {img_src} ...")
        canvas_url = upload_file_to_canvas(local_path, base_url, course_id, token)
        rewrite_map[img_src] = canvas_url

    # Rewrite HTML to use Canvas URLs
    updated_html = replace_image_refs(html_content, rewrite_map)

    # Upload the page
    endpoint = f"{base_url}/api/v1/courses/{course_id}/pages/{page_slug}"
    headers = {"Authorization": f"Bearer {token}"}
    payload = {
        "wiki_page": {
            "title": page_title,
            "body": updated_html,
            "published": True
        }
    }

    response = requests.put(endpoint, headers=headers, json=payload)

    if response.status_code in (200, 201):
        print("\nPage successfully created/updated!")
        print("Page Title:", page_title)
        print("Canvas Page URL:", response.json().get("html_url"))
    else:
        print("\n✗ Error uploading page:", response.status_code)
        print(response.text)


# ------------------------------------------------------
# Entry point
# ------------------------------------------------------
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python3 upload_page.py <html_file>")
        sys.exit(1)

    upload_canvas_page(sys.argv[1])
