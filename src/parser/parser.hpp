#pragma once

#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "../lexer/token.hpp"
#include "../ast/ast.hpp"

namespace lex {

// Forward declaration
class SchemaRegistry;

// Maximum nesting depth for expressions (prevents stack overflow)
constexpr int MAX_EXPRESSION_DEPTH = 500;

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens, const SchemaRegistry* schema = nullptr);

    std::vector<std::unique_ptr<Definition>> parse();
    ASTFile parse_file();  // New: returns full AST with imports + definitions

    bool has_errors() const { return !errors_.empty(); }
    const std::vector<std::string>& errors() const { return errors_; }

private:
    std::vector<Token> tokens_;
    size_t pos_ = 0;
    int depth_ = 0;  // Current nesting depth
    const SchemaRegistry* schema_;

    std::vector<std::string> errors_;

    // Static lookup tables (initialized once)
    static const std::unordered_set<TokenType>& definition_keywords();
    static const std::unordered_set<TokenType>& condition_keywords();
    static const std::unordered_set<TokenType>& property_keywords();
    static const std::unordered_map<TokenType, std::string>& property_names();

    // Token manipulation
    Token& current();
    Token& peek(int offset = 1);
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);

    // Error handling
    void error(const std::string& message);
    void skip_to_next_definition();

    // Helpers (now use lookup tables)
    bool is_definition_keyword(TokenType type) const;
    bool is_condition_keyword(TokenType type) const;
    bool is_property_keyword(TokenType type) const;
    std::string property_token_to_string(TokenType type) const;

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
    int get_operator_precedence(TokenType type) const;
    Expression::BinaryOp token_to_binary_op(TokenType type) const;
    Expression::UnaryOp token_to_unary_op(TokenType type) const;

    // Condition parsing
    std::unique_ptr<Condition> parse_condition();
};

}
