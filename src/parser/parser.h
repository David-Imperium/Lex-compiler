#pragma once

#include <memory>
#include <vector>
#include "../lexer/token.h"
#include "../ast/ast.h"

namespace lex {

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    std::vector<std::unique_ptr<Definition>> parse();
    ASTFile parse_file();  // New: returns full AST with imports + definitions

    bool has_errors() const { return !errors_.empty(); }
    const std::vector<std::string>& errors() const { return errors_; }

private:
    std::vector<Token> tokens_;
    size_t pos_ = 0;

    std::vector<std::string> errors_;

    // Token manipulation
    Token& current();
    Token& peek(int offset = 1);
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);

    // Error handling
    void error(const std::string& message);
    void skip_to_next_definition();

    // Helpers
    bool is_definition_keyword(TokenType type);
    bool is_condition_keyword(TokenType type);
    bool is_property_keyword(TokenType type);
    std::string property_token_to_string(TokenType type);

    // Definition parsing
    std::unique_ptr<Definition> parse_definition();
    std::unique_ptr<Definition> parse_generic_definition(const std::string& type_name);

    // Module system
    std::unique_ptr<UseStatement> parse_use_statement();
    std::unique_ptr<ModuleDeclaration> parse_module_declaration();

    // Property parsing
    std::unique_ptr<Property> parse_property();
    std::unique_ptr<PropertyValue> parse_property_value();
    std::unique_ptr<PropertyValue> parse_bracket_value();
    std::unique_ptr<PropertyValue> parse_brace_value();
    std::unique_ptr<ResourceMap> parse_resource_map_content();
    std::unique_ptr<ReferenceList> parse_reference_list_content();

    // Expression parsing (Pratt parser with operator precedence)
    std::unique_ptr<Expression> parse_expression(int min_precedence = 0);
    std::unique_ptr<Expression> parse_primary();
    std::unique_ptr<Expression> parse_unary();
    std::unique_ptr<Expression> parse_postfix(std::unique_ptr<Expression> expr);
    std::unique_ptr<Expression> parse_member_or_call(const std::string& identifier);

    // Operator helpers
    int get_operator_precedence(TokenType type);
    Expression::BinaryOp token_to_binary_op(TokenType type);
    Expression::UnaryOp token_to_unary_op(TokenType type);

    // Condition parsing
    std::unique_ptr<Condition> parse_condition();
};

}
