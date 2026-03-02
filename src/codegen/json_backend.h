#pragma once

#include "backend.h"
#include <sstream>

namespace lex {

class JsonBackend : public Backend {
public:
    std::string name() const override { return "json"; }
    std::string file_extension() const override { return ".json"; }
    
    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override;

private:
    std::string escape_string(const std::string& s);
    std::string generate_era(const Definition& era);
    std::string generate_structure(const Definition& structure);
    std::string generate_unit(const Definition& unit);
    std::string generate_technology(const Definition& tech);
    std::string generate_resource_map(const ResourceMap& map);
    std::string generate_reference_list(const ReferenceList& list);
    
    std::string get_string_property(const Definition& def, const std::string& name);
    int64_t get_int_property(const Definition& def, const std::string& name, int64_t default_val = 0);
    const ResourceMap* get_resource_map_property(const Definition& def, const std::string& name);
    const ReferenceList* get_reference_list_property(const Definition& def, const std::string& name);
};

} // namespace lex
