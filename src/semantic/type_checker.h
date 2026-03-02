#pragma once

#include "../ast/ast.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

namespace lex {

struct TypeError {
    std::string message;
    std::string location;
    std::string context;  // What was being checked
};

class TypeChecker {
public:
    // Check entire AST, returns true if no errors
    bool check(const std::vector<std::unique_ptr<Definition>>& definitions);

    // Access results
    bool has_errors() const { return !errors_.empty(); }
    const std::vector<TypeError>& errors() const { return errors_; }

    // Type inference for expressions
    LexType infer_expression_type(const Expression* expr);

    // Register known definitions for reference resolution
    void register_definition(const Definition* def);
    void register_definitions(const std::vector<std::unique_ptr<Definition>>& definitions);

    // Property type schema (can be extended)
    using PropertyTypeValidator = std::function<bool(const PropertyValue*, const std::string&)>;

    // Register custom validators for specific properties
    void set_property_validator(const std::string& property_name,
                                PropertyTypeValidator validator);

private:
    std::vector<TypeError> errors_;
    std::unordered_map<std::string, const Definition*> definitions_;

    // Property type expectations per definition type
    std::unordered_map<std::string, LexType> expected_property_types_;

    void add_error(const std::string& message, const std::string& location, const std::string& context);

    // Type checking helpers
    bool check_definition(const Definition* def);
    bool check_property(const Property* prop, const Definition* context);
    bool check_condition(const Condition* cond, const Definition* context);
    bool check_expression(const Expression* expr, const std::string& context);

    // Binary operator type rules
    bool is_valid_binary_operand(LexType left, LexType right,
                                 Expression::BinaryOp op) const;

    // Get expected type for a property in a definition type
    std::optional<LexType> get_expected_property_type(
        const std::string& definition_type,
        const std::string& property_name) const;
};

} // namespace lex
