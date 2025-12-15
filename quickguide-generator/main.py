import xml.etree.ElementTree as ET
import re
from collections import defaultdict
import sys



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
    html += "<details>\n"
    html += "<summary>\n"
    html += r.find('name').text
    html += "</summary>\n"
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
        
    html += "</details>\n"
    #html += "<hr>\n"

    return html


# Things I want it to do: 
# * Print the overview table for a peripheral
# Ex: ./quickguide-generator.py overview-table GPIOA
# * Print the details for each register in the peripheral
# Ex: ./quickguide-generator.py register-details GPIOA CFG*
# * Print the base adresses for a peripheral regexp
# Ex: ./quickguide-generator.py baseaddress GPIO*

if(sys.argv[1] == "overview-table"): 
    if(len(sys.argv) != 3 and len(sys.argv) != 4):     
        print("Usage: " + sys.argv[0]  + "overview-table <peripheral> [register-regexp]")
        exit(1)
    
    found = False
    for p in peripherals:
        if p.find('name').text != sys.argv[2]: continue
        found = True
        p_name = p.find('name').text
        if "derivedFrom" in p.attrib: 
            html += "<b>WARNING: Peripheral is derived from " + p.attrib["derivedFrom"] + ", and will not parse correctly yet</b><br>\n"
            continue
        register_regexp = sys.argv[3] if len(sys.argv) > 3 else None
        html += PrintPeripheralOverviewTable(p, register_regexp)
    if not found:
        print("Peripheral " + sys.argv[2] + " not found")
        exit(1)

if sys.argv[1] == "baseaddress":
    if len(sys.argv) != 3:
        print("Usage: " + sys.argv[0] + " baseaddress <peripheral-regexp>")
        exit(1)

    pattern = re.compile(sys.argv[2])

    matches = []
    for p in peripherals:
        name = p.find('name').text
        if pattern.match(name):
            matches.append(p)

    if not matches:
        print("Peripheral matching " + sys.argv[2] + " not found")
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
