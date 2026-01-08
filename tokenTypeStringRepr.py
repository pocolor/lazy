#!/bin/python3

from string import ascii_uppercase
import sys

indent_level = int(sys.argv[2]) if len(sys.argv) > 2 else 0

enum_values = []
with open(sys.argv[1] if len(sys.argv) > 1 else "tokenTypeEnum", "r") as f:
    data = f.readlines()

    for line in data:
        line = line.strip()
        words = line.split(",")

        for word in words:
            word = word.strip()
            if len(word) > 0 and word[0] in ascii_uppercase:
                enum_values.append(word)

enum_values_string_repr = []
for value in enum_values:
    if "_" in value:
        v = value.split("_")
        v = map(str.capitalize, v)
        v = "".join(v)
        enum_values_string_repr.append(v)
    else:
        enum_values_string_repr.append(value.capitalize())

out = [f"{"\t"*indent_level}case TokenType::{x[0]}: return \"{x[1]}\";" for x in zip(enum_values, enum_values_string_repr)]

print("\n".join(out))
print(f"{"\t"*indent_level}default: return \"<UnknownTokenType>\";")
