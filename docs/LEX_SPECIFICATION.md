# Lex Language Specification

**Version:** 0.1.0  
**Last Updated:** 2026-03-01  
**Status:** Draft

---

## Overview

Lex is a **declarative, multi-target transpiler language** for game modding. It compiles `.lex` files to multiple output formats (Lua, JSON, React, Lore context for LLMs).

**Key Characteristics:**
- **Declarative**: Describe WHAT, not HOW
- **English keywords only** (standard for AI agents, humans, and tools)
- **Type-safe**: Strong typing with inference
- **Reference-safe**: Cross-reference validation

---

## 1. Lexical Structure

### 1.1 Character Set

```
Letter        = [a-zA-Z]
Digit         = [0-9]
HexDigit      = [0-9a-fA-F]
Underscore    = "_"
Identifier    = Letter (Letter | Digit | Underscore)*
```

### 1.2 Literals

```ebnf
StringLiteral     = '"' { Character | Escape }* '"'
                    | '"' '"' { Character | Escape }* '"' '"'  (* multiline *)

IntegerLiteral    = Digit+
FloatLiteral      = Digit+ "." Digit+
HexColorLiteral   = "#" HexDigit HexDigit HexDigit HexDigit HexDigit HexDigit
BooleanLiteral    = "true" | "false" | "vero" | "falso"
NullLiteral       = "null" | "nullo"
```

**Escape Sequences:**
- `\n` - newline
- `\t` - tab
- `\"` - quote
- `\\` - backslash

**Examples:**
```lex
name: "Steam Factory"
description: "Converts coal into energy"
quote: "Il vapore trasforma il ferro in potere"

amount: 50
ratio: 1.5
color: #8B6914
enabled: true
```

### 1.3 Keywords

**Core Constructs:**

| Token | English | Description |
|-------|---------|-------------|
| `ERA` | `era` | Defines an'era historica |
| `STRUCTURE` | `structure` | Definesce un edificio |
| `UNIT` | `unit` | Definesce un'unità |
| `TECHNOLOGY` | `technology` | Definisce una tecnologia |
| `RESOURCE` | `resource` | Definisce una risorsa |
| `CHOICE` | `choice` | Scelta narrativa |
| `ENDING` | `ending` | Finale del gioco |
| `EVENT` | `event` | Evento generico |
| `SECRET` | `secret` | Contenuto segreto |

**Conditional Constructs:**

| Token | English | Description |
|-------|---------|-------------|
| `WHEN` | `when` | Condizione temporale |
| `IF` | `if` | Condizione generica |
| `AVAILABLE_IF` | `available_if` | Condizione disponibilità |
| `SECRET_IF` | `secret_if` | Condizione segreto |
| `ACTIVE_IF` | `active_if` | Condizione attivazione |
| `BONUS_IF` | `bonus_if` | Condizione bonus |

**Logical Operators:**

| Token | English | Description |
|-------|---------|-------------|
| `AND` | `and` | AND logico |
| `OR` | `or` | OR logico |
| `NOT` | `not` | NOT logico |
| `TRUE` | `true` | Booleano vero |
| `FALSE` | `false` | Booleano falso |

**Property Keywords:**

| Token | English | Description |
|-------|---------|-------------|
| `NAME` | `name` | Nome visualizzato |
| `ID` | `id` | Identificatore unico |
| `DESCRIPTION` | `description` | Descrizione testuale |
| `COST` | `cost` | Costo costruzione |
| `PRODUCTION` | `production` | Produzione per turno |
| `MAINTENANCE` | `maintenance` | Costo mantenimento |
| `REQUIRES` | `requires` | Prerequisiti necessari |
| `UNLOCKS` | `unlocks` | Cosa sblocca |
| `TYPE` | `type` | Tipo di entità |
| `CATEGORY` | `category` | Categoria |
| `LEVEL` | `level` | Livello/tier |

**Full keyword table:** See `LEX_KEYWORDS.md`

### 1.4 Operators

```ebnf
ComparisonOp    = "==" | "!=" | ">" | "<" | ">=" | "<="
ArithmeticOp    = "+" | "-" | "*" | "/" | "%"
AssignmentOp    = "=" | "+=" | "-="
LogicalOp       = "and" | "or" | "not"
ArrowOp         = "->"
RangeOp         = ".."
```

### 1.5 Punctuation

```ebnf
Colon           = ":"
Comma           = ","
Dot             = "."
Semicolon       = ";"
LeftBrace       = "{"
RightBrace      = "}"
LeftBracket     = "["
RightBracket    = "]"
LeftParen       = "("
RightParen      = ")"
```

### 1.6 Comments

```ebnf
LineComment     = "//" { Character }* EndOfLine
BlockComment    = "/*" { Character }* "*/"
```

**Examples:**
```lex
// This is a line comment

/*
   This is a block comment
   spanning multiple lines
*/
```

---

## 2. Type System

### 2.1 Primitive Types

| Type | Description | Example |
|------|-------------|---------|
| `Integer` | 64-bit signed integer | `42`, `-17`, `0` |
| `Float` | 64-bit floating point | `3.14`, `-0.5`, `1.0` |
| `String` | UTF-8 string | `"Hello"`, `"Ciao mondo"` |
| `Boolean` | Boolean value | `true`, `false` |
| `Color` | Hex color | `#8B6914`, `#FF0000` |
| `Null` | Null value | `null` |

### 2.2 Composite Types

#### ResourceMap
Map of resource names to quantities.

```ebnf
ResourceMap = "{" ResourceEntry { "," ResourceEntry } "}"
ResourceEntry = Identifier ":" ( Integer | Float )
```

**Example:**
```lex
cost: {
    Gold: 50,
    Wood: 20,
    Coal: 8
}
```

#### ReferenceList
List of cross-references to other definitions.

```ebnf
ReferenceList = "[" Identifier { "," Identifier } "]"
```

**Example:**
```lex
requires: {
    technologies: [SteamEngine, Metallurgy],
    structures: [SteelFoundry]
}
```

#### StatsBlock
Block of numeric statistics.

```ebnf
StatsBlock = "{" StatEntry { "," StatEntry } "}"
StatEntry = Identifier ":" ( Integer | Float | Percentage )
Percentage = ( Integer | Float ) "%"
```

**Example:**
```lex
stats: {
    attack: 25,
    defense: 20,
    movement: 3,
    speed_bonus: +10%
}
```

#### ConditionalBlock
Conditional property with trigger and effects.

```ebnf
ConditionalBlock = ConditionalKeyword "{" 
                     ConditionClause [ MessageClause ]
                     EffectClause [ FlagClause ] [ ArchivistClause ]
                   "}"
ConditionalKeyword = "available_if" | "secret_if" | "active_if" | "bonus_if"
```

**Example:**
```lex
available_if: {
    condition: civilization.total_industry >= 100,
    message: "Your industry is developed enough"
}
```

### 2.3 Type Inference

Types are inferred from values. No explicit type declarations required.

```lex
era: Steampunk           // inferred: Reference
cost: 50                 // inferred: Integer
ratio: 1.5               // inferred: Float
enabled: true            // inferred: Boolean
color: #8B6914           // inferred: Color
```

---

## 3. Grammar (EBNF)

### 3.1 Top-Level Structure

```ebnf
LexFile         = { Definition | Comment }

Definition      = EraDefinition
                | StructureDefinition
                | UnitDefinition
                | TechnologyDefinition
                | ResourceDefinition
                | ChoiceDefinition
                | EndingDefinition
                | EventDefinition
```

### 3.2 Core Definitions

#### Era Definition
```ebnf
EraDefinition       = "era" | "era" Identifier "{" 
                        EraProperty { "," EraProperty } 
                      "}"

EraProperty         = PropertyAssignment
                    | "structures" ":" StructureBlock
                    | "technologies" ":" TechnologyBlock
                    | "units" ":" UnitBlock
                    | "choices" ":" ChoiceBlock
                    | "endings" ":" EndingBlock
                    | "transition" ":" TransitionBlock
                    | "atmosphere" ":" StringLiteral
                    | "music" ":" StringLiteral
                    | "dominant_color" ":" HexColorLiteral
```

#### Structure Definition
```lex
StructureDefinition = "structure" Identifier "{"
                        StructureProperty { "," StructureProperty }
                      "}"

StructureProperty   = PropertyAssignment
                    | "cost" ":" ResourceMap
                    | "production" ":" ResourceMap
                    | "maintenance" ":" ResourceMap
                    | "requires" ":" RequirementsBlock
                    | "unlocks" ":" UnlocksBlock
                    | "available_if" ":" ConditionalBlock
                    | "events" ":" EventBlock
                    | "ui" ":" UIBlock
                    | "lore" ":" LoreBlock
                    | "integrations" ":" IntegrationBlock
```

#### Unit Definition
```lex
UnitDefinition      = "unit" Identifier "{"
                        UnitProperty { "," UnitProperty }
                      "}"

UnitProperty        = PropertyAssignment
                    | "type" ":" Identifier
                    | "stats" ":" StatsBlock
                    | "cost" ":" ResourceMap
                    | "maintenance" ":" ResourceMap
                    | "abilities" ":" ReferenceList
                    | "requires" ":" RequirementsBlock
```

#### Technology Definition
```lex
TechnologyDefinition = "technology" Identifier "{"
                         TechnologyProperty { "," TechnologyProperty }
                       "}"

TechnologyProperty  = PropertyAssignment
                    | "research_cost" ":" ( Integer | Float )
                    | "research_time" ":" Integer
                    | "prerequisites" ":" ReferenceList
                    | "unlocks" ":" UnlocksBlock
                    | "quote" ":" StringLiteral
```

#### Choice Definition (Narrative)
```lex
ChoiceDefinition    = "choice" Identifier "{"
                        ChoiceProperty { "," ChoiceProperty }
                      "}"

ChoiceProperty      = PropertyAssignment
                    | "era" ":" Identifier
                    | "trigger" ":" TriggerBlock
                    | "text" ":" StringLiteral
                    | "options" ":" OptionBlock
```

#### Ending Definition
```ebnf
EndingDefinition    = ( "ending" | "finale" ) Identifier "{"
                        EndingProperty { "," EndingProperty }
                      "}"

EndingProperty      = PropertyAssignment
                    | "type" | "tipo" ":" ( "normal" | "secret" | "segreto" )
                    | "trigger" ":" TriggerBlock
                    | "cinematic" ":" StringLiteral
                    | "text" ":" StringLiteral
                    | "effect" ":" EffectBlock
                    | "archivist" ":" StringLiteral
```

### 3.3 Common Structures

#### Property Assignment
```ebnf
PropertyAssignment  = PropertyName ":" PropertyValue

PropertyName        = Identifier  (* e.g., "name", "era", "description" *)

PropertyValue       = Literal
                    | Identifier  (* reference or enum value *)
                    | ResourceMap
                    | StatsBlock
                    | ConditionalBlock
                    | ReferenceList
                    | BlockValue
```

#### Block Value
```ebnf
BlockValue          = "{" BlockProperty { "," BlockProperty } "}"
BlockProperty       = PropertyAssignment
```

#### Requirements Block
```lex
RequirementsBlock   = "{" 
                        [ "technologies" ":" ReferenceList ] ","
                        [ "structures" ":" ReferenceList ] ","
                        [ "resources" ":" ResourceMap ] ","
                        [ "era" ":" Identifier ]
                      "}"
```

#### Unlocks Block
```lex
UnlocksBlock        = "{"
                        [ "technologies" ":" ReferenceList ] ","
                        [ "structures" ":" ReferenceList ] ","
                        [ "units" ":" ReferenceList ] ","
                        [ "bonuses" ":" StringList ]
                      "}"

StringList          = "[" StringLiteral { "," StringLiteral } "]"
```

#### Trigger Block
```lex
TriggerBlock        = "{"
                        "condition" ":" Expression ","
                        [ "probability" ":" ( Integer | Float | Percentage ) ] ","
                        [ "era" ":" Identifier ] ","
                        [ "flag" ":" Identifier ]
                      "}"
```

#### Effect Block
```ebnf
EffectBlock         = "{"
                        [ ResourceModifier { "," ResourceModifier } ] ","
                        [ "flag" ":" Identifier ] ","
                        [ "unlocks" ":" ReferenceList ] ","
                        [ "locks" ":" ReferenceList ]
                      "}"

ResourceModifier    = Identifier ":" ( Integer | Float | ModExpression )
ModExpression       = ( "+" | "-" ) ( Integer | Float )
```

---

## 4. Expressions

### 4.1 Boolean Expressions

Used in conditions and triggers.

```ebnf
Expression          = OrExpression

OrExpression        = AndExpression { "or" AndExpression }

AndExpression       = ComparisonExpression { "and" ComparisonExpression }

ComparisonExpression = NotExpression [ ComparisonOp NotExpression ]

NotExpression       = "not" PrimaryExpression
                    | PrimaryExpression

PrimaryExpression   = Literal
                    | ReferenceExpression
                    | FunctionCall
                    | "(" Expression ")"

ReferenceExpression = Identifier { "." Identifier } [ "." Identifier ]
                    (* e.g., civilization.total_industry, map.current_tile.has_river *)

FunctionCall        = Identifier "(" [ ArgumentList ] ")"
                    (* e.g., has_structure_in_every_city(SteamFactory) *)
```

### 4.2 Comparison Operators

| Operator | Meaning | Example |
|----------|---------|---------|
| `==` | Equal | `era == Steampunk` |
| `!=` | Not equal | `status != AtWar` |
| `>` | Greater than | `population > 100` |
| `<` | Less than | `gold < 50` |
| `>=` | Greater or equal | `happiness >= 70` |
| `<=` | Less or equal | `stability <= 30` |

### 4.3 Arithmetic Expressions

Used in effect calculations.

```ebnf
ArithmeticExpression = Term { ( "+" | "-" ) Term }

Term                = Factor { ( "*" | "/" | "%" ) Factor }

Factor              = Literal
                    | ReferenceExpression
                    | "(" ArithmeticExpression ")"
```

### 4.4 Examples

```lex
// Boolean expressions
condition: population > 100 and happiness < 40
condition: era == Steampunk or era == Cyberpunk
condition: not has_war_with(Neighbor)
condition: civilization.total_industry >= 100

// Arithmetic expressions
production: { Energy: 15 + 5 }  // 20
cost: { Gold: base_cost * 1.5 }  // 1.5x multiplier
```

---

## 5. Semantic Rules

### 5.1 Identifier Scoping

**Rule:** All definitions are globally scoped within a single `.lex` file.

```lex
// ✅ Valid: Reference to previously defined structure
structure SteelFoundry { ... }

structure SteamFactory {
    requires: { structures: [SteelFoundry] }  // OK
}
```

**Rule:** Forward references are allowed (parser resolves in second pass).

```lex
// ✅ Valid: Forward reference
structure SteamFactory {
    requires: { structures: [AdvancedFactory] }  // OK even if defined later
}

structure AdvancedFactory { ... }
```

### 5.2 Cross-Reference Validation

**Rule:** All referenced identifiers must exist in the compilation unit.

```lex
// ❌ Error: Undefined reference
structure Factory {
    requires: { technologies: [UnknownTech] }  // ERROR: UnknownTech not defined
}
```

**Rule:** Reference types must match expected categories.

```lex
// ❌ Error: Type mismatch
structure Factory {
    requires: { structures: [SteamEngine] }  // ERROR: SteamEngine is a technology, not structure
}
```

### 5.3 Era Membership

**Rule:** Structures, units, and technologies must belong to an era.

```lex
// ✅ Valid: Era specified
structure SteamFactory {
    era: Steampunk  // Required
    // ...
}

// ❌ Error: Missing era
structure Factory {
    // ERROR: era property required
}
```

**Rule:** Era values must reference defined eras.

```lex
era Steampunk { ... }

structure Factory {
    era: Steampunk  // OK
}

structure CyberBuilding {
    era: Cyberpunk  // ERROR: Cyberpunk era not defined
}
```

### 5.4 Resource Validation

**Rule:** Resource names in maps must be consistent.

```lex
// ✅ Valid: Consistent resource naming
cost: { Gold: 50, Wood: 20 }
production: { Gold: 10, Wood: 5 }

// ⚠️ Warning: Inconsistent casing
cost: { gold: 50 }  // lowercase
production: { Gold: 10 }  // capitalized
```

### 5.5 Conditional Logic

**Rule:** Conditional blocks must contain a `condition` clause.

```lex
// ✅ Valid
available_if: {
    condition: population > 100
}

// ❌ Error: Missing condition
available_if: {
    message: "Not enough population"  // ERROR: condition required
}
```

**Rule:** Effect modifiers must use valid operators.

```lex
// ✅ Valid
effect: {
    production: { Energy: +5 }  // +5 bonus
    maintenance: { Gold: -2 }   // -2 reduction
}

// ❌ Invalid
effect: {
    production: { Energy: *2 }  // ERROR: multiplication not allowed in modifiers
}
```

---

## 6. Special Blocks

### 6.1 UI Block

Defines UI properties for frontend rendering.

```lex
UIBlock             = "ui" ":" "{"
                        [ "icon" ":" StringLiteral ] ","
                        [ "color" ":" HexColorLiteral ] ","
                        [ "tooltip" ":" TooltipBlock ] ","
                        [ "panel" ":" PanelBlock
                      "}"

TooltipBlock        = "{"
                        [ "title" ":" StringLiteral ] ","
                        [ "description" ":" StringLiteral ] ","
                        [ "stats" ":" StringList ] ","
                        [ "requirements" ":" StringList ]
                      "}"

PanelBlock          = "{"
                        [ "components" ":" StringList ] ","
                        [ "actions" ":" ActionList ]
                      "}"

ActionList          = "[" ActionDef { "," ActionDef } "]"
ActionDef           = "{"
                        "name" ":" StringLiteral ","
                        "icon" ":" StringLiteral ","
                        "action" ":" StringLiteral
                      "}"

TooltipBlock        = "{"
                        [ "title" | "titolo" ":" StringLiteral ] ","
                        [ "description" | "descrizione" ":" StringLiteral ] ","
                        [ "stats" ":" StringList ] ","
                        [ "requirements" | "requisiti" ":" StringList ]
                      "}"

PanelBlock          = "{"
                        [ "components" | "componenti" ":" StringList ] ","
                        [ "actions" | "azioni" ":" ActionList ]
                      "}"

ActionList          = "[" ActionDef { "," ActionDef } "]"
ActionDef           = "{"
                        "name" | "nome" ":" StringLiteral ","
                        "icon" | "icona" ":" StringLiteral ","
                        "action" | "azione" ":" StringLiteral
                      "}"
```

### 6.2 Lore Block

Defines AI context for LLM companion.

```lex
LoreBlock           = "lore" ":" "{"
                        [ "quote" ":" StringLiteral ] ","
                        [ "description" ":" StringLiteral ] ","
                        [ "ai_context" ":" StringLiteral ] ","
                        [ "historical_references" ":" StringList ]
                      "}"
```

### 6.3 Integration Block

Defines cross-system integrations.

```lex
IntegrationBlock    = "integrations" ":" "{"
                        [ "economy" ":" EconomyBlock ] ","
                        [ "society" ":" SocietyBlock ] ","
                        [ "military" ":" MilitaryBlock ]
                      "}"

EconomyBlock        = "{"
                        [ "market" ":" MarketBlock ]
                      "}"

MarketBlock         = "{"
                        [ "demand" ":" ResourceMap ] ","
                        [ "supply" ":" ResourceMap ]
                      "}"

SocietyBlock        = "{"
                        [ "employment" ":" Integer ] ","
                        [ "health" ":" Integer ] ","
                        [ "happiness" ":" Integer ]
                      "}"

MilitaryBlock       = "{"
                        [ "support_units" ":" ReferenceList ]
                      "}"
```

---

## 7. Complete Example

### 7.1 Minimal Structure

```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

### 7.2 Full Structure

```lex
structure SteamFactory {
    era: Steampunk
    name: "Steam Factory"
    description: "Converts coal into energy"
    
    cost: {
        Coal: 8,
        Steel: 5,
        Gold: 50
    }
    
    production: {
        Energy: 15,
        Industry: 10
    }
    
    maintenance: {
        Coal: 2,
        Gold: 5
    }
    
    requires: {
        technologies: [SteamEngine],
        structures: [SteelFoundry]
    }
    
    unlocks: {
        technologies: [AdvancedSteamEngine, TransportZeppelin],
        structures: [MechanicalWorkshop]
    }
    
    available_if: {
        condition: civilization.total_industry >= 100,
        message: "Your industry is developed enough"
    }
    
    ui: {
        icon: "assets/steampunk/steam_factory.png",
        color: #8B6914,
        
        tooltip: {
            title: "Steam Factory",
            description: "Converts coal into energy and industrial output",
            stats: [
                "Energy Production: +15",
                "Industry Production: +10",
                "Maintenance: 2 Coal/turn, 5 Gold/turn"
            ]
        }
    }
    
    lore: {
        quote: "Steam transforms iron into power",
        ai_context: "CRITICAL: This structure is essential for Steampunk era progression"
    }
}
```

### 7.3 Era Definition

```lex
era Steampunk {
    name: "Steampunk Era"
    period: "1850-1900"
    description: "The age of steam, mechanical engineering, and industrial revolution"
    
    atmosphere: "industrial, foggy, mechanical"
    music: "steampunk_ost"
    dominant_color: #8B6914
    
    transition: {
        requires: {
            technologies_minimum: 12,
            resources: { Energy: 1000, Industry: 500 }
        },
        cinematic: "SteampunkTransitionCinematic",
        archivist: "The smell of coal fills the air. A new era has begun."
    }
    
    structures: {
        SteamFactory: { /* ... */ }
        MechanicalWorkshop: { /* ... */ }
        CoalRefinery: { /* ... */ }
    }
    
    technologies: {
        SteamEngine: {
            name: "Steam Engine",
            research_cost: 450,
            prerequisites: [Metallurgy, BasicThermodynamics],
            unlocks: {
                structures: [SteamFactory, MechanicalWorkshop],
                units: [ArmoredTrain]
            }
        }
    }
    
    units: {
        SteamTank: {
            type: military,
            stats: { attack: 25, defense: 20, movement: 3 },
            cost: { Steel: 15, Coal: 8, Gold: 150 },
            abilities: [Breakthrough, Armored]
        }
    }
    
    choices: {
        WorkersRevolution: {
            trigger: {
                condition: population > 100 and happiness < 40,
                probability: 30%
            },
            text: "Factory workers are organizing. How do you respond?",
            options: {
                Suppress: {
                    effect: { Industry: +30, Happiness: -20 }
                },
                Negotiate: {
                    effect: { Happiness: +25, Gold: -200 }
                }
            }
        }
    }
}
```

---

## 8. Error Handling

### 8.1 Lexical Errors

| Error Code | Description | Example |
|------------|-------------|---------|
| `LEX001` | Unterminated string | `"unclosed string` |
| `LEX002` | Invalid hex color | `#ZZZZZZ` |
| `LEX003` | Invalid number format | `12.34.56` |
| `LEX004` | Unknown escape sequence | `"\x"` |

### 8.2 Syntax Errors

| Error Code | Description | Example |
|------------|-------------|---------|
| `SYN001` | Missing closing brace | `{ unclosed block` |
| `SYN002` | Unexpected token | `era: { } :` |
| `SYN003` | Missing colon | `era Steampunk` (should be `era: Steampunk`) |
| `SYN004` | Invalid property | `foo: bar` (unknown property) |

### 8.3 Semantic Errors

| Error Code | Description | Example |
|------------|-------------|---------|
| `SEM001` | Undefined reference | `requires: [UnknownTech]` |
| `SEM002` | Duplicate definition | Two `structure Farm` definitions |
| `SEM003` | Missing required property | Structure without `era:` |
| `SEM004` | Type mismatch | `era: 123` (should be identifier) |
| `SEM005` | Circular dependency | A requires B, B requires A |

### 8.4 Error Format

Errors are reported in structured JSON format for tooling:

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

---

## 9. Output Targets

### 9.1 Lua Backend

Generates Lua scripts for gameplay logic.

**Input:**
```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

**Output:**
```lua
-- Auto-generated from: farm.lex
Buildings.Farm = {
    id = "Farm",
    era = "Ancient",
    cost = { Gold = 50 },
    production = { Food = 5 }
}

function Buildings.Farm:onBuild(city)
    city.resources.Food = city.resources.Food + 5
end
```

### 9.2 JSON Backend

Generates JSON data files.

**Input:**
```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

**Output:**
```json
{
  "id": "Farm",
  "era": "Ancient",
  "cost": {
    "Gold": 50
  },
  "production": {
    "Food": 5
  }
}
```

### 9.3 React Backend

Generates TypeScript types and React components.

**Output:**
```typescript
// types/buildings.ts
export interface Building {
  id: string;
  era: string;
  cost: ResourceMap;
  production: ResourceMap;
}

export const Farm: Building = {
  id: "Farm",
  era: "Ancient",
  cost: { Gold: 50 },
  production: { Food: 5 }
};
```

### 9.4 Lore Backend

Generates context for LLM companion.

**Output:**
```
# Farm (Building)
Era: Ancient

## Description
Basic food production structure.

## AI Context
IMPORTANCE: Low
RECOMMENDATION: Build in every city for population growth.
PREREQUISITES: None
SYNERGY: Works well with Granary (+25% food storage)
```

---

## 10. Future Extensions

### 10.1 Planned Features (v0.2.0)

- **Enums**: Define custom enum types
- **Inheritance**: Structure templates
- **Macros**: Reusable property blocks
- **Namespaces**: Module system

### 10.2 Experimental Features

- **Inline Lua**: Embed Lua code for complex logic
- **Computed Properties**: Dynamic calculations
- **Generics**: Generic resource types

---

## Appendix A: Complete Token List

See `LEX_KEYWORDS_EN_IT.md` for the complete bilingual keyword reference.

---

## Appendix B: Grammar Railroad Diagrams

(To be generated from EBNF)

---

**Version:** 0.1.0  
**Last Updated:** 2026-03-01  
**Status:** Draft - Subject to changes during implementation
