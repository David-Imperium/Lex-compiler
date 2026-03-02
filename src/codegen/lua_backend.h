#pragma once

#include "backend.h"

namespace lex {

class LuaBackend : public Backend {
public:
    std::string name() const override { return "lua"; }
    std::string file_extension() const override { return ".lua"; }

    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override;

private:
    std::string generate_definition(const Definition& def);
    std::string generate_structure(const Definition& structure);
    std::string generate_unit(const Definition& unit);
    std::string generate_technology(const Definition& tech);
    std::string generate_era(const Definition& era);
    std::string generate_resource_map(const ResourceMap& map);
    std::string generate_reference_list(const ReferenceList& list);

    // Expression code generation
    std::string generate_expression(const Expression* expr);
    std::string generate_binary_expression(const Expression* expr);
    std::string generate_unary_expression(const Expression* expr);
    std::string generate_call_expression(const Expression* expr);
    std::string generate_member_expression(const Expression* expr);

    // Condition code generation
    std::string generate_condition(const Condition* cond);
    std::string generate_condition_block(const Condition* cond);

    // Helpers to extract values from generic properties
    std::string get_string_property(const Definition& def, const std::string& name);
    int64_t get_int_property(const Definition& def, const std::string& name, int64_t default_val = 0);
    const ResourceMap* get_resource_map_property(const Definition& def, const std::string& name);

    // Operator mapping
    std::string binary_op_to_lua(Expression::BinaryOp op) const;
    std::string unary_op_to_lua(Expression::UnaryOp op) const;
};

}
