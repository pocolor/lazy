#include "tokenizer.h"
#include <cstring>

using namespace Lazy::Lexer;

std::ostream& Lazy::Lexer::operator<<(std::ostream& os, const Token& token) {
    os << "TokenType=" << token.type << "\nText=" << (token.text ? token.text : "<nullptr>") << "\nLine=" << token.line << ", Column=" << token.column;
    return os;
}

std::vector<Token> Lazy::Lexer::tokenize(const char* source) {
    std::vector<Token> vec;
    vec.reserve(strlen(source));
    return vec;
}