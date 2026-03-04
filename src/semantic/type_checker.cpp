#include "type_checker.h"
#include "../schema/schema.h"
#include <sstream>

namespace lex {

bool TypeChecker::check(const std::vector<std::unique_ptr<Definition>>& definitions) {
    // First pass: register all definitions
    register_definitions(definitions);

    // Second pass: type check each definition
    bool all_valid = true;
    for (const auto& def : definitions) {
        if (!check_definition(def.get())) {
            all_valid = false;
        }
    }

    return all_valid;
}

void TypeChecker::register_definition(const Definition* def) {
    if (def) {
        definitions_[def->identifier] = def;
    }
}

void TypeChecker::register_definitions(const std::vector<std::unique_ptr<Definition>>& definitions) {
    for (const auto& def : definitions) {
        register_definition(def.get());
    }
}

void TypeChecker::set_property_validator(const std::string& property_name,
                                         PropertyTypeValidator validator) {
    // Reserved for custom validators - not used in basic implementation
    (void)property_name;
    (void)validator;
}

void TypeChecker::add_error(const std::string& message, const std::string& location,
                            const std::string& context) {
    errors_.push_back({message, location, context});
}

LexType TypeChecker::infer_expression_type(const Expression* expr) {
    if (!expr) return LexType::UNKNOWN;
    return expr->infer_type();
}

bool TypeChecker::check_definition(const Definition* def) {
    if (!def) return true;  // Null is ok

    std::string context = def->definition_type + " '" + def->identifier + "'";
    bool valid = true;

    // Check all properties
    for (const auto& prop : def->properties) {
        if (!check_property(prop.get(), def)) {
            valid = false;
        }
    }

    // Check all conditions
    for (const auto& cond : def->conditions) {
        if (!check_condition(cond.get(), def)) {
            valid = false;
        }
    }

    return valid;
}

bool TypeChecker::check_property(const Property* prop, const Definition* context_def) {
    if (!prop || !prop->value) return true;

    std::string context = "property '" + prop->name + "' in " +
                          context_def->definition_type + " '" + context_def->identifier + "'";

    // Check expression-type properties
    if (prop->value->type == PropertyValue::Type::EXPRESSION) {
        if (!check_expression(prop->value->expression.get(), context)) {
            return false;
        }

        // Validate against expected type if known
        auto expected = get_expected_property_type(context_def->definition_type, prop->name);
        if (expected) {
            LexType actual = infer_expression_type(prop->value->expression.get());
            if (actual != LexType::UNKNOWN && actual != *expected) {
                // Some type mismatches are warnings, not errors
                // For now, we only error on clear incompatibilities
                if ((*expected == LexType::INTEGER && actual == LexType::FLOAT) ||
                    (*expected == LexType::FLOAT && actual == LexType::INTEGER)) {
                    // Numeric compatibility - ok
                } else if (*expected == LexType::REFERENCE &&
                           (actual == LexType::STRING || actual == LexType::REFERENCE)) {
                    // Reference can be string or reference - ok
                } else {
                    add_error("Type mismatch: expected " + type_to_string(*expected) +
                             ", got " + type_to_string(actual),
                             "", context);
                    return false;
                }
            }
        }
    }

    return true;
}

bool TypeChecker::check_condition(const Condition* cond, const Definition* context_def) {
    if (!cond) return true;

    std::string context = cond->trigger + " condition in " +
                          context_def->definition_type + " '" + context_def->identifier + "'";

    // Check the condition expression
    if (cond->expression) {
        if (!check_expression(cond->expression.get(), context)) {
            return false;
        }

        // Conditions should evaluate to boolean
        LexType expr_type = infer_expression_type(cond->expression.get());
        if (expr_type != LexType::UNKNOWN && expr_type != LexType::BOOLEAN) {
            add_error("Condition expression must be boolean, got " + type_to_string(expr_type),
                     "", context);
            return false;
        }
    }

    // Check condition block properties
    for (const auto& prop : cond->properties) {
        if (!check_property(prop.get(), context_def)) {
            return false;
        }
    }

    return true;
}

bool TypeChecker::check_expression(const Expression* expr, const std::string& context) {
    if (!expr) return true;

    switch (expr->type) {
        case Expression::Type::INTEGER:
        case Expression::Type::FLOAT:
        case Expression::Type::STRING:
        case Expression::Type::BOOLEAN:
        case Expression::Type::COLOR:
        case Expression::Type::NULL_VAL:
            // Literals are always valid
            return true;

        case Expression::Type::REFERENCE: {
            // Reference - check if definition exists (warning only if not found)
            if (definitions_.find(expr->reference) == definitions_.end()) {
                // Not necessarily an error - could be a game state variable
                // For now, we don't error on unknown references
            }
            return true;
        }

        case Expression::Type::UNARY: {
            if (!expr->operand) {
                add_error("Unary operator missing operand", "", context);
                return false;
            }
            if (!check_expression(expr->operand.get(), context)) {
                return false;
            }
            LexType operand_type = infer_expression_type(expr->operand.get());

            if (expr->unary_op == Expression::UnaryOp::NOT) {
                if (operand_type != LexType::UNKNOWN && operand_type != LexType::BOOLEAN) {
                    add_error("'not' operator requires boolean operand, got " +
                             type_to_string(operand_type), "", context);
                    return false;
                }
            } else if (expr->unary_op == Expression::UnaryOp::NEG) {
                if (operand_type != LexType::UNKNOWN &&
                    operand_type != LexType::INTEGER &&
                    operand_type != LexType::FLOAT) {
                    add_error("Negation requires numeric operand, got " +
                             type_to_string(operand_type), "", context);
                    return false;
                }
            }
            return true;
        }

        case Expression::Type::BINARY: {
            if (!expr->left || !expr->right) {
                add_error("Binary operator missing operand(s)", "", context);
                return false;
            }
            if (!check_expression(expr->left.get(), context)) {
                return false;
            }
            if (!check_expression(expr->right.get(), context)) {
                return false;
            }

            LexType left_type = infer_expression_type(expr->left.get());
            LexType right_type = infer_expression_type(expr->right.get());

            if (!is_valid_binary_operand(left_type, right_type, expr->binary_op)) {
                add_error("Invalid operand types for operator: " +
                         type_to_string(left_type) + " and " + type_to_string(right_type),
                         "", context);
                return false;
            }
            return true;
        }

        case Expression::Type::CALL: {
            // Check all arguments
            bool valid = true;
            for (const auto& arg : expr->arguments) {
                if (!check_expression(arg.get(), context)) {
                    valid = false;
                }
            }
            // Function calls are always valid for now (no function type signatures)
            return valid;
        }

        case Expression::Type::MEMBER: {
            if (!expr->object) {
                add_error("Member access missing object", "", context);
                return false;
            }
            return check_expression(expr->object.get(), context);
        }

        default:
            return true;
    }
}

bool TypeChecker::is_valid_binary_operand(LexType left, LexType right,
                                          Expression::BinaryOp op) const {
    // UNKNOWN is always acceptable (deferred type checking)
    if (left == LexType::UNKNOWN || right == LexType::UNKNOWN) {
        return true;
    }

    switch (op) {
        case Expression::BinaryOp::ADD:
        case Expression::BinaryOp::SUB:
        case Expression::BinaryOp::MUL:
        case Expression::BinaryOp::DIV:
        case Expression::BinaryOp::MOD:
            // Arithmetic: both operands should be numeric
            return (left == LexType::INTEGER || left == LexType::FLOAT) &&
                   (right == LexType::INTEGER || right == LexType::FLOAT);

        case Expression::BinaryOp::EQ:
        case Expression::BinaryOp::NE:
            // Equality: any types can be compared
            return true;

        case Expression::BinaryOp::GT:
        case Expression::BinaryOp::LT:
        case Expression::BinaryOp::GE:
        case Expression::BinaryOp::LE:
            // Comparison: numeric or string comparison
            return (left == LexType::INTEGER || left == LexType::FLOAT ||
                    left == LexType::STRING) &&
                   (right == LexType::INTEGER || right == LexType::FLOAT ||
                    right == LexType::STRING);

        case Expression::BinaryOp::AND:
        case Expression::BinaryOp::OR:
            // Logical: both operands should be boolean
            return left == LexType::BOOLEAN && right == LexType::BOOLEAN;

        default:
            return true;
    }
}

std::optional<LexType> TypeChecker::get_expected_property_type(
    const std::string& definition_type,
    const std::string& property_name) const {

    // Common properties across all definitions
    if (property_name == "name" || property_name == "description" ||
        property_name == "icon" || property_name == "tooltip" ||
        property_name == "quote" || property_name == "text") {
        return LexType::STRING;
    }
    if (property_name == "era") {
        return LexType::REFERENCE;
    }

    // Try schema-driven lookup
    auto& schema = SchemaRegistry::instance();
    auto def_schema = schema.get_definition(definition_type);
    if (def_schema) {
        auto prop_schema = def_schema->get_property(property_name);
        if (prop_schema) {
            // Convert type_hint to LexType
            const std::string& hint = prop_schema->type_hint;
            if (hint == "int" || hint == "integer") return LexType::INTEGER;
            if (hint == "float" || hint == "double") return LexType::FLOAT;
            if (hint == "string" || hint == "str") return LexType::STRING;
            if (hint == "bool" || hint == "boolean") return LexType::BOOLEAN;
            if (hint == "resource_map") return LexType::RESOURCE_MAP;
            if (hint == "reference_list") return LexType::REFERENCE_LIST;
        }
    }

    // Unknown property - no type expectation
    return std::nullopt;
}

} // namespace lex
