import json

html = ""

table_css = "style = 'border-collapse: collapse; table-layout: fixed; width: 100%;'"
shrink_css = "style = 'width: 5%;'"
middle_css = "style = 'width: 1%;'"
unused_css = "style = 'background-color: lightgray;'"
th_css = "style = 'border: 1px solid black;  padding: 1px;  text-align: left;  white-space: nowrap; font-family: \"Arial Narrow\", sans-serif;'"
td_css = "style = 'border: 1px solid black;  padding: 1px;  text-align: left;  white-space: nowrap; font-family: \"Arial Narrow\", sans-serif;'"
td_gray_css = "style = 'background-color: lightgray; border: 1px solid black;  padding: 1px;  text-align: left;  white-space: nowrap; font-family: \"Arial Narrow\", sans-serif;'"

def th(text): 
    html = "<th " + th_css + ">"
    html += text + "</th>\n"
    return html
def td(text): 
    html = "<td " + td_css + ">"
    html += text + "</td>\n"
    return html

# Load the JSON file
with open("quickguide-generator/instructions.json", "r") as f:
    data = json.load(f)


def PrintCategoryInstructions(instructions, pseudo):
    ret = ""
    for instr in instructions:         
        name = instr["instruction"]
        operands = instr["operands"]
        restrictions = instr["restrictions"]
        description = instr["description"]
        is_pseudo = instr["is_pseudo"]
        implementation = instr.get("implementation")
        if is_pseudo != pseudo: continue
        ret += "<tr>\n"
        ret += "<td colspan = 2><code>" + name + "</code> - " + description + "</td>\n"
        ret += "</tr>\n"
        ret += "<tr><td colspan = 2>\n"
        ret += "<italic>This is a longer description that should be fetched from the data file, or omitted if not needed. It could be really long and span multiple lines. It really could.</italic>\n"
        ret += "</td></tr>\n"

        ret += "<tr>\n"
        ret += "<td>Syntax:</td>\n"
        ret += "<td><code>" + name + " " + ', '.join(operands) + "</code></td>\n"
        ret += "</tr>\n"

        for i in range(len(operands)):
            ret += "<tr>\n"
            ret += "<td>" + operands[i] + "</td>\n"
            if restrictions[i] is None: 
                restriction = "None"
            else: restriction = restrictions[i]

            if operands[i] == "rd": restriction = "Destination register"
            elif operands[i] == "rs": restriction = "Source register"
            elif operands[i] == "rs1": restriction = "Source register 1"
            elif operands[i] == "rs2": restriction = "Source register 2"

            ret += "<td>" + restriction + "</td>\n"
            ret += "</tr>\n"


        if is_pseudo: 
            ret += "<tr>\n"
            ret += "<td>Implementation:</td>\n"
            ret += "<td><code>" + implementation + "</code></td>\n"
            ret += "</tr>\n"

        ret += "<tr><td colspan=2><hr></td></tr>\n"

    return ret

# Iterate through categories
for category, instructions in data.items():
    html += "<h1>" + category + "</h1>\n"
    html += "<table " + table_css + ">\n"

    html += "<colgroup>"
    html += "<col " + middle_css +">"
    html += "<col " + shrink_css + ">"
    html += "</colgroup>"

    # Real instructions
    html += "<tr>\n"
    html += "<td colspan = 2 " + td_gray_css + "> Real Instructions </td>\n"
    html += "</tr>\n"
    html += PrintCategoryInstructions(instructions, pseudo=False)
    # Pseudo-instructions
    html += "<tr><td colspan = 2 " + td_gray_css + "> Pseudo Instructions </td></tr>\n"
    html += PrintCategoryInstructions(instructions, pseudo=True)
    html += "</table>\n"
    html += "<br>\n"

# Write to output HTML file
with open("quickguide-generator/instructions.html", "w") as f:
    f.write(html)
