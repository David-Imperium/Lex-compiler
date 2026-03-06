#pragma once

#include <vector>
#include <string>
#include "token.hpp"

namespace lex {

class Lexer {
public:
    explicit Lexer(const std::string& source, const std::string& filename = "<stdin>");

    std::vector<Token> tokenize();
    Token next_token();
    Token peek_token();

    bool has_errors() const { return !errors_.empty(); }
    const std::vector<std::string>& errors() const { return errors_; }

private:
    std::string source_;
    std::string filename_;
    size_t pos_ = 0;
    int line_ = 1;
    int column_ = 1;

    std::vector<std::string> errors_;

    char current() const;
    char peek() const;
    void advance();
    void skip_whitespace();
    void skip_comment();

    Token scan_token();
    Token scan_string();
    Token scan_number();
    Token scan_identifier();
    Token scan_hex_color();
};

}
