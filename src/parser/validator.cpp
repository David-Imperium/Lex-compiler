#include "validator.h"
#include "../schema/schema.h"
#include <sstream>

namespace lex {

void Validator::add_error(const std::string& code, const std::string& message, const std::string& location) {
    SemanticError err;
    err.code = code;
    err.message = message;
    err.location = location;
    err.severity = ErrorSeverity::ERROR;
    errors_.push_back(err);
}

void Validator::add_warning(const std::string& code, const std::string& message, const std::string& location) {
    SemanticError warn;
    warn.code = code;
    warn.message = message;
    warn.location = location;
    warn.severity = ErrorSeverity::WARNING;
    warnings_.push_back(warn);
}

bool Validator::validate(const std::vector<std::unique_ptr<Definition>>& definitions) {
    errors_.clear();
    warnings_.clear();
    symbols_by_type_.clear();
    all_symbols_.clear();

    // Phase 1: Register all symbols by type
    register_symbols(definitions);

    // Phase 2: Validate references
    validate_references(definitions);

    // Phase 3: Validate required properties (schema-driven)
    validate_required_properties(definitions);

    // Phase 4: Validate resource maps
    validate_resource_maps(definitions);

    return !has_errors();
}

void Validator::register_symbols(const std::vector<std::unique_ptr<Definition>>& definitions) {
    for (const auto& def : definitions) {
        std::string symbol = def->identifier;

        // Check for duplicate definitions (any type)
        if (all_symbols_.find(symbol) != all_symbols_.end()) {
            add_error(
                "DUPLICATE_DEFINITION",
                "Duplicate definition: '" + symbol + "' is already defined",
                def->location
            );
            continue;
        }

        all_symbols_.insert(symbol);

        // Register by type (dynamic - works for any definition type)
        symbols_by_type_[def->definition_type].insert(symbol);
    }
}

void Validator::validate_references(const std::vector<std::unique_ptr<Definition>>& definitions) {
    auto& schema = SchemaRegistry::instance();

    for (const auto& def : definitions) {
        // Get schema for this definition type
        auto def_schema = schema.get_definition(def->definition_type);

        for (const auto& prop : def->properties) {
            if (!prop->value) continue;

            // Get property schema to check reference_type
            std::string reference_type;
            if (def_schema) {
                auto prop_schema = def_schema->get_property(prop->name);
                if (prop_schema) {
                    reference_type = prop_schema->reference_type;
                }
            }

            // Fallback to heuristic for legacy compatibility
            if (reference_type.empty()) {
                if (prop->name == "era") reference_type = "era";
                else if (prop->name == "technologies" || prop->name == "prerequisites") reference_type = "technology";
                else if (prop->name == "structures") reference_type = "structure";
                else if (prop->name == "units") reference_type = "unit";
            }

            // Validate single reference (e.g., era: Ancient)
            if (prop->value->expression && prop->value->expression->type == Expression::Type::REFERENCE) {
                if (!reference_type.empty()) {
                    auto& refs = symbols_by_type_[reference_type];
                    if (refs.find(prop->value->expression->reference) == refs.end()) {
                        add_error(
                            "UNDEFINED_REFERENCE",
                            "'" + def->identifier + "' references undefined " + reference_type + ": " + prop->value->expression->reference,
                            def->location
                        );
                    }
                }
            }

            // Validate reference lists
            if (prop->value->type == PropertyValue::Type::REFERENCE_LIST && !reference_type.empty()) {
                for (const auto& ref : prop->value->reference_list->references) {
                    if (ref.empty()) continue;

                    auto& refs = symbols_by_type_[reference_type];
                    if (refs.find(ref) == refs.end()) {
                        add_warning(
                            "UNDEFINED_REFERENCE",
                            "'" + def->identifier + "' references undefined " + reference_type + ": " + ref,
                            def->location
                        );
                    }
                }
            }
        }
    }
}

void Validator::validate_required_properties(const std::vector<std::unique_ptr<Definition>>& definitions) {
    auto& schema = SchemaRegistry::instance();

    for (const auto& def : definitions) {
        // Get schema for this definition type
        auto def_schema = schema.get_definition(def->definition_type);

        // If no schema, skip validation (custom types have no required props)
        if (!def_schema) continue;

        // Collect property names
        std::set<std::string> prop_names;
        for (const auto& prop : def->properties) {
            prop_names.insert(prop->name);
        }

        // Check required properties from schema
        for (const auto& prop_schema : def_schema->properties) {
            if (prop_schema.required && prop_names.find(prop_schema.name) == prop_names.end()) {
                add_error(
                    "MISSING_REQUIRED_PROPERTY",
                    def->definition_type + " '" + def->identifier + "' is missing required property: " + prop_schema.name,
                    def->location
                );
            }
        }
    }
}

void Validator::validate_resource_maps(const std::vector<std::unique_ptr<Definition>>& definitions) {
    for (const auto& def : definitions) {
        for (const auto& prop : def->properties) {
            if (!prop->value || prop->value->type != PropertyValue::Type::RESOURCE_MAP) continue;

            auto* map = prop->value->resource_map.get();
            if (!map) continue;

            // Check for negative values in cost/production/maintenance
            if (prop->name == "cost" || prop->name == "production" || prop->name == "maintenance") {
                for (const auto& [resource, amount] : map->resources) {
                    if (amount < 0) {
                        add_warning(
                            "NEGATIVE_RESOURCE",
                            "'" + def->identifier + "' has negative " + prop->name + " for " + resource + ": " + std::to_string(amount),
                            def->location
                        );
                    }
                }
            }
        }
    }
}

} // namespace lex
