#!/usr/bin/env python3

import json
import argparse
import re
import sys
sys.stdout.reconfigure(encoding="utf-8")
from html import escape


# -----------------------------
# Helper functions
# -----------------------------

def str2bool(v):
    if isinstance(v, bool):
        return v
    if v.lower() in ("true", "yes", "1"):
        return True
    if v.lower() in ("false", "no", "0"):
        return False
    raise argparse.ArgumentTypeError("Boolean value expected.")


def operand_default_explanation(op):
    if op == "rd":
        return "Destination register"
    if op in ("rs", "rs1", "rs2"):
        return "Source register"
    if "(" in op:
        return "Memory address"
    return "Immediate / value"


def instruction_matches(instr, name_re, include_pseudo):
    if not include_pseudo and instr.get("is_pseudo", False):
        return False
    if name_re and not re.search(name_re, instr["instruction"], re.IGNORECASE):
        return False
    return True


# -----------------------------
# Argument parsing
# -----------------------------

parser = argparse.ArgumentParser(description="Generate HTML instruction reference")
parser.add_argument("-category", type=str, default=None, help="Regex to filter categories")
parser.add_argument("-name", type=str, default=None, help="Regex to filter instruction names")
parser.add_argument("-category-header", type=str2bool, default=True)
parser.add_argument("-pseudo-instructions", type=str2bool, default=True)
parser.add_argument("-short", type=str2bool, default=True)

args = parser.parse_args()

category_re = re.compile(args.category, re.IGNORECASE) if args.category else None
name_re = args.name

# -----------------------------
# Load JSON
# -----------------------------

try:
    with open("quickguide-generator/instructions.json", "r", encoding="utf-8") as f:
        data = json.load(f)
except FileNotFoundError:
    sys.exit("Error: instructions.json not found")
except json.JSONDecodeError as e:
    sys.exit(f"Error parsing JSON: {e}")

# -----------------------------
# HTML output
# -----------------------------

print("<!DOCTYPE html>")
print("<html><head>")
print("<meta charset='utf-8'>")
print("<style>")
print("body { font-family: sans-serif; }")
print("table { border-collapse: collapse; width: 100%; }")
print("th, td { border: 1px solid #ccc; padding: 6px; }")
print("th { background: #eee; }")
print("code { background: #f4f4f4; padding: 2px 4px; }")
print("</style>")
print("</head><body>")

for category, instructions in data.items():

    if category_re and not category_re.search(category):
        continue

    filtered = [
        instr for instr in instructions
        if instruction_matches(instr, name_re, args.pseudo_instructions)
    ]

    if not filtered:
        continue

    if args.category_header:
        print(f"<h2>{escape(category)}</h2>")

    # -------------------------
    # SHORT MODE
    # -------------------------
    if args.short:
        print("<table>")
        print("<tr><th>Instruction</th><th>Description</th></tr>")

        for instr in filtered:
            operands = instr.get("operands", [])
            syntax = instr["instruction"]
            if operands:
                syntax += " " + ", ".join(operands)

            print("<tr>")
            print(f"<td><code>{escape(syntax)}</code></td>")
            print(f"<td>{escape(instr.get('shortdesc', ''))}</td>")
            print("</tr>")

        print("</table>")

    # -------------------------
    # LONG MODE
    # -------------------------
    else:
        for instr in filtered:
            print(f"<h3>{escape(instr['instruction'])}</h3>")
            print(f"<p>{escape(instr.get('description', ''))}</p>")

            operands = instr.get("operands", [])
            restrictions = instr.get("restrictions", [])

            syntax = instr["instruction"]
            if operands:
                syntax += " " + ", ".join(operands)

            print(f"<p><b>Syntax:</b> <code>{escape(syntax)}</code></p>")

            if operands:
                print("<table>")
                print("<tr><th>Operand</th><th>Explanation</th></tr>")

                for i, op in enumerate(operands):
                    expl = operand_default_explanation(op)
                    if i < len(restrictions) and restrictions[i]:
                        expl = restrictions[i]

                    print("<tr>")
                    print(f"<td><code>{escape(op)}</code></td>")
                    print(f"<td>{escape(expl)}</td>")
                    print("</tr>")

                print("</table>")

            print("<hr>")

print("</body></html>")
