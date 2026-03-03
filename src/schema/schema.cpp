#include "schema.h"
#include <sstream>
#include <algorithm>

namespace lex {

// ============================================================================
// Schema Registry Implementation
// ============================================================================

void SchemaRegistry::load_from_cli(const std::string& types_str) {
    clear();
    
    // Parse comma-separated types
    std::stringstream ss(types_str);
    std::string type;
    
    while (std::getline(ss, type, ',')) {
        // Trim whitespace
        type.erase(0, type.find_first_not_of(" \t"));
        type.erase(type.find_last_not_of(" \t") + 1);
        
        if (type.empty()) continue;
        
        // Register as a minimal definition schema
        DefinitionSchema def;
        def.name = type;
        def.description = "User-defined definition type: " + type;
        
        // No property validation for CLI-defined types (flexible)
        register_definition(def);
    }
}

void SchemaRegistry::load_imperium_default() {
    clear();
    
    // Era definition
    {
        DefinitionSchema def;
        def.name = "era";
        def.description = "Historical era in the game";
        def.properties = {
            {"name", true, "string", "Display name"},
            {"period", false, "string", "Historical period"},
            {"music", false, "string", "Background music track"},
            {"dominant_color", false, "string", "UI theme color"},
            {"atmosphere", false, "string", "Atmospheric description"}
        };
        register_definition(def);
    }
    
    // Structure definition
    {
        DefinitionSchema def;
        def.name = "structure";
        def.description = "Buildable structure";
        def.properties = {
            {"era", true, "string", "Required era"},
            {"cost", true, "resource_map", "Resource cost"},
            {"production", false, "resource_map", "Resources produced per turn"},
            {"maintenance", false, "resource_map", "Upkeep cost per turn"},
            {"requires", false, "reference_list", "Prerequisite technologies"},
            {"unlocks", false, "reference_list", "Technologies unlocked"},
            {"description", false, "string", "In-game description"},
            {"icon", false, "string", "UI icon path"},
            {"tooltip", false, "string", "Hover tooltip text"}
        };
        register_definition(def);
    }
    
    // Unit definition
    {
        DefinitionSchema def;
        def.name = "unit";
        def.description = "Military or civilian unit";
        def.properties = {
            {"era", true, "string", "Required era"},
            {"type", false, "string", "Unit type (military, civilian, etc.)"},
            {"cost", true, "resource_map", "Resource cost"},
            {"maintenance", false, "resource_map", "Upkeep cost per turn"},
            {"attack", false, "int", "Attack strength"},
            {"defense", false, "int", "Defense rating"},
            {"movement", false, "int", "Movement points"},
            {"range", false, "int", "Attack range"},
            {"requires", false, "reference_list", "Prerequisites"},
            {"description", false, "string", "Unit description"}
        };
        register_definition(def);
    }
    
    // Technology definition
    {
        DefinitionSchema def;
        def.name = "technology";
        def.description = "Researchable technology";
        def.properties = {
            {"era", true, "string", "Required era"},
            {"research_cost", true, "int", "Research points needed"},
            {"prerequisites", false, "reference_list", "Required technologies"},
            {"unlocks", false, "reference_list", "What this unlocks"},
            {"description", false, "string", "Technology description"},
            {"quote", false, "string", "Flavor quote"}
        };
        register_definition(def);
    }
    
    // Resource definition
    {
        DefinitionSchema def;
        def.name = "resource";
        def.description = "Game resource type";
        def.properties = {
            {"type", false, "string", "Resource category"},
            {"icon", false, "string", "UI icon path"},
            {"color", false, "string", "Display color"},
            {"description", false, "string", "Resource description"}
        };
        register_definition(def);
    }
    
    // Choice definition (narrative)
    {
        DefinitionSchema def;
        def.name = "choice";
        def.description = "Player choice in narrative events";
        def.properties = {
            {"era", false, "string", "When this choice appears"},
            {"text", true, "string", "Choice text shown to player"},
            {"options", false, "reference_list", "Available options"}
        };
        register_definition(def);
    }
    
    // Ending definition (narrative)
    {
        DefinitionSchema def;
        def.name = "ending";
        def.description = "Game ending condition";
        def.properties = {
            {"type", true, "string", "Ending type (victory, defeat, etc.)"},
            {"text", false, "string", "Ending narrative text"},
            {"requirements", false, "string", "Conditions for this ending"}
        };
        register_definition(def);
    }
    
    // Event definition
    {
        DefinitionSchema def;
        def.name = "event";
        def.description = "Random or triggered event";
        def.properties = {
            {"era", false, "string", "When this event can occur"},
            {"trigger", false, "string", "Event trigger condition"},
            {"text", false, "string", "Event description"},
            {"effect", false, "string", "Event effect"},
            {"probability", false, "float", "Base probability"}
        };
        register_definition(def);
    }
    
    // Secret definition
    {
        DefinitionSchema def;
        def.name = "secret";
        def.description = "Hidden content discoverable by player";
        def.properties = {
            {"trigger", false, "string", "Discovery condition"},
            {"activate", false, "string", "What happens when discovered"},
            {"archivist", false, "string", "Archivist response text"}
        };
        register_definition(def);
    }
    
    // Terrain definition (for 4X/strategy games)
    {
        DefinitionSchema def;
        def.name = "terrain";
        def.description = "Map terrain type";
        def.properties = {
            {"name", true, "string", "Display name"},
            {"movement_cost", false, "int", "Movement points to traverse"},
            {"defense_bonus", false, "int", "Combat defense bonus"},
            {"attack_bonus", false, "int", "Combat attack bonus"},
            {"arcane_bonus", false, "bool", "Arcane/magic bonus"},
            {"buildable", false, "bool", "Can structures be built here"},
            {"description", false, "string", "Terrain description"},
            {"color", false, "string", "Display color (hex)"},
            {"icon", false, "string", "UI icon path"}
        };
        register_definition(def);
    }
}

} // namespace lex
