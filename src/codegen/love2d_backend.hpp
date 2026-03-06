#pragma once

#include "backend.hpp"
#include <set>

namespace lex {

class Love2DBackend : public Backend {
public:
    std::string name() const override { return "love2d"; }
    std::string file_extension() const override { return ".lua"; }

    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override;

private:
    std::set<std::string> seen_types_;

    std::string generate_definition(const Definition& def);
    std::string generate_generic(const Definition& def);
    std::string generate_resource_map(const ResourceMap& map);
    std::string generate_reference_list(const ReferenceList& list);
    std::string type_to_table_name(const std::string& type) const;

    std::string generate_expression(const Expression* expr, bool in_condition = false);
    std::string generate_binary_expression(const Expression* expr, bool in_condition = false);
    std::string generate_unary_expression(const Expression* expr, bool in_condition = false);
    std::string generate_call_expression(const Expression* expr, bool in_condition = false);
    std::string generate_member_expression(const Expression* expr, bool in_condition = false);

    std::string generate_condition(const Condition* cond);

    std::string binary_op_to_lua(Expression::BinaryOp op) const;
    std::string unary_op_to_lua(Expression::UnaryOp op) const;

    std::string generate_property_value(const Property& prop);
};

}
