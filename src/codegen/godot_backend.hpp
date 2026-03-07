#pragma once

#include "backend.hpp"
#include <set>

namespace lex {

class GodotBackend : public Backend {
public:
    std::string name() const override { return "godot"; }
    std::string file_extension() const override { return ".gd"; }

    // Configure from source name (derive class name)
    void configure(const std::string& source_name) override;

    // Set custom class name (default: "GameData")
    void set_class_name(const std::string& name) { class_name_ = name; }

    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override;

private:
    std::set<std::string> seen_types_;
    std::string class_name_ = "GameData";

    std::string type_to_var_name(const std::string& type) const;

    std::string generate_definition(const Definition& def);
    std::string generate_property_value(const Property& prop);
    std::string generate_resource_map(const ResourceMap& map);
    std::string generate_reference_list(const ReferenceList& list);
    std::string generate_object_value(const ObjectValue& obj);
    std::string generate_array_value(const ArrayValue& arr);
    std::string generate_property_value_nested(const PropertyValue& pv);
    std::string generate_expression(const Expression* expr);

    std::string escape_string(const std::string& s) const;
};

}
