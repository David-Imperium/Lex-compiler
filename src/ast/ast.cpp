#include "ast.hpp"

namespace lex {

// ============================================================================
// Property Clone Methods
// ============================================================================

std::unique_ptr<ObjectValue> ObjectValue::clone() const {
    auto cloned = std::make_unique<ObjectValue>();
    for (const auto& [name, value] : properties) {
        if (value) {
            cloned->properties.push_back({name, value->clone()});
        }
    }
    return cloned;
}

std::unique_ptr<ArrayValue> ArrayValue::clone() const {
    auto cloned = std::make_unique<ArrayValue>();
    for (const auto& val : values) {
        if (val) {
            cloned->values.push_back(val->clone());
        }
    }
    return cloned;
}

std::unique_ptr<PropertyValue> PropertyValue::clone() const {
    auto cloned = std::make_unique<PropertyValue>();
    cloned->type = type;

    if (expression) {
        cloned->expression = expression->clone();
    }
    if (resource_map) {
        auto new_map = std::make_unique<ResourceMap>();
        new_map->resources = resource_map->resources;
        cloned->resource_map = std::move(new_map);
    }
    if (reference_list) {
        auto new_list = std::make_unique<ReferenceList>();
        new_list->references = reference_list->references;
        cloned->reference_list = std::move(new_list);
    }
    if (object) {
        cloned->object = object->clone();
    }
    if (array) {
        cloned->array = array->clone();
    }

    return cloned;
}

std::unique_ptr<Property> Property::clone() const {
    auto cloned = std::make_unique<Property>();
    cloned->name = name;
    if (value) {
        cloned->value = value->clone();
    }
    return cloned;
}

// ============================================================================
// Type System Helpers
// ============================================================================

std::string type_to_string(LexType type) {
    switch (type) {
        case LexType::UNKNOWN: return "unknown";
        case LexType::INTEGER: return "integer";
        case LexType::FLOAT: return "float";
        case LexType::STRING: return "string";
        case LexType::BOOLEAN: return "boolean";
        case LexType::REFERENCE: return "reference";
        case LexType::RESOURCE_MAP: return "resource_map";
        case LexType::REFERENCE_LIST: return "reference_list";
        case LexType::OBJECT: return "object";
        case LexType::ARRAY: return "array";
        case LexType::COLOR: return "color";
        case LexType::VOID: return "void";
        default: return "unknown";
    }
}

// ============================================================================
// Expression Type Inference
// ============================================================================

LexType Expression::infer_type() const {
    switch (type) {
        case Type::INTEGER:
            return LexType::INTEGER;
        case Type::FLOAT:
            return LexType::FLOAT;
        case Type::STRING:
            return LexType::STRING;
        case Type::BOOLEAN:
            return LexType::BOOLEAN;
        case Type::COLOR:
            return LexType::COLOR;
        case Type::NULL_VAL:
            return LexType::VOID;
        case Type::REFERENCE:
            return LexType::REFERENCE;
        case Type::UNARY:
            if (operand) {
                if (unary_op == UnaryOp::NOT) return LexType::BOOLEAN;
                return operand->infer_type();  // NEG preserves type
            }
            return LexType::UNKNOWN;
        case Type::BINARY:
            // Comparison and logical ops return boolean
            if (binary_op == BinaryOp::EQ || binary_op == BinaryOp::NE ||
                binary_op == BinaryOp::GT || binary_op == BinaryOp::LT ||
                binary_op == BinaryOp::GE || binary_op == BinaryOp::LE ||
                binary_op == BinaryOp::AND || binary_op == BinaryOp::OR) {
                return LexType::BOOLEAN;
            }
            // Arithmetic ops: prefer float if either operand is float
            if (left && right) {
                LexType left_type = left->infer_type();
                LexType right_type = right->infer_type();
                if (left_type == LexType::FLOAT || right_type == LexType::FLOAT) {
                    return LexType::FLOAT;
                }
                return LexType::INTEGER;
            }
            return LexType::UNKNOWN;
        case Type::CALL:
            // Function calls return UNKNOWN unless we have type signatures
            return LexType::UNKNOWN;
        case Type::MEMBER:
            // Member access return UNKNOWN unless we have type info
            return LexType::UNKNOWN;
        default:
            return LexType::UNKNOWN;
    }
}

// ============================================================================
// Expression Clone
// ============================================================================

std::unique_ptr<Expression> Expression::clone() const {
    auto cloned = std::make_unique<Expression>();
    cloned->type = type;
    cloned->inferred_type = inferred_type;
    cloned->value = value;
    cloned->reference = reference;
    cloned->binary_op = binary_op;
    cloned->unary_op = unary_op;
    cloned->function_name = function_name;
    cloned->member_name = member_name;

    // Clone sub-expressions
    if (left) cloned->left = left->clone();
    if (right) cloned->right = right->clone();
    if (operand) cloned->operand = operand->clone();
    if (object) cloned->object = object->clone();

    // Clone arguments vector
    for (const auto& arg : arguments) {
        if (arg) {
            cloned->arguments.push_back(arg->clone());
        }
    }

    return cloned;
}

// ============================================================================
// Definition Typed Accessors
// ============================================================================

std::optional<int64_t> Definition::get_int_property(const std::string& name) const {
    for (const auto& prop : properties) {
        if (prop->name == name && prop->value) {
            if (prop->value->type == PropertyValue::Type::EXPRESSION &&
                prop->value->expression) {
                auto& expr = prop->value->expression;
                if (expr->type == Expression::Type::INTEGER) {
                    return std::get<int64_t>(expr->value);
                }
            }
        }
    }
    return std::nullopt;
}

std::optional<double> Definition::get_float_property(const std::string& name) const {
    for (const auto& prop : properties) {
        if (prop->name == name && prop->value) {
            if (prop->value->type == PropertyValue::Type::EXPRESSION &&
                prop->value->expression) {
                auto& expr = prop->value->expression;
                if (expr->type == Expression::Type::FLOAT) {
                    return std::get<double>(expr->value);
                }
            }
        }
    }
    return std::nullopt;
}

std::optional<std::string> Definition::get_string_property(const std::string& name) const {
    for (const auto& prop : properties) {
        if (prop->name == name && prop->value) {
            if (prop->value->type == PropertyValue::Type::EXPRESSION &&
                prop->value->expression) {
                auto& expr = prop->value->expression;
                if (expr->type == Expression::Type::STRING) {
                    return std::get<std::string>(expr->value);
                } else if (expr->type == Expression::Type::REFERENCE) {
                    return expr->reference;
                }
            }
        }
    }
    return std::nullopt;
}

std::optional<bool> Definition::get_bool_property(const std::string& name) const {
    for (const auto& prop : properties) {
        if (prop->name == name && prop->value) {
            if (prop->value->type == PropertyValue::Type::EXPRESSION &&
                prop->value->expression) {
                auto& expr = prop->value->expression;
                if (expr->type == Expression::Type::BOOLEAN) {
                    return std::get<bool>(expr->value);
                }
            }
        }
    }
    return std::nullopt;
}

const ResourceMap* Definition::get_resource_map_property(const std::string& name) const {
    for (const auto& prop : properties) {
        if (prop->name == name && prop->value) {
            if (prop->value->type == PropertyValue::Type::RESOURCE_MAP) {
                return prop->value->resource_map.get();
            }
        }
    }
    return nullptr;
}

const ReferenceList* Definition::get_reference_list_property(const std::string& name) const {
    for (const auto& prop : properties) {
        if (prop->name == name && prop->value) {
            if (prop->value->type == PropertyValue::Type::REFERENCE_LIST) {
                return prop->value->reference_list.get();
            }
        }
    }
    return nullptr;
}

const Expression* Definition::get_expression_property(const std::string& name) const {
    for (const auto& prop : properties) {
        if (prop->name == name && prop->value) {
            if (prop->value->type == PropertyValue::Type::EXPRESSION) {
                return prop->value->expression.get();
            }
        }
    }
    return nullptr;
}

bool Definition::has_property(const std::string& name) const {
    for (const auto& prop : properties) {
        if (prop->name == name) {
            return true;
        }
    }
    return false;
}

// ============================================================================
// Expression Factory Methods
// ============================================================================

std::unique_ptr<Expression> Expression::make_integer(int64_t val) {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::INTEGER;
    expr->value = val;
    return expr;
}

std::unique_ptr<Expression> Expression::make_float(double val) {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::FLOAT;
    expr->value = val;
    return expr;
}

std::unique_ptr<Expression> Expression::make_string(const std::string& val) {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::STRING;
    expr->value = val;
    return expr;
}

std::unique_ptr<Expression> Expression::make_bool(bool val) {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::BOOLEAN;
    expr->value = val;
    return expr;
}

std::unique_ptr<Expression> Expression::make_null() {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::NULL_VAL;
    return expr;
}

std::unique_ptr<Expression> Expression::make_reference(const std::string& name) {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::REFERENCE;
    expr->reference = name;
    return expr;
}

std::unique_ptr<Expression> Expression::make_binary(BinaryOp op,
                                                     std::unique_ptr<Expression> left,
                                                     std::unique_ptr<Expression> right) {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::BINARY;
    expr->binary_op = op;
    expr->left = std::move(left);
    expr->right = std::move(right);
    return expr;
}

std::unique_ptr<Expression> Expression::make_unary(UnaryOp op,
                                                    std::unique_ptr<Expression> operand) {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::UNARY;
    expr->unary_op = op;
    expr->operand = std::move(operand);
    return expr;
}

std::unique_ptr<Expression> Expression::make_call(const std::string& name,
                                                   std::vector<std::unique_ptr<Expression>> args) {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::CALL;
    expr->function_name = name;
    expr->arguments = std::move(args);
    return expr;
}

std::unique_ptr<Expression> Expression::make_member(std::unique_ptr<Expression> obj,
                                                     const std::string& member) {
    auto expr = std::make_unique<Expression>();
    expr->type = Type::MEMBER;
    expr->object = std::move(obj);
    expr->member_name = member;
    return expr;
}

} // namespace lex
