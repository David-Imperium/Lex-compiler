#pragma once

#include <string>
#include <variant>
#include <cstdint>

namespace lex {

enum class TokenType {
    // Literals
    INTEGER,
    FLOAT,
    STRING,
    HEX_COLOR,
    TRUE,
    FALSE,
    NULL_LIT,

    // Identifiers
    IDENTIFIER,

    // Core Constructs
    ERA,
    STRUCTURE,
    UNIT,
    TECHNOLOGY,
    RESOURCE,
    CHOICE,
    ENDING,
    EVENT,
    SECRET,
    TERRAIN,

    // Module System
    USE,
    MODULE,
    PUBLIC,
    INTERNAL,
    PRIVATE,

    // Conditional Keywords
    WHEN,
    IF,
    AVAILABLE_IF,
    SECRET_IF,
    ACTIVE_IF,
    BONUS_IF,

    // Property Keywords
    NAME,
    ID,
    DESCRIPTION,
    COST,
    PRODUCTION,
    MAINTENANCE,
    CONSUMPTION,
    REQUIRES,
    UNLOCKS,
    LOCKS,
    TYPE,
    CATEGORY,
    LEVEL,
    QUOTE,

    // Trigger/Condition Keywords
    TRIGGER,
    CONDITION,
    PROBABILITY,

    // Action Keywords
    ACTIVATE,
    DEACTIVATE,
    ENABLE,
    DISABLE,
    FLAG,
    MARK,
    EFFECT,
    BONUS,
    MALUS,

    // UI Keywords
    ICON,
    COLOR,
    TOOLTIP,
    PANEL,
    COMPONENTS,

    // Lore Keywords
    CONTEXT,
    REFERENCES,
    ATMOSPHERE,
    TEXT_CONTENT,

    // Companion AI
    ARCHIVIST,
    LORE,

    // Unit Stats
    ATTACK,
    DEFENSE,
    MOVEMENT,
    // RANGE already defined in Special section

    // Population Stats
    POPULATION,
    HEALTH,
    HAPPINESS,
    STABILITY,

    // Technology Keywords
    RESEARCH_COST,
    PREREQUISITES,
    RESEARCH_TIME,

    // Era Keywords
    PERIOD,
    MUSIC,
    DOMINANT_COLOR,
    TRANSITION,

    // Logical Operators
    AND,
    OR,
    NOT,

    // Comparison Operators
    EQ,         // ==
    NE,         // !=
    GT,         // >
    LT,         // <
    GE,         // >=
    LE,         // <=

    // Arithmetic Operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,

    // Assignment Operators
    ASSIGN,       // =
    PLUS_ASSIGN,  // +=
    MINUS_ASSIGN, // -=
    ARROW,        // ->

    // Punctuation
    COLON,
    COMMA,
    DOT,
    SEMICOLON,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_PAREN,
    RIGHT_PAREN,

    // Special
    RANGE,      // ..

    // Control
    END_OF_FILE,
    ERROR
};

struct Location {
    std::string file;
    int line = 0;
    int column = 0;
    int start = 0;
    int end = 0;
};

struct Token {
    TokenType type = TokenType::ERROR;
    std::string lexeme;
    Location location;
    std::variant<int64_t, double, std::string, bool> value;
};

} // namespace lex
