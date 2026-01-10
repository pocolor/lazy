#include "lexer.h"

using namespace Lazy;

static TokenType lookupKeyword(const std::string_view s) {
    switch (s.length()) {
        case 2:
            if (s == "if") return TokenType::IF;
            if (s == "do") return TokenType::DO;
            if (s == "in") return TokenType::IN;
            if (s == "fn") return TokenType::FN;
            if (s == "as") return TokenType::AS;
            break;
        case 3:
            if (s == "for") return TokenType::FOR;
            if (s == "try") return TokenType::TRY;
            if (s == "var") return TokenType::VAR;
            if (s == "val") return TokenType::VAL;
            if (s == "int") return TokenType::INT;
            break;
        case 4:
            if (s == "else") return TokenType::ELSE;
            if (s == "when") return TokenType::WHEN;
            if (s == "this") return TokenType::THIS;
            if (s == "data") return TokenType::DATA;
            if (s == "enum") return TokenType::ENUM;
            if (s == "open") return TokenType::OPEN;
            if (s == "from") return TokenType::FROM;
            if (s == "char") return TokenType::CHAR;
            if (s == "byte") return TokenType::BYTE;
            if (s == "long") return TokenType::LONG;
            if (s == "true") return TokenType::TRUE;
            if (s == "null") return TokenType::NULL_;
            break;
        case 5:
            if (s == "match") return TokenType::MATCH;
            if (s == "while") return TokenType::WHILE;
            if (s == "break") return TokenType::BREAK;
            if (s == "super") return TokenType::SUPER;
            if (s == "class") return TokenType::CLASS;
            if (s == "async") return TokenType::ASYNC;
            if (s == "await") return TokenType::AWAIT;
            if (s == "yield") return TokenType::YIELD;
            if (s == "catch") return TokenType::CATCH;
            if (s == "throw") return TokenType::THROW;
            if (s == "short") return TokenType::SHORT;
            if (s == "float") return TokenType::FLOAT;
            if (s == "false") return TokenType::FALSE;
            break;
        case 6:
            if (s == "object") return TokenType::OBJECT;
            if (s == "static") return TokenType::STATIC;
            if (s == "extern") return TokenType::EXTERN;
            if (s == "inline") return TokenType::INLINE;
            if (s == "return") return TokenType::RETURN;
            if (s == "public") return TokenType::PUBLIC;
            if (s == "sealed") return TokenType::SEALED;
            if (s == "import") return TokenType::IMPORT;
            if (s == "assert") return TokenType::ASSERT;
            if (s == "string") return TokenType::STRING;
            if (s == "double") return TokenType::DOUBLE;
            if (s == "delete") return TokenType::DELETE;
            break;
        case 7:
            if (s == "permits") return TokenType::PERMITS;
            if (s == "private") return TokenType::PRIVATE;
            if (s == "finally") return TokenType::FINALLY;
            if (s == "boolean") return TokenType::BOOLEAN;
            break;
        case 8:
            if (s == "continue") return TokenType::CONTINUE;
            if (s == "abstract") return TokenType::ABSTRACT;
            if (s == "override") return TokenType::OVERRIDE;
            if (s == "unsigned") return TokenType::UNSIGNED;
            break;
        case 9:
            if (s == "interface") return TokenType::INTERFACE;
            if (s == "protected") return TokenType::PROTECTED;
            if (s == "namespace") return TokenType::NAMESPACE;
            if (s == "typealias") return TokenType::TYPEALIAS;
            break;
        case 10:
            if (s == "annotation") return TokenType::ANNOTATION;
            break;
        default: break;
    }
    return TokenType::IDENTIFIER;
}

inline bool isAlpha(const char c) { return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'; }

inline bool isDigit(const char c) { return c >= '0' && c <= '9'; }

inline bool isAlnum(const char c) { return isAlpha(c) || isDigit(c); }

inline bool isHexDigit(const char c) { return c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f'; }

Token Lexer::nextToken() {
    skipWhitespaceAndComments();

    if (match('\0'))
        return tokenCurrentPos(TokenType::END_OF_FILE);

    if (isDigit(peek()) || match('.') && isDigit(peekNext()))
        return lexNumber();

    if (match('\"') || match('r') && matchNext('\"'))
        return lexString();

    if (match('\''))
        return lexChar();

    if (isAlpha(peek()) || match('_'))
        return lexIdentifierOrKeyword();

    return lexOperator();
}

void Lexer::skipWhitespaceAndComments() {
    while (match(' ') || match('\t') || match('\n') || match('\r') || match('\f') || match('\v') || match('#')) {
        if (match('#')) {
            while (!match('\n')) consume();
        }
        consume();
    }
}

Token Lexer::lexIdentifierOrKeyword() {
    const char *start = m_cursor;
    while (isAlnum(peek()) || peek() == '_') consume();

    const std::string_view text(start, m_cursor - start);
    return tokenCurrentPos(lookupKeyword(text), text);
}

Token Lexer::lexNumber() {
    const char *start = m_cursor;
    bool isDecimal = false;

    if (peek() == '0' && isAlpha(peekNext())) {
        consume();  // 0
        consume();  // b, o, d, x
        while (isHexDigit(peek()) || isAlpha(peek())) consume();
        return tokenCurrentPos(TokenType::NUMBER_L, std::string_view(start, m_cursor - start));
    }

    while (isDigit(peek())) consume();

    if (peek() == '.') {
        isDecimal = true;
        consume();
        while (isDigit(peek())) consume();
    }

    if (peek() == 'e' || peek() == 'E') {
        isDecimal = true;
        consume();
        if (peek() == '+' || peek() == '-') consume();
        while (isDigit(peek())) consume();
    }

    if (peek() == 'f' || peek() == 'F') {
        isDecimal = true;
        consume();
    }

    return tokenCurrentPos(isDecimal ? TokenType::DECIMAL_L : TokenType::NUMBER_L, std::string_view(start, m_cursor - start));
}

Token Lexer::lexString() {
    const char *start = m_cursor;
    if (match('r')) {
        consume();
        consume();
        while (!match('\"')) consume();
        consume();
        return tokenCurrentPos(TokenType::STRING_L, std::string_view(start, m_cursor - start));
    }

    consume();
    while (!match('\"')) {
        if (match('$')) {
            consume();
            if (match('{')) {
                while (!match('}')) consume();
            }
        }

        if (match('\\') && matchNext('\"')) {
            consume();
        }
        consume();
    }
    consume();

    return tokenCurrentPos(TokenType::STRING_L, std::string_view(start, m_cursor - start));
}

Token Lexer::lexChar() {
    const char *start = m_cursor;
    consume();

    while (!match('\'')) {
        if (match('\\') && matchNext('\'')) {
            consume();
        }
        consume();
    }
    consume();

    return tokenCurrentPos(TokenType::CHAR_L, std::string_view(start, m_cursor - start));
}

Token Lexer::lexOperator() {
    switch (consume()) {
        case '=':
            if (match('=')) return tokenOp(TokenType::DOUBLE_EQUAL, 2);
            if (match('>')) return tokenOp(TokenType::ARROW, 2);
            return tokenOp(TokenType::EQUAL, 1);
        case '+':
            if (match('+')) return tokenOp(TokenType::DOUBLE_PLUS, 2);
            if (match('=')) return tokenOp(TokenType::PLUS_EQUAL, 2);
            return tokenOp(TokenType::PLUS, 1);
        case '-':
            if (match('-')) return tokenOp(TokenType::DOUBLE_MINUS, 2);
            if (match('=')) return tokenOp(TokenType::MINUS_EQUAL, 2);
            return tokenOp(TokenType::MINUS, 1);
        case '*':
            if (match('*') && matchNext('=')) return tokenOp(TokenType::DOUBLE_STAR_EQUAL, 3);
            if (match('*')) return tokenOp(TokenType::DOUBLE_STAR, 2);
            if (match('=')) return tokenOp(TokenType::STAR_EQUAL, 2);
            return tokenOp(TokenType::STAR, 1);
        case '/':
            if (match('/') && matchNext('=')) return tokenOp(TokenType::DOUBLE_SLASH_EQUAL, 3);
            if (match('/')) return tokenOp(TokenType::DOUBLE_SLASH, 2);
            if (match('=')) return tokenOp(TokenType::SLASH_EQUAL, 2);
            return tokenOp(TokenType::SLASH, 1);
        case '%':
            if (match('=')) return tokenOp(TokenType::PERCENT_EQUAL, 2);
            return tokenOp(TokenType::PERCENT, 1);
        case '@':
            if (match('=')) return tokenOp(TokenType::AT_EQUAL, 2);
            return tokenOp(TokenType::AT, 1);

        case '!':
            if (match('=')) return tokenOp(TokenType::EXCLAMATION_MARK_EQUAL, 2);
            if (match('.')) return tokenOp(TokenType::EXCLAMATION_DOT, 2);
            return tokenOp(TokenType::EXCLAMATION_MARK, 1);
        case '>':
            if (match('>') && matchNext('>') && *(m_cursor + 2) == '=') return tokenOp(TokenType::TRIPLE_GREATER_THAN_EQUAL, 4);
            if (match('>') && matchNext('>')) return tokenOp(TokenType::TRIPLE_GREATER_THAN, 3);
            if (match('>') && matchNext('=')) return tokenOp(TokenType::DOUBLE_GREATER_THAN_EQUAL, 3);
            if (match('>')) return tokenOp(TokenType::DOUBLE_GREATER_THAN, 2);
            if (match('=')) return tokenOp(TokenType::GREATER_THAN_EQUAL, 2);
            return tokenOp(TokenType::GREATER_THAN, 1);
        case '<':
            if (match('<') && matchNext('=')) return tokenOp(TokenType::DOUBLE_LESS_THAN_EQUAL, 3);
            if (match('<')) return tokenOp(TokenType::DOUBLE_LESS_THAN, 2);
            if (match('=')) return tokenOp(TokenType::LESS_THAN_EQUAL, 2);
            return tokenOp(TokenType::LESS_THAN, 1);

        case '~':
            if (match('=')) return tokenOp(TokenType::TILDE_EQUAL, 2);
            return tokenOp(TokenType::TILDE, 1);
        case '&':
            if (match('&')) return tokenOp(TokenType::DOUBLE_AMPERSAND, 2);
            if (match('=')) return tokenOp(TokenType::AMPERSAND_EQUAL, 2);
            return tokenOp(TokenType::AMPERSAND, 1);
        case '|':
            if (match('|')) return tokenOp(TokenType::DOUBLE_PIPE, 2);
            if (match('=')) return tokenOp(TokenType::PIPE_EQUAL, 2);
            return tokenOp(TokenType::PIPE, 1);
        case '^':
            if (match('=')) return tokenOp(TokenType::CARET_EQUAL, 2);
            return tokenOp(TokenType::CARET, 1);

        case '.': return tokenOp(TokenType::DOT, 1);
        case ',': return tokenOp(TokenType::COMMA, 1);
        case ':': return tokenOp(TokenType::COLON, 1);
        case ';': return tokenOp(TokenType::SEMICOLON, 1);

        case '?':
            if (match('.')) return tokenOp(TokenType::QUESTION_DOT, 2);
            if (match(':')) return tokenOp(TokenType::QUESTION_COLON, 2);
            return tokenOp(TokenType::QUESTION_MARK, 1);

        case '(': return tokenOp(TokenType::LEFT_PAREN, 1);
        case '{': return tokenOp(TokenType::LEFT_BRACE, 1);
        case '[': return tokenOp(TokenType::LEFT_BRACKET, 1);
        case ')': return tokenOp(TokenType::RIGHT_PAREN, 1);
        case '}': return tokenOp(TokenType::RIGHT_BRACE, 1);
        case ']': return tokenOp(TokenType::RIGHT_BRACKET, 1);

        default: return tokenCurrentPos(TokenType::UNKNOWN);
    }
}
