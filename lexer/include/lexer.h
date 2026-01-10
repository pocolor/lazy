#pragma once

#include "token.h"

namespace Lazy {
    class Lexer {
    public:
        explicit Lexer(const std::string_view source)
            : m_source(source), m_cursor(source.data()) {
        }

        [[nodiscard]] Token nextToken();

    private:
        std::string_view m_source;
        const char *m_cursor;
        uint32_t m_line = 1, m_column = 1;

        void skipWhitespaceAndComments();

        [[nodiscard]] Token tokenCurrentPos(
            const TokenType type,
            const std::string_view text = std::string_view()
        ) const {
            return Token(type, text, m_line, m_column - text.length());
        }

        [[nodiscard]] Token tokenOp(const TokenType type, const unsigned char len) {
            for (unsigned char i = 1; i < len; ++i) consume();
            return tokenCurrentPos(type, std::string_view(m_cursor - len, len));
        }

        [[nodiscard]] char peek() const { return *m_cursor; }

        [[nodiscard]] char peekNext() const {
            return (m_cursor + 1 < m_source.data() + m_source.size()) ? *(m_cursor + 1) : '\0';
        }

        [[nodiscard]] bool match(const char c) const { return peek() == c; }
        [[nodiscard]] bool matchNext(const char c) const { return peekNext() == c; }

        char consume() {
            m_column++;
            if (peekNext() == '\n') {
                m_line++;
                m_column = 1;
            }
            return *m_cursor++;
        }

        Token lexIdentifierOrKeyword();

        Token lexNumber();

        Token lexString();

        Token lexChar();

        Token lexOperator();
    };
}
