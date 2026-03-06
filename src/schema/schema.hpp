#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <optional>
#include <functional>

namespace lex {

// Definition schema - describes what definition types are allowed
// and what properties they can have.

struct PropertySchema {
    std::string name;
    bool required = false;
    std::string type_hint;  // "int", "float", "string", "bool", "resource_map", "reference_list"
    std::string reference_type;  // If reference_list, what type does it reference? (e.g., "technology", "structure")
    std::string description;
};

struct DefinitionSchema {
    std::string name;  // e.g., "structure", "unit", "item"
    std::vector<PropertySchema> properties;
    std::string description;
    
    // Quick lookup
    bool has_property(const std::string& prop_name) const {
        for (const auto& p : properties) {
            if (p.name == prop_name) return true;
        }
        return false;
    }
    
    std::optional<PropertySchema> get_property(const std::string& prop_name) const {
        for (const auto& p : properties) {
            if (p.name == prop_name) return p;
        }
        return std::nullopt;
    }
};

// Schema registry - holds all allowed definition types
class SchemaRegistry {
public:
    // Default instance for backward compatibility
    // Prefer passing SchemaRegistry* to components instead of using singleton
    static SchemaRegistry& instance() {
        static SchemaRegistry reg;
        return reg;
    }

    // Public constructor for dependency injection
    SchemaRegistry() = default;

    // Clear and reset
    void clear() {
        definitions_.clear();
        definition_names_.clear();
    }

    // Register a definition type
    void register_definition(const DefinitionSchema& def) {
        definitions_[def.name] = def;
        definition_names_.insert(def.name);
    }

    // Check if a definition type is allowed
    bool is_valid_definition(const std::string& def_name) const {
        return definition_names_.count(def_name) > 0;
    }

    // Get definition schema
    std::optional<DefinitionSchema> get_definition(const std::string& def_name) const {
        auto it = definitions_.find(def_name);
        if (it != definitions_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    // Get all definition names
    const std::set<std::string>& definition_names() const {
        return definition_names_;
    }

    // Load from CLI --types argument (comma-separated)
    // e.g., "era,structure,unit,technology,resource"
    void load_from_cli(const std::string& types_str);

    // Load Imperium default schema
    void load_imperium_default();

private:
    std::map<std::string, DefinitionSchema> definitions_;
    std::set<std::string> definition_names_;
};

} // namespace lex
