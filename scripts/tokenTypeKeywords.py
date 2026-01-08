#!/bin/python3

import sys
import re
from pathlib import Path

indent_level = int(sys.argv[1]) if len(sys.argv) > 1 else 0

token_h_filepath = Path(__file__).resolve().parents[1] / "lexer" / "include" / "token.h"
file_path = Path(sys.argv[2]) if len(sys.argv) > 2 else token_h_filepath

if indent_level < 0:
    print("Error: indent_level cannot be negative.", file=sys.stderr)
    sys.exit(1)

if not file_path.exists():
    print(f"Error: {file_path} not found.", file=sys.stderr)
    sys.exit(1)

content = file_path.read_text()

enum_body = re.search(r"enum\s+class\s+TokenType\s*\{(.*?)\};", content, re.DOTALL).group(1)

keywords_section = re.split(r"//\s*KEYWORDS", enum_body, flags=re.IGNORECASE)
keywords_section = re.split(r"//\s*OPERATORS", keywords_section[1], flags=re.IGNORECASE)[0]

clean_keywords = re.sub(r"//.*", "", keywords_section)
tokens = sorted(re.findall(r"\b[a-zA-Z_]+\b", clean_keywords))

def indent(s: str = "\t"): return s * indent_level

keywords = [f"{indent()}\"{t.lower()}\"" for t in tokens]

print(",\n".join(keywords))
