#pragma once

#include "token.h"

#include <ostream>
#include <vector>

namespace Lazy {
namespace Lexer {

struct Token {
    TokenType type;
    const char* text;
    unsigned int line, column;

    Token(TokenType type = TokenType::ERROR, const char* text = nullptr, unsigned int line = 0, unsigned int column = 0):
        type(type), text(text), line(line), column(column) {}
};

std::ostream& operator<<(std::ostream& os, const Token& token);

std::vector<Token> tokenize(const char* source);

}
}
