#include "json_backend.h"
#include <sstream>

namespace lex {

std::string JsonBackend::generate(const std::vector<std::unique_ptr<Definition>>& ast) {
    std::stringstream output;
    
    output << "{\n";
    output << "  \"version\": \"1.0\",\n";
    output << "  \"generated_by\": \"Lex Compiler v0.1.0\",\n";
    
    // Group definitions by type
    std::vector<const Definition*> eras;
    std::vector<const Definition*> structures;
    std::vector<const Definition*> units;
    std::vector<const Definition*> technologies;
    std::vector<const Definition*> resources;
    std::vector<const Definition*> others;
    
    for (const auto& def : ast) {
        if (def->definition_type == "era") eras.push_back(def.get());
        else if (def->definition_type == "structure") structures.push_back(def.get());
        else if (def->definition_type == "unit") units.push_back(def.get());
        else if (def->definition_type == "technology") technologies.push_back(def.get());
        else if (def->definition_type == "resource") resources.push_back(def.get());
        else others.push_back(def.get());
    }
    
    bool needComma = false;
    
    // Eras
    if (!eras.empty()) {
        output << "  \"eras\": {\n";
        for (size_t i = 0; i < eras.size(); i++) {
            output << generate_era(*eras[i]);
            if (i < eras.size() - 1) output << ",\n";
            else output << "\n";
        }
        output << "  }";
        needComma = true;
    }
    
    // Structures
    if (!structures.empty()) {
        if (needComma) output << ",\n";
        output << "  \"structures\": {\n";
        for (size_t i = 0; i < structures.size(); i++) {
            output << generate_structure(*structures[i]);
            if (i < structures.size() - 1) output << ",\n";
            else output << "\n";
        }
        output << "  }";
        needComma = true;
    }
    
    // Units
    if (!units.empty()) {
        if (needComma) output << ",\n";
        output << "  \"units\": {\n";
        for (size_t i = 0; i < units.size(); i++) {
            output << generate_unit(*units[i]);
            if (i < units.size() - 1) output << ",\n";
            else output << "\n";
        }
        output << "  }";
        needComma = true;
    }
    
    // Technologies
    if (!technologies.empty()) {
        if (needComma) output << ",\n";
        output << "  \"technologies\": {\n";
        for (size_t i = 0; i < technologies.size(); i++) {
            output << generate_technology(*technologies[i]);
            if (i < technologies.size() - 1) output << ",\n";
            else output << "\n";
        }
        output << "  }";
        needComma = true;
    }
    
    output << "\n}\n";
    
    return output.str();
}

std::string JsonBackend::escape_string(const std::string& s) {
    std::string result;
    for (char c : s) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\t': result += "\\t"; break;
            default: result += c;
        }
    }
    return result;
}

std::string JsonBackend::generate_era(const Definition& era) {
    std::stringstream json;
    json << "    \"" << era.identifier << "\": {\n";
    
    // Collect all properties first
    std::vector<std::string> props;
    
    props.push_back("\"id\": \"" + era.identifier + "\"");
    
    std::string name = get_string_property(era, "name");
    if (!name.empty()) {
        props.push_back("\"name\": \"" + escape_string(name) + "\"");
    }
    
    std::string period = get_string_property(era, "period");
    if (!period.empty()) {
        props.push_back("\"period\": \"" + escape_string(period) + "\"");
    }
    
    std::string description = get_string_property(era, "description");
    if (!description.empty()) {
        props.push_back("\"description\": \"" + escape_string(description) + "\"");
    }
    
    std::string atmosphere = get_string_property(era, "atmosphere");
    if (!atmosphere.empty()) {
        props.push_back("\"atmosphere\": \"" + escape_string(atmosphere) + "\"");
    }
    
    std::string music = get_string_property(era, "music");
    if (!music.empty()) {
        props.push_back("\"music\": \"" + escape_string(music) + "\"");
    }
    
    std::string color = get_string_property(era, "dominant_color");
    if (!color.empty()) {
        props.push_back("\"dominant_color\": \"" + color + "\"");
    }
    
    // Output with proper comma handling
    for (size_t i = 0; i < props.size(); i++) {
        json << "      " << props[i];
        if (i < props.size() - 1) json << ",";
        json << "\n";
    }
    
    json << "    }";
    return json.str();
}

std::string JsonBackend::generate_structure(const Definition& structure) {
    std::stringstream json;
    json << "    \"" << structure.identifier << "\": {\n";
    
    std::vector<std::string> props;
    props.push_back("\"id\": \"" + structure.identifier + "\"");
    
    std::string era = get_string_property(structure, "era");
    if (!era.empty()) {
        props.push_back("\"era\": \"" + era + "\"");
    }
    
    std::string name = get_string_property(structure, "name");
    if (!name.empty()) {
        props.push_back("\"name\": \"" + escape_string(name) + "\"");
    }
    
    const ResourceMap* cost = get_resource_map_property(structure, "cost");
    if (cost) {
        props.push_back("\"cost\": " + generate_resource_map(*cost));
    }
    
    const ResourceMap* production = get_resource_map_property(structure, "production");
    if (production) {
        props.push_back("\"production\": " + generate_resource_map(*production));
    }
    
    const ResourceMap* maintenance = get_resource_map_property(structure, "maintenance");
    if (maintenance) {
        props.push_back("\"maintenance\": " + generate_resource_map(*maintenance));
    }
    
    const ReferenceList* prereqs = get_reference_list_property(structure, "prerequisites");
    if (prereqs) {
        props.push_back("\"prerequisites\": " + generate_reference_list(*prereqs));
    }
    
    std::string description = get_string_property(structure, "description");
    if (!description.empty()) {
        props.push_back("\"description\": \"" + escape_string(description) + "\"");
    }
    
    for (size_t i = 0; i < props.size(); i++) {
        json << "      " << props[i];
        if (i < props.size() - 1) json << ",";
        json << "\n";
    }
    
    json << "    }";
    return json.str();
}

std::string JsonBackend::generate_unit(const Definition& unit) {
    std::stringstream json;
    json << "    \"" << unit.identifier << "\": {\n";
    json << "      \"id\": \"" << unit.identifier << "\",\n";
    
    std::string era = get_string_property(unit, "era");
    if (!era.empty()) {
        json << "      \"era\": \"" << era << "\",\n";
    }
    
    std::string name = get_string_property(unit, "name");
    if (!name.empty()) {
        json << "      \"name\": \"" << escape_string(name) << "\",\n";
    }
    
    std::string type = get_string_property(unit, "type");
    if (!type.empty()) {
        json << "      \"type\": \"" << type << "\",\n";
    }
    
    int64_t attack = get_int_property(unit, "attack", -1);
    if (attack >= 0) {
        json << "      \"attack\": " << attack << ",\n";
    }
    
    int64_t defense = get_int_property(unit, "defense", -1);
    if (defense >= 0) {
        json << "      \"defense\": " << defense << ",\n";
    }
    
    int64_t movement = get_int_property(unit, "movement", -1);
    if (movement >= 0) {
        json << "      \"movement\": " << movement << ",\n";
    }
    
    const ResourceMap* cost = get_resource_map_property(unit, "cost");
    if (cost) {
        json << "      \"cost\": " << generate_resource_map(*cost) << ",\n";
    }
    
    const ResourceMap* maintenance = get_resource_map_property(unit, "maintenance");
    if (maintenance) {
        json << "      \"maintenance\": " << generate_resource_map(*maintenance) << "\n";
    }
    
    json << "    }";
    return json.str();
}

std::string JsonBackend::generate_technology(const Definition& tech) {
    std::stringstream json;
    json << "    \"" << tech.identifier << "\": {\n";
    json << "      \"id\": \"" << tech.identifier << "\",\n";
    
    std::string era = get_string_property(tech, "era");
    if (!era.empty()) {
        json << "      \"era\": \"" << era << "\",\n";
    }
    
    std::string name = get_string_property(tech, "name");
    if (!name.empty()) {
        json << "      \"name\": \"" << escape_string(name) << "\",\n";
    }
    
    int64_t research_cost = get_int_property(tech, "research_cost", 0);
    json << "      \"research_cost\": " << research_cost << ",\n";
    
    const ReferenceList* prereqs = get_reference_list_property(tech, "prerequisites");
    if (prereqs) {
        json << "      \"prerequisites\": " << generate_reference_list(*prereqs) << ",\n";
    }
    
    std::string description = get_string_property(tech, "description");
    if (!description.empty()) {
        json << "      \"description\": \"" << escape_string(description) << "\"\n";
    }
    
    json << "    }";
    return json.str();
}

std::string JsonBackend::generate_resource_map(const ResourceMap& map) {
    std::string json = "{";
    bool first = true;
    for (const auto& [name, value] : map.resources) {
        if (!first) json += ", ";
        json += "\"" + name + "\": " + std::to_string(value);
        first = false;
    }
    json += "}";
    return json;
}

std::string JsonBackend::generate_reference_list(const ReferenceList& list) {
    std::string json = "[";
    for (size_t i = 0; i < list.references.size(); i++) {
        if (i > 0) json += ", ";
        json += "\"" + list.references[i] + "\"";
    }
    json += "]";
    return json;
}

std::string JsonBackend::get_string_property(const Definition& def, const std::string& name) {
    for (const auto& prop : def.properties) {
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
    return "";
}

int64_t JsonBackend::get_int_property(const Definition& def, const std::string& name, int64_t default_val) {
    for (const auto& prop : def.properties) {
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
    return default_val;
}

const ResourceMap* JsonBackend::get_resource_map_property(const Definition& def, const std::string& name) {
    for (const auto& prop : def.properties) {
        if (prop->name == name && prop->value) {
            if (prop->value->type == PropertyValue::Type::RESOURCE_MAP) {
                return prop->value->resource_map.get();
            }
        }
    }
    return nullptr;
}

const ReferenceList* JsonBackend::get_reference_list_property(const Definition& def, const std::string& name) {
    for (const auto& prop : def.properties) {
        if (prop->name == name && prop->value) {
            if (prop->value->type == PropertyValue::Type::REFERENCE_LIST) {
                return prop->value->reference_list.get();
            }
        }
    }
    return nullptr;
}

} // namespace lex
