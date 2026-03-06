#pragma once

#include "backend.hpp"
#include <set>
#include <sstream>

namespace lex {

class UnityBackend : public Backend {
public:
    std::string name() const override { return "cs"; }
    std::string file_extension() const override { return ".cs"; }

    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override;

private:
    std::set<std::string> seen_types_;

    std::string type_to_class_name(const std::string& type) const;

    std::string generate_scriptable_object_class(const std::string& type_name,
                                                   const std::vector<const Definition*>& defs);
    std::string generate_property_field(const Property& prop);
    std::string generate_resource_map_class(const std::string& name);
    std::string generate_definition_data(const Definition& def);
    std::string generate_property_value(const Property& prop);

    std::string escape_string(const std::string& s) const;
    std::string csharp_type(const Property& prop) const;
};

}
