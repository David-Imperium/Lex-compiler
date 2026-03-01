# Lex Compiler Architecture

**Version:** 0.1.0
**Last Updated:** 2026-03-01
**Status:** Draft

---

## 1. Overview

Lex is a **multi-target transpiler** that compiles `.lex` source files to multiple output formats.

### 1.1 Compilation Pipeline

```
.lex Source
    │
    ▼
┌─────────┐
│  Lexer  │ ──► Token Stream
└─────────┘
    │
    ▼
┌──────────┐
│  Parser  │ ──► Abstract Syntax Tree (AST)
└──────────┘
    │
    ▼
┌───────────┐
│ Validator │ ──► Semantic Analysis
└───────────┘
    │
    ▼
┌────────────────────────────────────────────┐
│           Code Generation Backends          │
├──────────┬─────────┬─────────┬────────────┤
│   Lua    │  JSON   │  React  │    Lore    │
└──────────┴─────────┴─────────┴────────────┘
```

### 1.2 Design Principles

1. **Separation of Concerns**: Each pipeline stage is independent
2. **Visitor Pattern**: AST traversal uses visitors for flexibility
3. **Extensibility**: New backends can be added without modifying core
4. **Error Recovery**: Parser continues after errors for better reporting
5. **English Keywords**: All keywords in English for consistency

---

## 2. Lexer Design

### 2.1 Token System

#### TokenType Enum

```cpp
// lexer/token.h

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
    REQUIRES,
    UNLOCKS,
    TYPE,
    CATEGORY,
    LEVEL,
    QUOTE,

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
    ASSIGN,     // =
    PLUS_ASSIGN,  // +=
    MINUS_ASSIGN, // -=
    ARROW,      // ->

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
```

#### Token Struct

```cpp
struct Location {
    std::string file;
    int line;
    int column;
    int start;
    int end;
};

struct Token {
    TokenType type;
    std::string lexeme;
    Location location;
    std::variant<int64_t, double, std::string, bool> value;
};
```

### 2.2 Keyword Table (~80 entries)

```cpp
// lexer/keywords.h

inline const std::map<std::string, TokenType> KEYWORDS = {
    // Core
    {"era", TokenType::ERA},
    {"structure", TokenType::STRUCTURE},
    {"unit", TokenType::UNIT},
    {"technology", TokenType::TECHNOLOGY},
    {"resource", TokenType::RESOURCE},
    {"choice", TokenType::CHOICE},
    {"ending", TokenType::ENDING},
    {"event", TokenType::EVENT},
    {"secret", TokenType::SECRET},

    // Properties
    {"name", TokenType::NAME},
    {"id", TokenType::ID},
    {"description", TokenType::DESCRIPTION},
    {"cost", TokenType::COST},
    {"production", TokenType::PRODUCTION},
    {"maintenance", TokenType::MAINTENANCE},
    {"requires", TokenType::REQUIRES},
    {"unlocks", TokenType::UNLOCKS},
    {"type", TokenType::TYPE},
    {"category", TokenType::CATEGORY},
    {"level", TokenType::LEVEL},
    {"quote", TokenType::QUOTE},

    // Conditionals
    {"when", TokenType::WHEN},
    {"if", TokenType::IF},
    {"available_if", TokenType::AVAILABLE_IF},
    {"secret_if", TokenType::SECRET_IF},
    {"active_if", TokenType::ACTIVE_IF},

    // Logical
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"not", TokenType::NOT},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"null", TokenType::NULL_LIT},

    // UI
    {"icon", TokenType::ICON},
    {"color", TokenType::COLOR},
    {"tooltip", TokenType::TOOLTIP},
    {"panel", TokenType::PANEL},

    // Lore
    {"archivist", TokenType::ARCHIVIST},
    {"lore", TokenType::LORE},
    {"context", TokenType::CONTEXT},
    {"references", TokenType::REFERENCES},

    // Stats
    {"attack", TokenType::ATTACK},
    {"defense", TokenType::DEFENSE},
    {"movement", TokenType::MOVEMENT},
    {"range", TokenType::RANGE},
    {"health", TokenType::HEALTH},
    {"happiness", TokenType::HAPPINESS},

    // Era
    {"period", TokenType::PERIOD},
    {"music", TokenType::MUSIC},
    {"dominant_color", TokenType::DOMINANT_COLOR},
    {"transition", TokenType::TRANSITION},
    {"atmosphere", TokenType::ATMOSPHERE},

    // Technology
    {"research_cost", TokenType::RESEARCH_COST},
    {"prerequisites", TokenType::PREREQUISITES},
    {"research_time", TokenType::RESEARCH_TIME},
};
```

### 2.3 Lexer Class Interface

```cpp
// lexer/lexer.h

class Lexer {
public:
    explicit Lexer(const std::string& source, const std::string& filename = "<stdin>");

    std::vector<Token> tokenize();
    Token next_token();
    Token peek_token();

    bool has_errors() const;
    const std::vector<LexError>& errors() const;

private:
    std::string source_;
    std::string filename_;
    size_t pos_;
    int line_;
    int column_;

    std::vector<LexError> errors_;

    char current() const;
    char peek() const;
    void advance();
    void skip_whitespace();
    void skip_comment();

    Token scan_token();
    Token scan_string();
    Token scan_number();
    Token scan_identifier();
    Token scan_hex_color();
};
```

### 2.4 Error Handling

```cpp
struct LexError {
    std::string code;      // LEX001, LEX002, etc.
    std::string message;
    Location location;
    std::string suggestion;
};
```

**Error Codes:**

| Code | Description |
|------|-------------|
| LEX001 | Unterminated string literal |
| LEX002 | Invalid hex color format |
| LEX003 | Invalid number format |
| LEX004 | Unknown escape sequence |
| LEX005 | Unexpected character |

---

## 3. Parser Design

### 3.1 Recursive Descent Parser

The parser uses **recursive descent** with **lookahead** for ambiguity resolution.

```cpp
// parser/parser.h

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    std::unique_ptr<ASTNode> parse();
    std::vector<std::unique_ptr<Definition>> parse_file();

    bool has_errors() const;
    const std::vector<ParseError>& errors() const;

private:
    std::vector<Token> tokens_;
    size_t pos_;

    std::vector<ParseError> errors_;

    Token& current();
    Token& peek(int offset = 1);
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);

    // Parsing methods
    std::unique_ptr<Definition> parse_definition();
    std::unique_ptr<EraDefinition> parse_era();
    std::unique_ptr<StructureDefinition> parse_structure();
    std::unique_ptr<UnitDefinition> parse_unit();
    std::unique_ptr<TechnologyDefinition> parse_technology();
    std::unique_ptr<ChoiceDefinition> parse_choice();
    std::unique_ptr<EndingDefinition> parse_ending();

    std::unique_ptr<Property> parse_property();
    std::unique_ptr<Expression> parse_expression();
    std::unique_ptr<Expression> parse_or_expr();
    std::unique_ptr<Expression> parse_and_expr();
    std::unique_ptr<Expression> parse_comparison();
    std::unique_ptr<Expression> parse_primary();

    std::unique_ptr<ResourceMap> parse_resource_map();
    std::unique_ptr<ReferenceList> parse_reference_list();
    std::unique_ptr<ConditionalBlock> parse_conditional_block();
};
```

### 3.2 AST Node Types

See Section 4 for complete AST structure.

### 3.3 Error Recovery

```cpp
// Synchronization points: definition boundaries
void synchronize() {
    while (!is_at_end()) {
        if (previous().type == TokenType::RIGHT_BRACE) return;

        switch (current().type) {
            case TokenType::ERA:
            case TokenType::STRUCTURE:
            case TokenType::UNIT:
            case TokenType::TECHNOLOGY:
            case TokenType::CHOICE:
            case TokenType::ENDING:
                return;
            default:
                advance();
        }
    }
}
```

**Error Codes:**

| Code | Description |
|------|-------------|
| SYN001 | Missing closing brace |
| SYN002 | Unexpected token |
| SYN003 | Missing colon |
| SYN004 | Invalid property name |
| SYN005 | Expected identifier |

---

## 4. AST Structure

### 4.1 Base AST Node

```cpp
// ast/ast.h

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    Location location;
};

class ASTVisitor {
public:
    virtual void visit(EraDefinition& node) = 0;
    virtual void visit(StructureDefinition& node) = 0;
    virtual void visit(UnitDefinition& node) = 0;
    virtual void visit(TechnologyDefinition& node) = 0;
    virtual void visit(ChoiceDefinition& node) = 0;
    virtual void visit(EndingDefinition& node) = 0;
    virtual void visit(ResourceDefinition& node) = 0;

    virtual void visit(Property& node) = 0;
    virtual void visit(ResourceMap& node) = 0;
    virtual void visit(ReferenceList& node) = 0;
    virtual void visit(ConditionalBlock& node) = 0;

    virtual void visit(BinaryExpr& node) = 0;
    virtual void visit(UnaryExpr& node) = 0;
    virtual void visit(LiteralExpr& node) = 0;
    virtual void visit(ReferenceExpr& node) = 0;
    virtual void visit(CallExpr& node) = 0;
};
```

### 4.2 Definition Nodes

```cpp
// Base definition
class Definition : public ASTNode {
public:
    std::string identifier;
    std::vector<std::unique_ptr<Property>> properties;
};

// Era definition
class EraDefinition : public Definition {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::string name;
    std::string period;
    std::string atmosphere;
    std::string music;
    std::string dominant_color;

    std::unique_ptr<TransitionBlock> transition;

    std::vector<std::unique_ptr<StructureDefinition>> structures;
    std::vector<std::unique_ptr<TechnologyDefinition>> technologies;
    std::vector<std::unique_ptr<UnitDefinition>> units;
    std::vector<std::unique_ptr<ChoiceDefinition>> choices;
    std::vector<std::unique_ptr<EndingDefinition>> endings;
};

// Structure definition
class StructureDefinition : public Definition {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::string era_ref;
    std::string name;
    std::string description;

    std::unique_ptr<ResourceMap> cost;
    std::unique_ptr<ResourceMap> production;
    std::unique_ptr<ResourceMap> maintenance;

    std::unique_ptr<RequirementsBlock> requires_block;
    std::unique_ptr<UnlocksBlock> unlocks_block;

    std::unique_ptr<ConditionalBlock> available_if;

    std::unique_ptr<UIBlock> ui;
    std::unique_ptr<LoreBlock> lore;
    std::unique_ptr<IntegrationBlock> integrations;
};

// Unit definition
class UnitDefinition : public Definition {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::string era_ref;
    std::string type;
    std::string name;

    std::unique_ptr<StatsBlock> stats;
    std::unique_ptr<ResourceMap> cost;
    std::unique_ptr<ResourceMap> maintenance;

    std::unique_ptr<ReferenceList> abilities;
    std::unique_ptr<RequirementsBlock> requires_block;
};

// Technology definition
class TechnologyDefinition : public Definition {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::string era_ref;
    std::string name;
    std::string quote;

    double research_cost;
    int research_time;

    std::unique_ptr<ReferenceList> prerequisites;
    std::unique_ptr<UnlocksBlock> unlocks_block;
};

// Choice definition (narrative)
class ChoiceDefinition : public Definition {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::string era_ref;
    std::string text;

    std::unique_ptr<TriggerBlock> trigger;
    std::map<std::string, std::unique_ptr<OptionBlock>> options;
};

// Ending definition
class EndingDefinition : public Definition {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::string type;  // "normal" or "secret"
    std::string text;
    std::string cinematic;
    std::string archivist;

    std::unique_ptr<TriggerBlock> trigger;
    std::unique_ptr<EffectBlock> effect;
};
```

### 4.3 Expression Nodes

```cpp
// Base expression
class Expression : public ASTNode {
public:
    enum class Type {
        INTEGER, FLOAT, STRING, BOOLEAN, COLOR, NULL_VAL, REFERENCE
    };
    Type inferred_type;
};

// Binary expression (a op b)
class BinaryExpr : public Expression {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::unique_ptr<Expression> left;
    TokenType op;  // AND, OR, comparison, arithmetic
    std::unique_ptr<Expression> right;
};

// Unary expression (op a)
class UnaryExpr : public Expression {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    TokenType op;  // NOT, MINUS
    std::unique_ptr<Expression> operand;
};

// Literal expression
class LiteralExpr : public Expression {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::variant<int64_t, double, std::string, bool> value;
};

// Reference expression (a.b.c)
class ReferenceExpr : public Expression {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::vector<std::string> parts;  // ["civilization", "total_industry"]
};

// Function call expression
class CallExpr : public Expression {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::string function_name;
    std::vector<std::unique_ptr<Expression>> arguments;
};
```

### 4.4 Property Nodes

```cpp
// Generic property
class Property : public ASTNode {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::string name;
    std::unique_ptr<Expression> value;
};

// Resource map { Gold: 50, Wood: 20 }
class ResourceMap : public ASTNode {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::map<std::string, std::variant<int64_t, double>> resources;
};

// Reference list [SteamEngine, Metallurgy]
class ReferenceList : public ASTNode {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    std::vector<std::string> references;
};

// Conditional block
class ConditionalBlock : public ASTNode {
public:
    void accept(ASTVisitor& v) override { v.visit(*this); }

    TokenType kind;  // AVAILABLE_IF, SECRET_IF, etc.
    std::unique_ptr<Expression> condition;
    std::string message;
    std::unique_ptr<EffectBlock> effect;
    std::string flag;
    std::string archivist;
};

// Requirements block
class RequirementsBlock : public ASTNode {
public:
    std::unique_ptr<ReferenceList> technologies;
    std::unique_ptr<ReferenceList> structures;
    std::unique_ptr<ResourceMap> resources;
    std::string era;
};

// Unlocks block
class UnlocksBlock : public ASTNode {
public:
    std::unique_ptr<ReferenceList> technologies;
    std::unique_ptr<ReferenceList> structures;
    std::unique_ptr<ReferenceList> units;
    std::vector<std::string> bonuses;
};

// Stats block
class StatsBlock : public ASTNode {
public:
    std::map<std::string, std::variant<int64_t, double, std::string>> stats;
};

// UI block
class UIBlock : public ASTNode {
public:
    std::string icon;
    std::string color;
    std::unique_ptr<TooltipBlock> tooltip;
    std::unique_ptr<PanelBlock> panel;
};

// Lore block
class LoreBlock : public ASTNode {
public:
    std::string quote;
    std::string description;
    std::string ai_context;
    std::vector<std::string> historical_references;
};

// Integration block
class IntegrationBlock : public ASTNode {
public:
    std::unique_ptr<EconomyBlock> economy;
    std::unique_ptr<SocietyBlock> society;
    std::unique_ptr<MilitaryBlock> military;
};

// Trigger block
class TriggerBlock : public ASTNode {
public:
    std::unique_ptr<Expression> condition;
    double probability;
    std::string era;
    std::string flag;
};

// Effect block
class EffectBlock : public ASTNode {
public:
    std::map<std::string, std::string> resource_modifiers;  // "Gold": "+50"
    std::string flag;
    std::unique_ptr<ReferenceList> unlocks;
    std::unique_ptr<ReferenceList> locks;
};

// Transition block
class TransitionBlock : public ASTNode {
public:
    std::unique_ptr<RequirementsBlock> requires_block;
    std::string cinematic;
    std::string archivist;
};

// Option block (for choices)
class OptionBlock : public ASTNode {
public:
    std::string text;
    std::unique_ptr<EffectBlock> effect;
    std::unique_ptr<ReferenceList> unlocks;
};
```

### 4.5 Visitor Pattern

```cpp
// Example: AST printer for debugging
class ASTPrinter : public ASTVisitor {
public:
    std::string result;

    void visit(EraDefinition& node) override {
        result += "(Era " + node.identifier + " ...)";
    }

    void visit(StructureDefinition& node) override {
        result += "(Structure " + node.identifier + " ...)";
    }

    void visit(BinaryExpr& node) override {
        result += "(" + token_to_string(node.op) + " ";
        node.left->accept(*this);
        result += " ";
        node.right->accept(*this);
        result += ")";
    }

    // ... other visit methods
};
```

---

## 5. Code Generation Pipeline

### 5.1 Backend Interface

```cpp
// codegen/backend.h

class Backend {
public:
    virtual ~Backend() = default;

    virtual std::string name() const = 0;
    virtual std::string file_extension() const = 0;

    virtual std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) = 0;

    virtual void configure(const std::map<std::string, std::string>& options) {}

protected:
    void report_error(const std::string& message);
    std::vector<std::string> errors_;
};

class BackendRegistry {
public:
    static BackendRegistry& instance();

    void register_backend(std::unique_ptr<Backend> backend);
    Backend* get_backend(const std::string& name);
    std::vector<std::string> available_backends() const;

private:
    std::map<std::string, std::unique_ptr<Backend>> backends_;
};
```

### 5.2 Lua Backend

**Primary target for MVP.** Generates Lua scripts for Imperium gameplay logic.

```cpp
// codegen/lua_backend.cpp

class LuaBackend : public Backend {
public:
    std::string name() const override { return "lua"; }
    std::string file_extension() const override { return ".lua"; }

    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override {
        std::string output;

        output += generate_header();
        output += "\n";

        for (const auto& def : ast) {
            output += generate_definition(*def);
            output += "\n";
        }

        return output;
    }

private:
    std::string generate_header() {
        return "-- Auto-generated by Lex Compiler\n"
               "-- Do not edit manually\n";
    }

    std::string generate_definition(const Definition& def) {
        if (auto* era = dynamic_cast<const EraDefinition*>(&def)) {
            return generate_era(*era);
        }
        if (auto* structure = dynamic_cast<const StructureDefinition*>(&def)) {
            return generate_structure(*structure);
        }
        // ... other types
        return "";
    }

    std::string generate_structure(const StructureDefinition& structure) {
        std::string lua;
        lua += "Buildings." + structure.identifier + " = {\n";
        lua += "    id = \"" + structure.identifier + "\",\n";
        lua += "    era = \"" + structure.era_ref + "\",\n";

        if (structure.cost) {
            lua += "    cost = " + generate_resource_map(*structure.cost) + ",\n";
        }
        if (structure.production) {
            lua += "    production = " + generate_resource_map(*structure.production) + ",\n";
        }

        lua += "}\n";
        lua += "\n";
        lua += generate_structure_functions(structure);

        return lua;
    }

    std::string generate_resource_map(const ResourceMap& map) {
        std::string lua = "{";
        bool first = true;
        for (const auto& [name, value] : map.resources) {
            if (!first) lua += ", ";
            lua += name + " = " + std::to_string(value);
            first = false;
        }
        lua += "}";
        return lua;
    }

    std::string generate_structure_functions(const StructureDefinition& structure) {
        std::string lua;

        // onBuild function
        lua += "function Buildings." + structure.identifier + ":onBuild(city)\n";
        if (structure.production) {
            for (const auto& [name, value] : structure.production->resources) {
                lua += "    city.resources." + name + " = city.resources." + name
                     + " + " + std::to_string(value) + "\n";
            }
        }
        lua += "end\n";

        return lua;
    }
};
```

**Example Output:**

```lua
-- Auto-generated by Lex Compiler
-- Do not edit manually

Buildings.SteamFactory = {
    id = "SteamFactory",
    era = "Steampunk",
    cost = {Coal = 8, Steel = 5, Gold = 50},
    production = {Energy = 15, Industry = 10},
    maintenance = {Coal = 2, Gold = 5}
}

function Buildings.SteamFactory:onBuild(city)
    city.resources.Energy = city.resources.Energy + 15
    city.resources.Industry = city.resources.Industry + 10
end

function Buildings.SteamFactory:onTurn(city)
    city.resources.Coal = city.resources.Coal - 2
    city.resources.Gold = city.resources.Gold - 5
end

function Buildings.SteamFactory:isAvailable(civilization)
    return civilization.total_industry >= 100
end
```

### 5.3 JSON Backend (Future)

Generates JSON data files for frontend consumption.

```cpp
class JSONBackend : public Backend {
public:
    std::string name() const override { return "json"; }
    std::string file_extension() const override { return ".json"; }

    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override {
        nlohmann::json root = nlohmann::json::array();

        for (const auto& def : ast) {
            root.push_back(definition_to_json(*def));
        }

        return root.dump(2);
    }

private:
    nlohmann::json definition_to_json(const Definition& def);
    nlohmann::json resource_map_to_json(const ResourceMap& map);
};
```

**Example Output:**

```json
{
  "id": "SteamFactory",
  "era": "Steampunk",
  "name": "Steam Factory",
  "cost": {
    "Coal": 8,
    "Steel": 5,
    "Gold": 50
  },
  "production": {
    "Energy": 15,
    "Industry": 10
  }
}
```

### 5.4 React Backend (Future)

Generates TypeScript types and React components.

```cpp
class ReactBackend : public Backend {
public:
    std::string name() const override { return "react"; }
    std::string file_extension() const override { return ".tsx"; }

    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override {
        std::string output;

        output += generate_types(ast);
        output += generate_components(ast);

        return output;
    }

private:
    std::string generate_types(const std::vector<std::unique_ptr<Definition>>& ast);
    std::string generate_components(const std::vector<std::unique_ptr<Definition>>& ast);
};
```

**Example Output:**

```typescript
// types/buildings.ts
export interface Building {
  id: string;
  era: string;
  name: string;
  cost: ResourceMap;
  production: ResourceMap;
  maintenance?: ResourceMap;
}

export const SteamFactory: Building = {
  id: "SteamFactory",
  era: "Steampunk",
  name: "Steam Factory",
  cost: { Coal: 8, Steel: 5, Gold: 50 },
  production: { Energy: 15, Industry: 10 }
};
```

### 5.5 Lore Backend (Future)

Generates context for LLM companion (Claude, GPT-4).

```cpp
class LoreBackend : public Backend {
public:
    std::string name() const override { return "lore"; }
    std::string file_extension() const override { return ".md"; }

    std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) override {
        std::string output;

        output += "# Lore Context\n\n";

        for (const auto& def : ast) {
            output += generate_lore_entry(*def);
            output += "\n---\n\n";
        }

        return output;
    }

private:
    std::string generate_lore_entry(const Definition& def);
};
```

**Example Output:**

```markdown
# Steam Factory (Structure)

**Era:** Steampunk

## Description
Converts coal into energy and industrial output.

## AI Context
IMPORTANCE: High for industrial era progression
RECOMMENDATION: Build after unlocking Steam Engine
PREREQUISITES: Steam Engine (tech), Steel Foundry (structure)
SYNERGY: Works with Mechanical Workshop (+15% industry bonus)

## Stats
- Energy Production: +15/turn
- Industry Production: +10/turn
- Maintenance: 2 Coal/turn, 5 Gold/turn
```

---

## 6. Directory Structure

```
lex/
├── src/
│   ├── lexer/
│   │   ├── token.h           # TokenType enum, Token struct, Location
│   │   ├── keywords.h        # Keyword table (~80 entries)
│   │   ├── lexer.h           # Lexer class declaration
│   │   └── lexer.cpp         # Lexer implementation
│   │
│   ├── parser/
│   │   ├── parser.h          # Parser class declaration
│   │   ├── parser.cpp        # Parser implementation
│   │   └── validator.h       # Semantic validator
│   │
│   ├── ast/
│   │   ├── ast.h             # AST node declarations
│   │   └── ast.cpp           # AST visitor implementations
│   │
│   ├── codegen/
│   │   ├── backend.h         # Backend interface
│   │   ├── lua_backend.h     # Lua backend declaration
│   │   └── lua_backend.cpp   # Lua backend implementation
│   │
│   ├── error.h               # Error types and formatting
│   ├── utils.h               # Utility functions
│   └── main.cpp              # CLI entry point
│
├── tests/
│   ├── lexer_tests.cpp
│   ├── parser_tests.cpp
│   └── codegen_tests.cpp
│
├── examples/
│   ├── basic_structure.lex
│   ├── advanced_structure.lex
│   └── era_definition.lex
│
├── docs/
│   ├── LEX_SPECIFICATION.md
│   ├── LEX_ARCHITECTURE.md   # This file
│   ├── LEX_KEYWORDS_EN_IT.md
│   └── README.md
│
├── CMakeLists.txt
├── vcpkg.json
├── .gitignore
├── TODO.md
├── ROADMAP.md
└── progress.md
```

---

## 7. Build System

### 7.1 CMake Configuration

```cmake
cmake_minimum_required(VERSION 3.20)
project(lex VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lexc
    src/main.cpp
    src/lexer/lexer.cpp
    src/parser/parser.cpp
    src/ast/ast.cpp
    src/codegen/lua_backend.cpp
)

target_include_directories(lexc PRIVATE ${CMAKE_SOURCE_DIR}/src)

if(MSVC)
    target_compile_options(lexc PRIVATE /W4 /WX)
else()
    target_compile_options(lexc PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

# Testing
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
```

### 7.2 vcpkg Dependencies

```json
{
  "$schema": "https://raw.githubusercontent.com/microsoft/vcpkg-tool/main/docs/vcpkg.schema.json",
  "name": "lex",
  "version": "0.1.0",
  "dependencies": []
}
```

---

## 8. Error Handling Strategy

### 8.1 Error Categories

| Category | Prefix | Stage |
|----------|--------|-------|
| Lexical | LEX | Lexer |
| Syntax | SYN | Parser |
| Semantic | SEM | Validator |
| CodeGen | CGN | Backend |

### 8.2 Error Reporting Format

```json
{
  "severity": "error",
  "code": "SEM001",
  "message": "Undefined reference: UnknownTech",
  "file": "structures.lex",
  "location": {
    "line": 15,
    "column": 24,
    "start": 340,
    "end": 351
  },
  "suggestion": "Did you mean: SteamEngine?"
}
```

### 8.3 Error Recovery

1. **Lexer**: Continue tokenizing, produce ERROR tokens
2. **Parser**: Synchronize at definition boundaries
3. **Validator**: Collect all errors, don't stop at first
4. **Backend**: Skip invalid definitions, report errors

---

## 9. Testing Strategy

### 9.1 Unit Tests

- Token classification (keywords)
- Lexer edge cases (strings, numbers, colors)
- Parser grammar rules
- AST construction
- Code generation templates

### 9.2 Integration Tests

- Full pipeline: `.lex` → Lua
- Roundtrip testing
- Error reporting quality

### 9.3 Test Files

```
tests/
├── fixtures/
│   ├── valid/
│   │   ├── minimal.lex
│   │   └── complete.lex
│   └── invalid/
│       ├── syntax_error.lex
│       └── semantic_error.lex
├── expected/
│   ├── minimal.lua
│   └── complete.lua
└── test_runner.cpp
```

---

## 10. Future Extensions

### 10.1 Planned Features

| Version | Feature |
|---------|---------|
| 0.2.0 | JSON backend |
| 0.2.0 | React backend |
| 0.3.0 | LSP server |
| 0.3.0 | VS Code extension |
| 0.4.0 | Language Server Protocol |
| 1.0.0 | Lore backend |

### 10.2 Experimental Features

- Inline Lua blocks
- Computed properties
- Generic resource types
- Macro system

---

**Version:** 0.1.0
**Last Updated:** 2026-03-01
**Status:** Draft - Subject to changes during implementation
