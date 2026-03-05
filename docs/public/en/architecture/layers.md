# Lex Architecture

**Version:** 0.4.0

---

## Layer System

```
┌─────────────────────────────────────────────┐
│                 Lex Neural                  │  AI Context, Lore Generation
├─────────────────────────────────────────────┤
│                  Lex Core                   │  Validation, Cross-references
├─────────────────────────────────────────────┤
│                  Lex Base                   │  Lexer, Parser, AST
└─────────────────────────────────────────────┘
```

---

## 1. Lex Base

**Purpose:** Foundation layer

**Components:**
- `Lexer` - Tokenizes source code
- `Parser` - Builds AST
- `AST` - Abstract Syntax Tree
- `SourceManager` - File/string handling

**Dependencies:** None

**Output:** AST

---

## 2. Lex Core

**Purpose:** Game content layer

**Components:**
- `Validator` - Semantic validation
- `TypeChecker` - Type inference
- `Schema` - Definition registry
- `ReferenceResolver` - Cross-reference resolution

**Dependencies:** Lex Base

**Output:** Validated AST

---

## 3. Lex Neural

**Purpose:** AI integration layer

**Components:**
- `AIContextGenerator` - Generate AI-ready context (JSON/Markdown/Minimal)
- `QuerySystem` - Natural language queries on game data
- `LoreGenerator` - Narrative content generation

**Dependencies:** Lex Core

**Output:** AI-readable context (Markdown, JSON, Minimal)

**New in v0.4.0:**
- Three output formats for AI context
- Query system for natural language search
- Compact minimal format for LLM context windows

---

## 4. Lex Engine (Private)

**Purpose:** Engine integration layer

**Location:** `C:\Imperium 2.0\lex_engine\`

**Components:**
- `Runtime` - FFI interface
- `EngineBridge` - Engine calls
- `VerboseLogger` - Structured errors
- `SchemaValidator` - Engine-specific validation

**Dependencies:** Lex Core + Imperium Engine

**Output:** Engine commands / Validated input

---

## File Structure

```
C:\Lex\
├── src/
│   ├── lexer/          # Lex Base
│   ├── parser/         # Lex Base
│   ├── ast/            # Lex Base
│   ├── semantic/       # Lex Core
│   │   ├── validator.cpp
│   │   ├── type_checker.cpp
│   │   └── schema.cpp
│   ├── codegen/        # Output backends
│   │   ├── lua_backend.cpp
│   │   ├── json_backend.cpp
│   │   ├── godot_backend.cpp
│   │   ├── unity_backend.cpp
│   │   ├── love2d_backend.cpp
│   │   └── defold_backend.cpp
│   └── lex.cpp          # Main API
│
└── docs/
    └── spec/
        ├── language.md
        ├── validation.md
        └── outputs.md
```

---

## Data Flow

```
.lex file
    │
    ▼ Lexer ──→ Tokens
    │
    ▼ Parser ──→ AST
    │
    ▼ Validator ──→ Validated AST
    │
    ▼ TypeChecker ──→ Typed AST
    │
    ├─▶ Backend ──→ Output (Lua/JSON/Godot/Unity/Love2D/Defold)
    │
    └─▶ AIContextGenerator ──→ AI Context (Markdown/JSON/Minimal)
                                   │
                                   └─▶ QuerySystem ──→ Query Results
```

---

## Extension Points

### Adding a New Backend

1. Create `src/codegen/xxx_backend.cpp`
2. Inherit from `Backend` interface
3. Register in `BackendFactory`
4. Add to CLI options

### Adding a New Keyword

1. Add token in `lexer/keywords.h`
2. Add grammar rule in `parser/grammar.cpp`
3. Add validation in `semantic/validator.cpp`
4. Update `docs/spec/language.md`
