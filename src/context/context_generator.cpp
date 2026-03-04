#include "context.hpp"
#include "../ast/ast.h"
#include "lex/version.h"
#include <sstream>
#include <algorithm>
#include <set>

namespace lex {

// ============================================================================
// Helper Functions
// ============================================================================

namespace {

// Escape JSON string
std::string escape_json(const std::string& s) {
    std::string result;
    for (char c : s) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\t': result += "\\t"; break;
            case '\r': result += "\\r"; break;
            default: result += c;
        }
    }
    return result;
}

// Convert expression to string representation
std::string expression_to_string(const Expression& expr) {
    switch (expr.type) {
        case Expression::Type::INTEGER:
            return std::to_string(std::get<int64_t>(expr.value));
        case Expression::Type::FLOAT:
            return std::to_string(std::get<double>(expr.value));
        case Expression::Type::STRING:
            return std::get<std::string>(expr.value);
        case Expression::Type::BOOLEAN:
            return std::get<bool>(expr.value) ? "true" : "false";
        case Expression::Type::REFERENCE:
            return expr.reference;
        case Expression::Type::COLOR:
            return std::get<std::string>(expr.value);
        case Expression::Type::NULL_VAL:
            return "null";
        default:
            return "(expression)";
    }
}

// Extract property value as string
std::string property_to_string(const Property& prop) {
    if (!prop.value) return "";
    
    switch (prop.value->type) {
        case PropertyValue::Type::EXPRESSION:
            if (prop.value->expression) {
                return expression_to_string(*prop.value->expression);
            }
            break;
        case PropertyValue::Type::RESOURCE_MAP:
            if (prop.value->resource_map) {
                std::string result = "{";
                bool first = true;
                for (const auto& [name, val] : prop.value->resource_map->resources) {
                    if (!first) result += ", ";
                    result += name + ": " + std::to_string(val);
                    first = false;
                }
                result += "}";
                return result;
            }
            break;
        case PropertyValue::Type::REFERENCE_LIST:
            if (prop.value->reference_list) {
                std::string result = "[";
                for (size_t i = 0; i < prop.value->reference_list->references.size(); i++) {
                    if (i > 0) result += ", ";
                    result += prop.value->reference_list->references[i];
                }
                result += "]";
                return result;
            }
            break;
    }
    return "";
}

// Extract references from property
std::vector<std::string> extract_references(const Property& prop) {
    std::vector<std::string> refs;
    
    if (!prop.value) return refs;
    
    switch (prop.value->type) {
        case PropertyValue::Type::EXPRESSION:
            if (prop.value->expression && 
                prop.value->expression->type == Expression::Type::REFERENCE) {
                refs.push_back(prop.value->expression->reference);
            }
            break;
        case PropertyValue::Type::REFERENCE_LIST:
            if (prop.value->reference_list) {
                refs = prop.value->reference_list->references;
            }
            break;
        default:
            break;
    }
    
    return refs;
}

// Generate summary based on entity type and properties (template-based)
std::string generate_summary(const EntityContext& entity) {
    std::string type = entity.type;
    std::string era;
    std::string production;
    std::string cost;
    std::string unlocks;
    std::string requires_val;
    
    // Extract key properties
    for (const auto& [key, val] : entity.properties) {
        if (key == "era") era = val;
        else if (key == "production") production = val;
        else if (key == "cost") cost = val;
        else if (key == "unlocks") unlocks = val;
        else if (key == "requires") requires_val = val;
    }
    
    // Generate summary based on type
    if (type == "structure") {
        if (!era.empty() && !production.empty()) {
            return "A " + era + " era structure that produces " + production;
        } else if (!era.empty()) {
            return "A " + era + " era structure";
        }
        return "A " + type;
    }
    
    if (type == "technology" || type == "tech") {
        if (!era.empty() && !unlocks.empty()) {
            return era + " era technology that unlocks " + unlocks;
        } else if (!era.empty()) {
            return era + " era technology";
        }
        return "A technology";
    }
    
    if (type == "unit") {
        if (!era.empty()) {
            return "A " + era + " era unit";
        }
        return "A unit";
    }
    
    if (type == "era") {
        return "A game era";
    }
    
    if (type == "resource") {
        return "A game resource";
    }
    
    if (type == "building" || type == "building_type") {
        if (!era.empty()) {
            return "A " + era + " era building";
        }
        return "A building";
    }
    
    // Generic fallback
    std::string result = entity.id;
    if (!era.empty()) {
        result += " (" + era + " era)";
    }
    return result;
}

// Determine edge type from property name
std::string get_edge_type(const std::string& prop_name) {
    if (prop_name == "requires") return "requires";
    if (prop_name == "unlocks") return "unlocks";
    if (prop_name == "produces") return "produces";
    if (prop_name == "consumes") return "consumes";
    if (prop_name == "enables") return "enables";
    if (prop_name == "depends_on") return "depends_on";
    return "references";
}

} // anonymous namespace

// ============================================================================
// Context Generation
// ============================================================================

ContextResult generate_context(
    const std::vector<std::unique_ptr<Definition>>& definitions,
    const ContextOptions& options
) {
    ContextResult result;
    result.success = true;
    result.schema_version = LEX_VERSION;
    
    // Build entity contexts
    std::map<std::string, std::vector<std::string>> referenced_by_map;
    std::set<std::string> types_set;
    
    for (const auto& def : definitions) {
        EntityContext entity;
        entity.id = def->identifier;
        entity.type = def->definition_type;
        entity.source = def.get();
        
        types_set.insert(def->definition_type);
        
        // Extract properties
        for (const auto& prop : def->properties) {
            if (prop->value) {
                std::string value_str = property_to_string(*prop);
                if (!value_str.empty()) {
                    entity.properties[prop->name] = value_str;
                }
                
                // Extract references
                auto refs = extract_references(*prop);
                for (const auto& ref : refs) {
                    entity.references.push_back(ref);
                    referenced_by_map[ref].push_back(entity.id);
                    
                    // Add edge based on property name
                    if (options.include_graph) {
                        GraphEdge edge;
                        edge.from = entity.id;
                        edge.to = ref;
                        edge.type = get_edge_type(prop->name);
                        result.graph.edges.push_back(edge);
                    }
                    
                    // Track dependencies (for "requires" property)
                    if (prop->name == "requires" || prop->name == "depends_on") {
                        entity.dependencies.push_back(ref);
                    }
                }
            }
        }
        
        // Generate summary
        if (options.include_summaries) {
            entity.summary = generate_summary(entity);
        }
        
        result.entities.push_back(std::move(entity));
    }
    
    // Add reverse references
    if (options.include_reverse_refs) {
        for (auto& entity : result.entities) {
            if (referenced_by_map.count(entity.id)) {
                entity.referenced_by = referenced_by_map[entity.id];
                entity.dependents = referenced_by_map[entity.id];
            }
        }
    }
    
    // Build graph nodes
    if (options.include_graph) {
        for (const auto& entity : result.entities) {
            result.graph.nodes.push_back(entity.id);
        }
    }
    
    // Build statistics
    if (options.include_statistics) {
        result.statistics.total_entities = result.entities.size();
        
        for (const auto& entity : result.entities) {
            result.statistics.by_type[entity.type]++;
            
            // Group by configured properties
            for (const auto& prop_name : options.group_by_properties) {
                auto it = entity.properties.find(prop_name);
                if (it != entity.properties.end()) {
                    std::string key = prop_name + ":" + it->second;
                    result.statistics.by_property[key]++;
                }
            }
        }
    }
    
    // Store types
    for (const auto& t : types_set) {
        result.types.push_back(t);
    }
    
    return result;
}

ContextResult generate_context(
    const ASTFile& ast,
    const ContextOptions& options
) {
    return generate_context(ast.definitions, options);
}

// ============================================================================
// JSON Formatter
// ============================================================================

std::string format_context_json(const ContextResult& context, const ContextOptions& options) {
    std::ostringstream out;
    std::string indent = options.indent;
    
    out << "{\n";
    
    // Schema info
    out << indent << "\"schema\": {\n";
    out << indent << indent << "\"version\": \"" << escape_json(context.schema_version) << "\",\n";
    out << indent << indent << "\"types\": [";
    for (size_t i = 0; i < context.types.size(); i++) {
        if (i > 0) out << ", ";
        out << "\"" << escape_json(context.types[i]) << "\"";
    }
    out << "]\n";
    out << indent << "},\n";
    
    // Entities
    out << indent << "\"entities\": [\n";
    for (size_t i = 0; i < context.entities.size(); i++) {
        const auto& entity = context.entities[i];
        
        out << indent << indent << "{\n";
        out << indent << indent << indent << "\"id\": \"" << escape_json(entity.id) << "\",\n";
        out << indent << indent << indent << "\"type\": \"" << escape_json(entity.type) << "\",\n";
        
        if (options.include_summaries && !entity.summary.empty()) {
            out << indent << indent << indent << "\"summary\": \"" << escape_json(entity.summary) << "\",\n";
        }
        
        // Properties
        out << indent << indent << indent << "\"properties\": {\n";
        size_t prop_idx = 0;
        for (const auto& [key, val] : entity.properties) {
            out << indent << indent << indent << indent << "\"" << escape_json(key) << "\": \"" 
                << escape_json(val) << "\"";
            if (++prop_idx < entity.properties.size()) out << ",";
            out << "\n";
        }
        out << indent << indent << indent << "},\n";
        
        // References
        out << indent << indent << indent << "\"references\": [";
        for (size_t j = 0; j < entity.references.size(); j++) {
            if (j > 0) out << ", ";
            out << "\"" << escape_json(entity.references[j]) << "\"";
        }
        out << "],\n";
        
        // Referenced by
        if (options.include_reverse_refs) {
            out << indent << indent << indent << "\"referenced_by\": [";
            for (size_t j = 0; j < entity.referenced_by.size(); j++) {
                if (j > 0) out << ", ";
                out << "\"" << escape_json(entity.referenced_by[j]) << "\"";
            }
            out << "],\n";
        }
        
        // Dependencies
        out << indent << indent << indent << "\"dependencies\": [";
        for (size_t j = 0; j < entity.dependencies.size(); j++) {
            if (j > 0) out << ", ";
            out << "\"" << escape_json(entity.dependencies[j]) << "\"";
        }
        out << "],\n";
        
        // Dependents
        out << indent << indent << indent << "\"dependents\": [";
        for (size_t j = 0; j < entity.dependents.size(); j++) {
            if (j > 0) out << ", ";
            out << "\"" << escape_json(entity.dependents[j]) << "\"";
        }
        out << "]\n";
        
        out << indent << indent << "}";
        if (i < context.entities.size() - 1) out << ",";
        out << "\n";
    }
    out << indent << "]";
    
    // Graph
    if (options.include_graph && !context.graph.nodes.empty()) {
        out << ",\n";
        out << indent << "\"graph\": {\n";
        out << indent << indent << "\"nodes\": [";
        for (size_t i = 0; i < context.graph.nodes.size(); i++) {
            if (i > 0) out << ", ";
            out << "\"" << escape_json(context.graph.nodes[i]) << "\"";
        }
        out << "],\n";
        
        out << indent << indent << "\"edges\": [\n";
        for (size_t i = 0; i < context.graph.edges.size(); i++) {
            const auto& edge = context.graph.edges[i];
            out << indent << indent << indent << "{";
            out << "\"from\": \"" << escape_json(edge.from) << "\", ";
            out << "\"to\": \"" << escape_json(edge.to) << "\", ";
            out << "\"type\": \"" << escape_json(edge.type) << "\"";
            out << "}";
            if (i < context.graph.edges.size() - 1) out << ",";
            out << "\n";
        }
        out << indent << indent << "]\n";
        out << indent << "}";
    }
    
    // Statistics
    if (options.include_statistics) {
        out << ",\n";
        out << indent << "\"statistics\": {\n";
        out << indent << indent << "\"total_entities\": " << context.statistics.total_entities << ",\n";
        
        out << indent << indent << "\"by_type\": {";
        size_t type_idx = 0;
        for (const auto& [key, val] : context.statistics.by_type) {
            if (type_idx++ > 0) out << ", ";
            out << "\"" << escape_json(key) << "\": " << val;
        }
        out << "},\n";
        
        out << indent << indent << "\"by_property\": {";
        size_t prop_idx = 0;
        for (const auto& [key, val] : context.statistics.by_property) {
            if (prop_idx++ > 0) out << ", ";
            out << "\"" << escape_json(key) << "\": " << val;
        }
        out << "}\n";
        
        out << indent << "}";
    }
    
    out << "\n}\n";
    
    return out.str();
}

// ============================================================================
// Markdown Formatter
// ============================================================================

std::string format_context_markdown(const ContextResult& context, const ContextOptions& options) {
    std::ostringstream out;
    
    // Header
    out << "# Game Data Context\n\n";
    out << "**Generated by Lex Compiler v" << context.schema_version << "**\n\n";
    
    // Overview
    if (options.include_statistics) {
        out << "## Overview\n\n";
        out << "- **Total Entities**: " << context.statistics.total_entities << "\n";
        
        out << "- **Types**: ";
        size_t type_idx = 0;
        for (const auto& [key, val] : context.statistics.by_type) {
            if (type_idx++ > 0) out << ", ";
            out << key << " (" << val << ")";
        }
        out << "\n";
        
        if (!context.statistics.by_property.empty()) {
            for (const auto& [key, val] : context.statistics.by_property) {
                size_t colon = key.find(':');
                if (colon != std::string::npos) {
                    std::string prop = key.substr(0, colon);
                    std::string value = key.substr(colon + 1);
                    out << "- **" << prop << " " << value << "**: " << val << " entities\n";
                }
            }
        }
        out << "\n";
    }
    
    // Group entities by type
    std::map<std::string, std::vector<const EntityContext*>> by_type;
    for (const auto& entity : context.entities) {
        by_type[entity.type].push_back(&entity);
    }
    
    // Entities by type
    for (const auto& [type, entities] : by_type) {
        // Capitalize first letter
        std::string section_title = type;
        if (!section_title.empty()) {
            section_title[0] = std::toupper(section_title[0]);
        }
        out << "## " << section_title << "s\n\n";
        
        for (const auto* entity : entities) {
            out << "### " << entity->id << "\n";
            
            if (options.include_summaries && !entity->summary.empty()) {
                out << "> " << entity->summary << "\n\n";
            }
            
            // Properties table
            if (!entity->properties.empty()) {
                out << "| Property | Value |\n";
                out << "|----------|-------|\n";
                for (const auto& [key, val] : entity->properties) {
                    out << "| " << key << " | " << val << " |\n";
                }
                out << "\n";
            }
            
            // Dependencies
            if (!entity->dependencies.empty()) {
                out << "**Dependencies**: ";
                for (size_t i = 0; i < entity->dependencies.size(); i++) {
                    if (i > 0) out << ", ";
                    out << entity->dependencies[i];
                }
                out << "\n\n";
            }
            
            // Dependents (what needs this)
            if (!entity->dependents.empty()) {
                out << "**Unlocks/Enables**: ";
                for (size_t i = 0; i < entity->dependents.size(); i++) {
                    if (i > 0) out << ", ";
                    out << entity->dependents[i];
                }
                out << "\n\n";
            }
            
            out << "---\n\n";
        }
    }
    
    // Dependency Graph
    if (options.include_graph && !context.graph.edges.empty()) {
        out << "## Dependency Graph\n\n";
        out << "```\n";
        
        // Simple text-based graph
        std::map<std::string, std::vector<std::string>> deps;
        for (const auto& edge : context.graph.edges) {
            if (edge.type == "requires" || edge.type == "unlocks") {
                deps[edge.from].push_back(edge.to);
            }
        }
        
        for (const auto& [from, to_list] : deps) {
            out << from << " -> ";
            for (size_t i = 0; i < to_list.size(); i++) {
                if (i > 0) out << ", ";
                out << to_list[i];
            }
            out << "\n";
        }
        
        out << "```\n\n";
    }
    
    return out.str();
}

// ============================================================================
// Minimal Formatter (for LLM prompts)
// ============================================================================

std::string format_context_minimal(const ContextResult& context, const ContextOptions& options) {
    std::ostringstream out;
    
    // Group entities by type
    std::map<std::string, std::vector<const EntityContext*>> by_type;
    for (const auto& entity : context.entities) {
        by_type[entity.type].push_back(&entity);
    }
    
    // Output each type group
    for (const auto& [type, entities] : by_type) {
        std::string type_upper = type;
        for (auto& c : type_upper) c = std::toupper(c);
        out << type_upper << "S:\n";
        
        for (const auto* entity : entities) {
            out << entity->id << ": ";
            
            // Key properties inline
            std::vector<std::string> parts;
            for (const auto& [key, val] : entity->properties) {
                parts.push_back(key + " " + val);
            }
            
            for (size_t i = 0; i < parts.size(); i++) {
                if (i > 0) out << ", ";
                out << parts[i];
            }
            out << "\n";
        }
        out << "\n";
    }
    
    // Dependencies summary
    if (options.include_graph && !context.graph.edges.empty()) {
        out << "DEPENDENCIES:\n";
        
        std::map<std::string, std::vector<std::pair<std::string, std::string>>> deps;
        for (const auto& edge : context.graph.edges) {
            deps[edge.from].push_back({edge.to, edge.type});
        }
        
        for (const auto& [from, to_list] : deps) {
            out << from << " ";
            for (size_t i = 0; i < to_list.size(); i++) {
                if (i > 0) out << ", ";
                if (to_list[i].second == "requires") {
                    out << "requires " << to_list[i].first;
                } else if (to_list[i].second == "unlocks") {
                    out << "unlocks " << to_list[i].first;
                } else {
                    out << to_list[i].second << " " << to_list[i].first;
                }
            }
            out << "\n";
        }
    }
    
    return out.str();
}

// ============================================================================
// Utility Functions
// ============================================================================

std::string context_extension(ContextFormat format) {
    switch (format) {
        case ContextFormat::JSON: return ".context.json";
        case ContextFormat::Markdown: return ".context.md";
        case ContextFormat::Minimal: return ".context.txt";
        default: return ".context";
    }
}

} // namespace lex
