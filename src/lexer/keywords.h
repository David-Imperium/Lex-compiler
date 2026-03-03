#pragma once

#include <map>
#include <string>
#include "token.h"

namespace lex {

// Keywords map: English keywords only (for AI agents and international community)
inline const std::map<std::string, TokenType> KEYWORDS = {
    // Core Constructs
    {"structure", TokenType::STRUCTURE},
    {"unit", TokenType::UNIT},
    {"technology", TokenType::TECHNOLOGY},
    {"resource", TokenType::RESOURCE},
    {"era", TokenType::ERA},
    {"terrain", TokenType::TERRAIN},

    // Narrative
    {"choice", TokenType::CHOICE},
    {"ending", TokenType::ENDING},
    {"event", TokenType::EVENT},
    {"secret", TokenType::SECRET},

    // Companion AI
    {"archivist", TokenType::ARCHIVIST},
    {"lore", TokenType::LORE},

    // Properties - Identification
    {"name", TokenType::NAME},
    {"id", TokenType::ID},
    {"description", TokenType::DESCRIPTION},
    {"quote", TokenType::QUOTE},

    // Properties - Economy
    {"cost", TokenType::COST},
    {"production", TokenType::PRODUCTION},
    {"maintenance", TokenType::MAINTENANCE},
    {"consumption", TokenType::CONSUMPTION},

    // Properties - Requirements/Unlock
    {"requires", TokenType::REQUIRES},
    {"unlocks", TokenType::UNLOCKS},
    {"locks", TokenType::LOCKS},

    // Properties - State
    {"type", TokenType::TYPE},
    {"category", TokenType::CATEGORY},
    {"level", TokenType::LEVEL},

    // Conditional Constructs
    {"when", TokenType::WHEN},
    {"if", TokenType::IF},
    {"available_if", TokenType::AVAILABLE_IF},
    {"secret_if", TokenType::SECRET_IF},
    {"active_if", TokenType::ACTIVE_IF},
    {"bonus_if", TokenType::BONUS_IF},

    // Trigger
    {"trigger", TokenType::TRIGGER},
    {"condition", TokenType::CONDITION},
    {"probability", TokenType::PROBABILITY},

    // Logical Operators
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"not", TokenType::NOT},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"null", TokenType::NULL_LIT},

    // Actions - Activation
    {"activate", TokenType::ACTIVATE},
    {"deactivate", TokenType::DEACTIVATE},
    {"enable", TokenType::ENABLE},
    {"disable", TokenType::DISABLE},

    // Actions - Flags
    {"flag", TokenType::FLAG},
    {"mark", TokenType::MARK},

    // Actions - Modifiers
    {"effect", TokenType::EFFECT},
    {"bonus", TokenType::BONUS},
    {"malus", TokenType::MALUS},

    // UI Properties
    {"icon", TokenType::ICON},
    {"color", TokenType::COLOR},
    {"tooltip", TokenType::TOOLTIP},
    {"panel", TokenType::PANEL},
    {"components", TokenType::COMPONENTS},

    // Lore Properties
    {"context", TokenType::CONTEXT},
    {"references", TokenType::REFERENCES},
    {"atmosphere", TokenType::ATMOSPHERE},
    {"text", TokenType::TEXT_CONTENT},

    // Unit Stats
    {"attack", TokenType::ATTACK},
    {"defense", TokenType::DEFENSE},
    {"movement", TokenType::MOVEMENT},
    {"range", TokenType::RANGE},

    // Population Stats
    {"population", TokenType::POPULATION},
    {"health", TokenType::HEALTH},
    {"happiness", TokenType::HAPPINESS},
    {"stability", TokenType::STABILITY},

    // Technology
    {"research_cost", TokenType::RESEARCH_COST},
    {"prerequisites", TokenType::PREREQUISITES},
    {"research_time", TokenType::RESEARCH_TIME},

    // Era Properties
    {"period", TokenType::PERIOD},
    {"music", TokenType::MUSIC},
    {"dominant_color", TokenType::DOMINANT_COLOR},
    {"transition", TokenType::TRANSITION}
};

} // namespace lex
