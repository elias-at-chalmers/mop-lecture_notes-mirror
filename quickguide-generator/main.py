import xml.etree.ElementTree as ET
import re
from collections import defaultdict
import sys


with open("ch32v30x.svd", "r", encoding="utf-8") as f:
    xml = f.read()

root = ET.fromstring(xml)

peripherals = root.find('peripherals')

html = ""

table_css = "style = 'border-collapse: collapse; table-layout: fixed; width: 100%;'"
shrink_css = "style = 'width: 5%;'"
middle_css = "style = 'width: 1%;'"
unused_css = "style = 'background-color: lightgray;'"
th_css = "style = 'border: 1px solid black;  padding: 1px;  text-align: center;  white-space: nowrap; font-family: \"Arial Narrow\", sans-serif;'"
td_css = "style = 'border: 1px solid black;  padding: 1px;  text-align: center;  white-space: nowrap; font-family: \"Arial Narrow\", sans-serif;'"
td_gray_css = "style = 'background-color: lightgray; border: 1px solid black;  padding: 1px;  text-align: center;  white-space: nowrap; font-family: \"Arial Narrow\", sans-serif;'"

def th(text): 
    html = "<th " + th_css + ">"
    html += text + "</th>\n"
    return html
def td(text): 
    html = "<td " + td_css + ">"
    html += text + "</td>\n"
    return html

isolate = "GPIOA"

def ParseBitFields(fields):
    # The fields in a register are described _either_ as <bitOffset> and <bitWidth>
    # _or_ <bitRange> (e.g. "[15:7]") OR <msb><lsb> :)
    fieldsdict = []
    for f in fields:
        if f.find('bitOffset') is not None:
            fieldsdict.append({"offset":int(f.find('bitOffset').text), "width":int(f.find('bitWidth').text), "name":f.find('name').text, "description":f.find('description').text if f.find('description') is not None else "" })
        elif  f.find('bitRange') is not None:
            s = f.find('bitRange').text
            a_str, b_str = s.strip("[]").split(":")
            a, b = int(a_str), int(b_str)
            fieldsdict.append({"offset":b, "width":b - a, "name":f.find('name').text, "description":f.find('description').text if f.find('description') is not None else "" })
    return fieldsdict


def PrintPeripheralOverviewTable(p):
    html = ""

    numbits = 32 # Assuming 32 if nothing else stated
    if(p.find('size')) is not None:
        numbits = int(p.find('size').text)

    html += '<table ' + table_css + '>\n'
    html += "<colgroup>"
    html += "<col " + shrink_css +">"
    for i in range(0, 32): html += "<col " + middle_css + ">"
    html += "<col " + shrink_css + ">"
    html += "</colgroup>"

    html += "<tr>" 
    html += th("offset")
    for i in range(0, 32): html += th(str(31 - i))
    html += th("Register")
    html += "</tr>\n"
    for r in p.find('registers'):

        html += "<tr>\n"
        html += td(r.find('addressOffset').text)

        if(r.find('fields') is not None):
            fields = ParseBitFields(r.find('fields'))
            fields.sort(key=lambda item: item["offset"], reverse=True)


        bit = 32
        for f in fields:
            # Check if we have empty space
            if (f["offset"] + f["width"]) != bit:
                html += "<td colspan=" + str(bit - (f["offset"] + f["width"])) + " " + td_gray_css + "> </td>\n"
            # Draw field
            html += "<td colspan=" + str(f["width"]) + " " + td_css + ">" + " " + "</td>\n"            
            bit = f["offset"]

        #html += "<td>"+ r.find('name').text + "</td>"
        html += td(r.find('name').text)
        html += "</tr>\n"

    html += "</table>"
    return html

def clean_whitespace(text: str) -> str:
    # Replace line breaks with spaces
    text = text.replace("\n", " ").replace("\r", " ")
    # Collapse multiple spaces into one
    text = re.sub(r"\s+", " ", text)
    # Strip leading/trailing spaces
    return text.strip()

def summarize_fields(fields):
    groups = defaultdict(list)

    for entry in fields:
        name = entry.get("name", "")
        match = re.match(r"([A-Za-z_]+)(\d+)$", name)
        if match:
            prefix, number = match.groups()
            groups[prefix].append((int(number), clean_whitespace(entry.get("description", ""))))

    overview = []
    for prefix, items in groups.items():
        numbers = [n for n, _ in items]
        desc = ""
        # try to grab the description from the lowest-numbered entry
        for n, d in sorted(items):
            if d:
                desc = d
                break
        overview.append(f"<b>{prefix}</b>n (n = {min(numbers)}..{max(numbers)}): {desc}")

    return overview


def PrintRegisterDetails(r):
    html = "<b>" + r.find('name').text + "</b>\n"
    html += "<p>\n";
    if r.find('description') is not None:
        html += r.find('description').text + "<br>\n"
    html += "<small>"
    if r.find('access') is not None:
        html += "Access: " + r.find('access').text + "  "
    if r.find('resetValue') is not None:
        html += "Reset Value: " + r.find('resetValue').text + "  "
    if r.find('resetMask') is not None:
        html += "Reset Mask: " + r.find('resetMask').text + "  "
    html += "</small>\n"
    html += "</p>\n"


    html += '<table ' + table_css + '>\n'
    html += '<tr>'

    html += "<colgroup>"
    for i in range(0, 32): html += "<col " + middle_css + ">"
    html += "</colgroup>"

    if(r.find('fields') is not None):
        fields = ParseBitFields(r.find('fields'))
        fields.sort(key=lambda item: item["offset"], reverse=True)

        html += "<tr>"
        for i in range(0, 32): html += th(str(31 - i))
        html += "</tr>"

        bit = 32
        for f in fields: 
            # Check if we have empty space
            if (f["offset"] + f["width"]) != bit:
                html += "<td colspan=" + str(bit - (f["offset"] + f["width"])) + " " + td_gray_css + "> </td>\n"
#                html += "<td class='unused-bit' colspan=" + str(bit - (f["offset"] + f["width"])) + ">"
                html += "</td>"
            # Draw field
            #html += "<td colspan=" + str(f["width"]) + ">" 
            html += "<td colspan=" + str(f["width"]) + " " + td_css + ">" 
            html += "<small>" + f["name"] + "</small></td>\n"
            bit = f["offset"]        

        html += '</tr>'
    html += '</table>'

    # Information about each field, but if there are n , e.g., CNFn fields then only one description

    if(r.find('fields') is not None):
        for line in summarize_fields(fields):
            html += line + "<br>\n"
        
    html += "<hr>\n"

    return html


# Things I want it to do: 
# * Print the overview table for a peripheral
# Ex: ./quickguide-generator.py overview-table GPIOA
# * Print the details for each register in the peripheral
# Ex: ./quickguide-generator.py register-details GPIOA CFG*
# * Print the base adresses for a peripheral regexp
# Ex: ./quickguide-generator.py baseaddress GPIO*

if(sys.argv[1] == "overview-table"): 
    if(len(sys.argv) != 3):     
        print("Usage: " + sys.argv[0]  + "overview-table <peripheral>")
        exit(1)
    
    found = False
    for p in peripherals:

        if p.find('name').text != sys.argv[2]: continue
        found = True
        p_name = p.find('name').text
        if "derivedFrom" in p.attrib: 
            html += "<b>WARNING: Peripheral is derived from " + p.attrib["derivedFrom"] + ", and will not parse correctly yet</b><br>\n"
            continue
        html += PrintPeripheralOverviewTable(p)
    if not found:
        print("Peripheral " + sys.argv[2] + " not found")
        exit(1)

if(sys.argv[1] == "baseaddress"):
    if(len(sys.argv) != 3):     
        print("Usage: " + sys.argv[0]  + "baseaddress <peripheral-regexp>")
        exit(1)
    
    pattern = re.compile(sys.argv[2])

    found = False
    for p in peripherals:

        if not pattern.match(p.find('name').text): continue
        found = True
        p_name = p.find('name').text
        html += p_name + ": <code>" + p.find("baseAddress").text + "</code><br>\n"
    if not found:
        print("Peripheral matching " + sys.argv[2] + " not found")
        exit(1)

if(sys.argv[1] == "register-details"):
    if(len(sys.argv) != 4):     
        print("Usage: " + sys.argv[0]  + "register-details <peripheral> <register-regexp>")
        exit(1)
    
    pattern = re.compile(sys.argv[3])

    found = False
    for p in peripherals:

        if p.find('name').text != sys.argv[2]: continue
        found = True
        p_name = p.find('name').text
        if "derivedFrom" in p.attrib: 
            html += "<b>WARNING: Peripheral is derived from " + p.attrib["derivedFrom"] + ", and will not parse correctly yet</b><br>\n"
            continue

        for r in p.find('registers'):        
            if not pattern.match(r.find('name').text): continue
            html += PrintRegisterDetails(r)
    if not found:
        print("Peripheral " + sys.argv[2] + " not found")
        exit(1)

print(html)
