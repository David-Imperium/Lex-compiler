#pragma once

#include "backend.hpp"
#include <sstream>
#include <map>

namespace lex {

class JsonBackend : public Backend {
public:
    std::string name() const override { return "json"; }
    std::string file_extension() const override { return ".json"; }

    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override;

private:
    std::string escape_string(const std::string& s);

    // Generic definition generator (works for any type)
    std::string generate_generic(const Definition& def);
    std::string generate_property_value(const Property& prop);

    // Collection generators
    std::string generate_resource_map(const ResourceMap& map);
    std::string generate_reference_list(const ReferenceList& list);
    
    // Nested value generators
    std::string generate_object_value(const ObjectValue& obj);
    std::string generate_array_value(const ArrayValue& arr);
    std::string generate_property_value_nested(const PropertyValue& pv);
};

} // namespace lex
