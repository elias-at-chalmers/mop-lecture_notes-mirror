import xml.etree.ElementTree as ET

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
            fieldsdict.append({"offset":int(f.find('bitOffset').text), "width":int(f.find('bitWidth').text), "name":f.find('name').text})
        elif  f.find('bitRange') is not None:
            s = f.find('bitRange').text
            a_str, b_str = s.strip("[]").split(":")
            a, b = int(a_str), int(b_str)
            fieldsdict.append({"offset":b, "width":b - a, "name":f.find('name').text})
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

def PrintRegisterDetails(r):
    html = "<h3>" + r.find('name').text + "</h3>\n"
    html += "<p>\n";
    if r.find('description') is not None:
        html += r.find('description').text + "<br>\n"
    if r.find('access') is not None:
        html += "Access: " + r.find('access').text + "<br>\n"
    if r.find('resetValue') is not None:
        html += "Reset Value: " + r.find('resetValue').text + "<br>\n"
    if r.find('resetMask') is not None:
        html += "Reset Mask: " + r.find('resetMask').text + "<br>\n"
    html += "</p>\n";


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


    return html

for p in peripherals:
    p_name = p.find('name').text

    if isolate != "" and p_name != isolate: continue

    ###########################################################################
    # Create the overview table for the peripheral
    ###########################################################################
    print(p_name)
    if "derivedFrom" in p.attrib:
        html += "<h1>" + p_name + " (same as " + p.attrib["derivedFrom"] + ")</h1>"
    else:
#        html += "<h1>" + p_name + "</h1>\n"
#        p_description = p.find('description')
#        if p_description is not None:
#            html += "<p>" + p_description.text + "</p>"
#        else:
#            html += "<p> no description </p>"
#        html += "<p>Base Address:" + p.find("baseAddress").text + "</p>"


        html += PrintPeripheralOverviewTable(p)

        for r in p.find('registers'):        
            html += PrintRegisterDetails(r)

 

with open("output.html", "w", encoding="utf-8") as file:
    file.write(html)

    print(html)