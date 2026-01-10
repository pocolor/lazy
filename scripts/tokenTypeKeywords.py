#!/bin/python3

import sys
import re
from pathlib import Path
from collections import defaultdict

indent_level = int(sys.argv[1]) if len(sys.argv) > 1 else 0
token_h_filepath = Path(__file__).resolve().parents[1] / "lexer" / "include" / "token.h"
file_path = Path(sys.argv[2]) if len(sys.argv) > 2 else token_h_filepath

if not file_path.exists():
    print(f"Error: {file_path} not found.", file=sys.stderr)
    sys.exit(1)

content = file_path.read_text()
enum_match = re.search(r"enum\s+class\s+TokenType.*?\{(.*?)\};", content, re.DOTALL)
if not enum_match:
    sys.exit("Error: TokenType enum not found.")

enum_body = enum_match.group(1)
parts = re.split(r"//\s*KEYWORDS", enum_body, flags=re.IGNORECASE)
if len(parts) < 2:
    sys.exit("Error: // KEYWORDS marker not found.")

keywords_section = re.split(r"//\s*OPERATORS", parts[1], flags=re.IGNORECASE)[0]
tokens = re.findall(r"\b[a-zA-Z_]+\b", re.sub(r"//.*", "", keywords_section))

grouped_tokens = defaultdict(list)
for t in tokens:
    grouped_tokens[len(t)].append(t)

def get_indent(level = 0): return "\t" * (indent_level + level)

print(f"{get_indent()}switch (s.length()) {{")

for length in sorted(grouped_tokens.keys()):
    print(f"{get_indent(1)}case {length}:")
    for t in grouped_tokens[length]:
        print(f'{get_indent(2)}if (s == "{t.lower()}") return TokenType::{t};')
    print(f"{get_indent(2)}break;")

print(f"{get_indent(1)}default: break;")
print(f"{get_indent()}}}")
print(f"{get_indent()}return TokenType::IDENTIFIER;")
