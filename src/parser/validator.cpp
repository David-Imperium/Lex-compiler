#include "validator.h"
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
    eras_.clear();
    structures_.clear();
    units_.clear();
    technologies_.clear();
    resources_.clear();
    all_symbols_.clear();

    // Phase 1: Register all symbols
    register_symbols(definitions);

    // Phase 2: Validate references
    validate_references(definitions);

    // Phase 3: Validate required properties
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
        
        // Register by type
        if (def->definition_type == "era") {
            eras_.insert(symbol);
        } else if (def->definition_type == "structure") {
            structures_.insert(symbol);
        } else if (def->definition_type == "unit") {
            units_.insert(symbol);
        } else if (def->definition_type == "technology") {
            technologies_.insert(symbol);
        } else if (def->definition_type == "resource") {
            resources_.insert(symbol);
        }
    }
}

void Validator::validate_references(const std::vector<std::unique_ptr<Definition>>& definitions) {
    for (const auto& def : definitions) {
        for (const auto& prop : def->properties) {
            if (!prop->value) continue;
            
            // Check era reference
            if (prop->name == "era" && prop->value->expression) {
                auto& expr = prop->value->expression;
                if (expr->type == Expression::Type::REFERENCE) {
                    if (eras_.find(expr->reference) == eras_.end()) {
                        add_error(
                            "UNDEFINED_ERA",
                            "Structure '" + def->identifier + "' references undefined era: " + expr->reference,
                            def->location
                        );
                    }
                }
            }
            
            // Check reference lists
            if (prop->value->type == PropertyValue::Type::REFERENCE_LIST) {
                for (const auto& ref : prop->value->reference_list->references) {
                    if (ref.empty()) continue;
                    
                    // Check based on property name
                    if (prop->name == "technologies" || prop->name == "prerequisites") {
                        if (technologies_.find(ref) == technologies_.end()) {
                            add_warning(
                                "UNDEFINED_TECHNOLOGY",
                                "'" + def->identifier + "' references undefined technology: " + ref + " (forward reference allowed)",
                                def->location
                            );
                        }
                    } else if (prop->name == "structures") {
                        if (structures_.find(ref) == structures_.end()) {
                            add_warning(
                                "UNDEFINED_STRUCTURE",
                                "'" + def->identifier + "' references undefined structure: " + ref,
                                def->location
                            );
                        }
                    } else if (prop->name == "units") {
                        if (units_.find(ref) == units_.end()) {
                            add_warning(
                                "UNDEFINED_UNIT",
                                "'" + def->identifier + "' references undefined unit: " + ref,
                                def->location
                            );
                        }
                    }
                }
            }
        }
    }
}

void Validator::validate_required_properties(const std::vector<std::unique_ptr<Definition>>& definitions) {
    for (const auto& def : definitions) {
        // Collect property names
        std::set<std::string> prop_names;
        for (const auto& prop : def->properties) {
            prop_names.insert(prop->name);
        }

        // Validate based on definition type
        if (def->definition_type == "structure") {
            if (prop_names.find("era") == prop_names.end()) {
                add_error(
                    "MISSING_REQUIRED_PROPERTY",
                    "Structure '" + def->identifier + "' is missing required property: era",
                    def->location
                );
            }
        } else if (def->definition_type == "unit") {
            if (prop_names.find("era") == prop_names.end()) {
                add_error(
                    "MISSING_REQUIRED_PROPERTY",
                    "Unit '" + def->identifier + "' is missing required property: era",
                    def->location
                );
            }
        } else if (def->definition_type == "technology") {
            if (prop_names.find("research_cost") == prop_names.end()) {
                add_error(
                    "MISSING_REQUIRED_PROPERTY",
                    "Technology '" + def->identifier + "' is missing required property: research_cost",
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
