#include "ast.h"

namespace lex {

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
