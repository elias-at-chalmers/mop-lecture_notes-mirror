import xml.etree.ElementTree as ET
import re
from collections import defaultdict
import sys

###############################################################################
# Argument parser
###############################################################################
import argparse

def build_parser():
    parser = argparse.ArgumentParser(
        prog="program",
        description="Peripheral/register inspection tool"
    )

    # Common flags
    def add_common_flags(p):
        p.add_argument(
            "-no-grouping",
            action="store_true",
            help="Disable grouping in output"
        )
        p.add_argument(
            "-no-folding",
            action="store_true",
            help="Disable grouping in output"
        )
        p.add_argument(
            "-no-headers",
            action="store_true",
            help="Disable column headers in output"
        )
        p.add_argument(
            "-all-open",
            action="store_true",
            help="Make all foldable sections open by default"
        )

    subparsers = parser.add_subparsers(
        dest="command",
        required=True
    )

    # ------------------------------------------------------------
    # overview-table
    # ------------------------------------------------------------
    p_overview = subparsers.add_parser(
        "overview-table",
        help="Show overview table for a peripheral"
    )
    p_overview.add_argument(
        "peripheral",
        help="Peripheral name (exact match)"
    )
    p_overview.add_argument(
        "register_regexp",
        nargs="?",
        default=None,
        help="Optional register name regexp"
    )
    add_common_flags(p_overview)

    # ------------------------------------------------------------
    # baseaddress
    # ------------------------------------------------------------
    p_base = subparsers.add_parser(
        "baseaddress",
        help="Show base addresses of peripherals"
    )
    p_base.add_argument(
        "peripheral_regexp",
        help="Peripheral name regexp"
    )
    add_common_flags(p_base)

    # ------------------------------------------------------------
    # register-details
    # ------------------------------------------------------------
    p_details = subparsers.add_parser(
        "register-details",
        help="Show detailed register information"
    )
    p_details.add_argument(
        "peripheral",
        help="Peripheral name (exact match)"
    )
    p_details.add_argument(
        "register_regexp",
        help="Register name regexp"
    )
    add_common_flags(p_details)

    return parser


def parse_args(argv=None):
    parser = build_parser()
    return parser.parse_args(argv)
###############################################################################
args = None


with open("quickguide-generator/ch32v30x.svd", "r", encoding="utf-8") as f:
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
td_rotated_css = "style = 'writing-mode: vertical-rl; transform: rotate(180deg);border: 1px solid black;  padding: 1px;  text-align: center;  white-space: nowrap; font-family: \"Arial Narrow\", sans-serif;'"

td_gray_css = "style = 'background-color: lightgray; border: 1px solid black;  padding: 1px;  text-align: center;  white-space: nowrap; font-family: \"Arial Narrow\", sans-serif;'"

def th(text): 
    html = "<th " + th_css + ">"
    html += text + "</th>\n"
    return html
def td(text): 
    html = "<td " + td_css + ">"
    html += text + "</td>\n"
    return html


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


def GetRegisterGroups(p): 
    groups = defaultdict(list)

    for r in p.find('registers'):
        name = r.find('name').text
        match = re.match(r"([A-Za-z_]+)(\d+)$", name)
        if match:
            prefix, number = match.groups()
            groups[prefix].append(r)
    return groups

def IsInGroup(r, groups):
    if(args.no_grouping): return False
    name = r.find('name').text
    match = re.match(r"([A-Za-z_]+)(\d+)$", name)
    if match:
        prefix, number = match.groups()
        if len(groups[prefix]) > 1: 
            return True
        else:  return False
    else:  return False

def GetFirstInGroup(r, groups):
    name = r.find('name').text
    match = re.match(r"([A-Za-z_]+)(\d+)$", name)
    if match:
        prefix, number = match.groups()
        return groups[prefix][0]
    else: 
        return None

def GetLastInGroup(r, groups):
    name = r.find('name').text
    match = re.match(r"([A-Za-z_]+)(\d+)$", name)
    if match:
        prefix, number = match.groups()
        return groups[prefix][-1]
    else: 
        return None    

def PrintPeripheralOverviewTable(p, register_regexp=None):
    html = ""

    numbits = 32 # Assuming 32 if nothing else stated
    if(p.find('size')) is not None:
        numbits = int(p.find('size').text)

    html += '<table class="fold-table" ' + table_css + '>\n'
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

    groups = GetRegisterGroups(p)

    for r in p.find('registers'):
        # --- Skip if regexp is given and name does not match ---
        regname = r.find('name').text
        if register_regexp and not re.search(register_regexp, regname):
            continue
        # -------------------------------------------------------


        #######################################################################
        # If this register is part of a group, create a foldable summary row
        #######################################################################
        if IsInGroup(r, groups): 
            if r == GetFirstInGroup(r, groups):
                first = GetFirstInGroup(r, groups)
                last = GetLastInGroup(r, groups)
                html += "<tr class = 'summary'>\n"
                html += td(first.find('addressOffset').text + " - <br>" + last.find('addressOffset').text)
                html += "<td colspan=32 " + td_css + "><i>\n"
                if r.find('description') is not None:
                    html += "Group: " + r.find('description').text + "\n"
                else: html += "Group: <click to expand>\n"
                html += "</i></td>\n"
                html += td(f"{first.find('name').text} ..<br> {last.find('name').text}")
                html += "</tr>\n"
        #######################################################################

            


        if IsInGroup(r, groups): html += "<tr class='detail'>\n"
        else: html += "<tr>\n"
        html += td(r.find('addressOffset').text)

        fields = []
        if r.find('fields') is not None:
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
        # Empty bits in the end
        if bit > 0:
            html += "<td colspan=" + str(bit) + " " + td_gray_css + "> </td>\n"


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
        else: 
            prefix = name
            number = 1
            groups[prefix].append((int(number), clean_whitespace(entry.get("description", ""))))

    overview = []
    for prefix, items in groups.items():
        if(len(groups[prefix]) == 1):
            overview.append(f"<b>{prefix}</b>: {groups[prefix][0][1]}")
        else: 
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
    html = ""

    if not args.no_folding:
        #######################################################################
        # If this register is in a group, fold all together
        #######################################################################
        if IsInGroup(r, groups): 
            first = GetFirstInGroup(r, groups)
            last = GetLastInGroup(r, groups)
            if r == first:
                ###############################################################
                # If first, start detail block
                ###############################################################
                html += "<details" + (" open" if getattr(args, "all_open", False) else "") + ">"
                html += "<summary>" + first.find('name').text + " ... " + last.find('name').text + "</summary>"
                
        #######################################################################
        # If not in group, fold by itself
        #######################################################################
        else: 
            html += "<details" + (" open" if getattr(args, "all_open", False) else "") + ">\n"
            html += "<summary>\n"
            html += r.find('name').text
            html += "</summary>\n"

    # Handle description: allow embedded block-level HTML (tables, divs, lists)
    desc_elem = r.find('description')
    if desc_elem is not None:
        parts = []
        if desc_elem.text:
            parts.append(desc_elem.text)
        for child in desc_elem:
            parts.append(ET.tostring(child, encoding='unicode'))
            if child.tail:
                parts.append(child.tail)
        desc_html = ''.join(parts).strip()

        # Emit block-level HTML as-is, otherwise wrap in a paragraph
        if re.search(r"<(table|div|ul|ol|pre|h[1-6])", desc_html, re.IGNORECASE):
            html += desc_html + "\n<br>\n"
        else:
            html += "<p>\n" + desc_html + "<br>\n</p>\n"
    html += "<small>"
    if r.find('access') is not None:
        html += "Access: " + r.find('access').text + "  "
    if r.find('resetValue') is not None:
        html += "Reset Value: " + r.find('resetValue').text + "  "
    if r.find('resetMask') is not None:
        html += "Reset Mask: " + r.find('resetMask').text + "  "
    html += "</small>\n"


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

            # Check if we need to rotate
            if(f["width"] == 1 and len(f["name"]) > 2):
                html += "<td colspan=" + str(f["width"]) + " " + td_rotated_css + ">" 
            else: 
                html += "<td colspan=" + str(f["width"]) + " " + td_css + ">" 
            html += "<small>" + f["name"] + "</small></td>\n"
            bit = f["offset"]        
        # Empty bits in the end
        if bit > 0:
            html += "<td colspan=" + str(bit) + " " + td_gray_css + "> </td>\n"            

        html += '</tr>'
    html += '</table>'

    # Information about each field, but if there are n , e.g., CNFn fields then only one description

    if(r.find('fields') is not None):
        for line in summarize_fields(fields):
            html += line + "<br>\n"

    #######################################################################
    # CLose details, unless we are in the middle of a group
    #######################################################################
    if not args.no_folding:
        if not IsInGroup(r, groups) or (r == last): 
            html += "</details>\n"
        #html += "<hr>\n"

    return html



def main(): 
    global args
    global html
    global groups
    args = parse_args()
    if(args.command == "overview-table"): 
        found = False
        for p in peripherals:
            if p.find('name').text != args.peripheral: continue
            found = True
            p_name = p.find('name').text
            if "derivedFrom" in p.attrib: 
                html += "<b>WARNING: Peripheral is derived from " + p.attrib["derivedFrom"] + ", and will not parse correctly yet</b><br>\n"
                continue
            register_regexp = args.register_regexp
            html += PrintPeripheralOverviewTable(p, register_regexp)
        if not found:
            print("Peripheral " + args.peripheral + " not found")
            exit(1)

    if(args.command == "baseaddress"):
        pattern = re.compile(args.peripheral_regexp)

        matches = []
        for p in peripherals:
            name = p.find('name').text
            if pattern.match(name):
                matches.append(p)

        if not matches:
            print("Peripheral matching " + args.peripheral_regexp + " not found")
            exit(1)

        # Exactly one match: Don't print name
        if len(matches) == 1:
            p = matches[0]
            p_name = p.find("name").text
            base = p.find("baseAddress").text
            html += f"<code>{base}</code><br>\n"

        # Multiple matches → normal output
        else:
            for p in matches:
                p_name = p.find("name").text
                base = p.find("baseAddress").text
                html += p_name + ": <code>" + base + "</code><br>\n"

    if(args.command == "register-details"):
        pattern = re.compile(args.register_regexp)

        found = False
        for p in peripherals:

            if p.find('name').text != args.peripheral: continue
            found = True
            p_name = p.find('name').text
            if "derivedFrom" in p.attrib: 
                html += "<b>WARNING: Peripheral is derived from " + p.attrib["derivedFrom"] + ", and will not parse correctly yet</b><br>\n"
                continue

            groups = GetRegisterGroups(p)

            for r in p.find('registers'):        
                if not pattern.match(r.find('name').text): continue
                html += PrintRegisterDetails(r)
        if not found:
            print("Peripheral " + args.peripheral + " not found")
            exit(1)

    print(html)

if __name__ == '__main__':
    import sys
    sys.exit(main())
