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
    std::unique_ptr<EraDefinition> parse_era();
    std::unique_ptr<StructureDefinition> parse_structure();
    std::unique_ptr<UnitDefinition> parse_unit();
    std::unique_ptr<TechnologyDefinition> parse_technology();
    std::unique_ptr<ResourceDefinition> parse_resource();
    std::unique_ptr<ChoiceDefinition> parse_choice();
    std::unique_ptr<EndingDefinition> parse_ending();
    std::unique_ptr<EventDefinition> parse_event();
    std::unique_ptr<SecretDefinition> parse_secret();

    // Property parsing
    std::unique_ptr<Property> parse_property();
    std::unique_ptr<PropertyValue> parse_property_value();
    std::unique_ptr<PropertyValue> parse_bracket_value();
    std::unique_ptr<PropertyValue> parse_brace_value();
    std::unique_ptr<ResourceMap> parse_resource_map_content();
    std::unique_ptr<ReferenceList> parse_reference_list_content();

    // Expression parsing
    std::unique_ptr<Expression> parse_expression();
    std::unique_ptr<Expression> parse_primary();

    // Condition parsing
    std::unique_ptr<Condition> parse_condition();
};

}
