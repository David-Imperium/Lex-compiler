# Lex Ecosystem Integration

**Versione:** 0.1.0  
**Data:** 2026-03-01  
**Stato:** 🟡 In Definizione

---

## Overview

Lex è progettato come **linguaggio collante multi-target** che integra:

1. **Game Engines** (Imperium e altri)
2. **Development Tools** (Editor, IDE, LSP)
3. **AI Systems** (Tutorial, Companion, Debugging)

Questo documento descrive come Lex si integra con l'ecosistema completo.

---

## Architettura Multi-Target

```
                    ┌─────────────────────────────────────┐
                    │           Lex Compiler              │
                    │         (Transpiler Core)           │
                    └────────────────┬────────────────────┘
                                     │
           ┌─────────────────────────┼─────────────────────────┐
           │                         │                         │
           ▼                         ▼                         ▼
    ┌──────────────┐         ┌──────────────┐         ┌──────────────┐
    │  Game Layer  │         │  Tool Layer  │         │   AI Layer   │
    │   (Runtime)  │         │  (Dev Time)  │         │  (Runtime)   │
    └──────────────┘         └──────────────┘         └──────────────┘
           │                         │                         │
    ┌──────┴──────┐           ┌──────┴──────┐           ┌──────┴──────┐
    │             │           │             │           │             │
    ▼             ▼           ▼             ▼           ▼             ▼
┌───────┐   ┌───────┐   ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐
│ Lua   │   │ JSON  │   │ LSP      │  │ Editor   │  │ Tutorial │  │ Companion│
│ Scripts│  │ Data  │   │ Server   │  │ Formats  │  │ Context  │  │ Context  │
└───────┘   └───────┘   └──────────┘  └──────────┘  └──────────┘  └──────────┘
    │             │           │             │             │             │
    ▼             ▼           ▼             ▼             ▼             ▼
 Imperium      Any         VS Code      Custom       Local LLM    In-Game
 Engine        Engine      Zed          Rust Editor  (Tutorial)   LLM
                                        (Imperium)
```

---

## Target 1: Lua Scripts (Game Runtime)

### Output
```lua
-- generated/scripts/structures/steam_factory.lua
local SteamFactory = {
    id = "steam_factory",
    name = "Steam Factory",
    era = "steampunk",
    
    cost = {
        coal = 8,
        steel = 5,
        gold = 50
    },
    
    production = {
        energy = 15,
        industry = 10
    },
    
    maintenance = {
        coal = 2,
        gold = 5
    },
    
    requires = {
        technologies = {"steam_engine"},
        structures = {"steel_foundry"}
    },
    
    available_if = function(state)
        return state.total_industry >= 100
    end,
    
    on_built = function(state, city, tile)
        -- Event logic
        if state:has_structure_in_every_city("steam_factory") then
            state:activate_event("industrial_conspiracy")
        end
    end
}

return SteamFactory
```

### Integrazione
- **Imperium Engine:** LuaJIT + sol2 (già implementato)
- **Altri Engine:** Standard Lua 5.4

---

## Target 2: JSON Data (Game Runtime)

### Output
```json
// generated/data/structures/steam_factory.json
{
  "$schema": "../schemas/structure.schema.json",
  "id": "steam_factory",
  "name": "Steam Factory",
  "era": "steampunk",
  
  "cost": {
    "coal": 8,
    "steel": 5,
    "gold": 50
  },
  
  "production": {
    "energy": 15,
    "industry": 10
  },
  
  "maintenance": {
    "coal": 2,
    "gold": 5
  },
  
  "requires": {
    "technologies": ["steam_engine"],
    "structures": ["steel_foundry"]
  },
  
  "ui": {
    "icon": "assets/steampunk/steam_factory.png",
    "color": "#8B6914",
    "tooltip": "Converts coal into energy and industry"
  },
  
  "metadata": {
    "version": "1.0.0",
    "generated_from": "mods/steampunk/structures.lex",
    "generated_at": "2026-03-01T12:00:00Z"
  }
}
```

### Integrazione
- **Qualsiasi Engine:** Parsing JSON standard
- **C++ Engines:** nlohmann/json
- **Rust Engines:** serde_json
- **Custom Engines:** Parser JSON nativo

---

## Target 3: LSP Server (Tool Layer)

### Funzionalità

**1. Autocomplete**
```json
// LSP Request
{
  "textDocument": { "uri": "file:///mods/steampunk/structures.lex" },
  "position": { "line": 5, "character": 10 },
  "context": { "triggerKind": "invoked" }
}

// LSP Response
{
  "items": [
    {
      "label": "era:",
      "kind": "property",
      "detail": "The era this structure belongs to",
      "insertText": "era: ${1:Era}"
    },
    {
      "label": "cost:",
      "kind": "property",
      "detail": "Resource costs to build",
      "insertText": "cost: {\n  ${1:Resource}: ${2:Amount}\n}"
    }
  ]
}
```

**2. Diagnostics**
```json
// LSP Notification
{
  "uri": "file:///mods/steampunk/structures.lex",
  "diagnostics": [
    {
      "range": {
        "start": { "line": 10, "character": 5 },
        "end": { "line": 10, "character": 15 }
      },
      "severity": 1,  // Error
      "message": "Unknown resource type 'Adamantium'",
      "source": "lex-validator",
      "relatedInformation": [
        {
          "location": {
            "uri": "file:///schemas/resources.schema.json",
            "range": { "start": { "line": 0, "character": 0 }, "end": { "line": 0, "character": 0 } }
          },
          "message": "Valid resource types: Coal, Steel, Gold, Wood, Food, ..."
        }
      ]
    }
  ]
}
```

**3. Go to Definition**
```json
// LSP Request
{
  "textDocument": { "uri": "file:///mods/steampunk/structures.lex" },
  "position": { "line": 15, "character": 20 }  // Over "steam_engine"
}

// LSP Response
{
  "uri": "file:///mods/steampunk/technologies.lex",
  "range": {
    "start": { "line": 5, "character": 0 },
    "end": { "line": 25, "character": 1 }
  }
}
```

### Integrazione
- **VS Code:** vscode-languageclient
- **Zed:** zed-language-server
- **Neovim:** nvim-lspconfig
- **Custom Rust Editor:** tower-lsp

---

## Target 4: Editor Formats (Rust Editor)

### Output Files

**1. Syntax Highlighting Definition**
```rust
// generated/editor/syntax.rs
pub const LEX_KEYWORDS: &[&str] = &[
    "structure", "unit", "technology", "era", "choice", "ending",
    "event", "secret",
    "name", "cost", "production", "maintenance", "description",
    "requires", "unlocks", "when", "if", "available_if"
];

pub const LEX_OPERATORS: &[&str] = &[
    ":", "=", "{", "}", "[", "]", ",", "->", "+=", "-="
];

pub const LEX_LITERALS: &str = r#"[0-9]+|"[^"]*"|#[0-9A-Fa-f]{6}"#;
```

**2. Autocomplete Database**
```json
// generated/editor/autocomplete.json
{
  "keywords": [
    {
      "word": "structure",
      "snippet": "structure ${1:Name} {\n    era: ${2:Era}\n    cost: { ${3:Resource}: ${4:Amount} }\n    production: { ${5:Resource}: ${6:Amount} }\n}"
    }
  ],
  
  "properties": [
    {
      "name": "era",
      "type": "string",
      "enum": ["Ancient", "Medieval", "Steampunk", "Modern", "Atompunk", "Cyber"],
      "required": true
    },
    {
      "name": "cost",
      "type": "map<string, number>",
      "required": true
    }
  ],
  
  "resources": ["Coal", "Steel", "Gold", "Wood", "Food", "Energy", "Industry"]
}
```

**3. Dependency Graph**
```json
// generated/editor/graph.json
{
  "nodes": [
    {
      "id": "steam_factory",
      "type": "structure",
      "era": "steampunk",
      "file": "mods/steampunk/structures.lex",
      "line": 10
    },
    {
      "id": "steam_engine",
      "type": "technology",
      "era": "steampunk",
      "file": "mods/steampunk/technologies.lex",
      "line": 5
    }
  ],
  
  "edges": [
    {
      "from": "steam_factory",
      "to": "steam_engine",
      "type": "requires",
      "label": "requires technology"
    },
    {
      "from": "steam_factory",
      "to": "steel_foundry",
      "type": "requires",
      "label": "requires structure"
    }
  ]
}
```

**4. Validation Schema**
```json
// generated/editor/validation.json
{
  "version": "0.1.0",
  "rules": [
    {
      "id": "required_era",
      "selector": "structure",
      "check": "has_property('era')",
      "message": "Structure must specify an era",
      "severity": "error"
    },
    {
      "id": "valid_resource",
      "selector": "cost, production, maintenance",
      "check": "all_keys_in(get_valid_resources())",
      "message": "Unknown resource type '{key}'",
      "severity": "error"
    },
    {
      "id": "reference_exists",
      "selector": "requires.technologies[*]",
      "check": "exists_in('technology')",
      "message": "Referenced technology '{ref}' not found",
      "severity": "error"
    }
  ]
}
```

**5. Outline View Data**
```json
// generated/editor/outline.json
{
  "file": "mods/steampunk/structures.lex",
  "symbols": [
    {
      "name": "Steam Factory",
      "kind": "structure",
      "line": 10,
      "children": [
        { "name": "era", "kind": "property", "line": 11 },
        { "name": "cost", "kind": "property", "line": 13 },
        { "name": "production", "kind": "property", "line": 18 }
      ]
    },
    {
      "name": "Coal Refinery",
      "kind": "structure",
      "line": 35,
      "children": [...]
    }
  ]
}
```

### Integrazione con Rust Editor

```rust
// editor/src/lex_integration.rs
use serde::Deserialize;

#[derive(Deserialize)]
pub struct LexAutocomplete {
    pub keywords: Vec<KeywordDef>,
    pub properties: Vec<PropertyDef>,
    pub resources: Vec<String>,
}

#[derive(Deserialize)]
pub struct LexDependencyGraph {
    pub nodes: Vec<Node>,
    pub edges: Vec<Edge>,
}

pub struct LexIntegration {
    autocomplete: LexAutocomplete,
    graph: LexDependencyGraph,
    validator: LexValidator,
}

impl LexIntegration {
    pub fn load_generated_files(path: &Path) -> Result<Self> {
        let autocomplete = serde_json::from_str(&fs::read_to_string(
            path.join("generated/editor/autocomplete.json")
        )?)?;
        
        let graph = serde_json::from_str(&fs::read_to_string(
            path.join("generated/editor/graph.json")
        )?)?;
        
        Ok(Self {
            autocomplete,
            graph,
            validator: LexValidator::load(path)?,
        })
    }
    
    pub fn get_completions(&self, context: &CompletionContext) -> Vec<Completion> {
        // Use autocomplete data
    }
    
    pub fn get_dependency_graph(&self) -> &LexDependencyGraph {
        &self.graph
    }
}
```

---

## Target 5: Tutorial LLM Context

### Scopo
Fornire contesto strutturato a un LLM che aiuta i modder a imparare Lex.

### Output
```json
// generated/tutorial/steampunk_era_tutorial.json
{
  "topic": "Creating Steampunk Era Content",
  "difficulty": "intermediate",
  "prerequisites": ["basic_lex_syntax", "era_system"],
  
  "summary": "
    The Steampunk era (1850-1900) introduces industrial mechanics including:
    - Coal-based energy production
    - Factory maintenance costs
    - Worker happiness and revolution risk
    - Technology dependencies
  ",
  
  "key_concepts": [
    {
      "concept": "Coal Dependency",
      "explanation": "Most Steampunk structures require coal for maintenance",
      "example": "maintenance: { coal: 2, gold: 5 }",
      "common_mistake": "Building factories without coal production"
    },
    {
      "concept": "Technology Tree",
      "explanation": "Technologies unlock structures and other technologies",
      "example": "requires: { technologies: [SteamEngine] }",
      "common_mistake": "Circular dependencies"
    }
  ],
  
  "tutorial_prompts": [
    {
      "user_question_pattern": "how (do I|to) create.*structure",
      "context_needed": ["basic_structure_template", "era_requirements"],
      "example_to_show": "generated/examples/basic_steampunk_structure.lex"
    },
    {
      "user_question_pattern": "my.*not working|error|invalid",
      "context_needed": ["common_errors", "validation_rules"],
      "action": "explain_error_and_fix"
    }
  ],
  
  "common_questions": [
    {
      "question": "Why is my structure not showing up?",
      "answer": "Check: (1) era matches current game era, (2) required technologies researched, (3) required structures built",
      "debug_steps": [
        "Check console for errors",
        "Verify era: property is correct",
        "Check requires: section"
      ]
    }
  ],
  
  "examples": [
    {
      "file": "generated/examples/basic_steampunk_structure.lex",
      "description": "Minimal Steampunk structure",
      "highlights": ["era: Steampunk", "coal maintenance"]
    },
    {
      "file": "generated/examples/advanced_steampunk_structure.lex",
      "description": "Full-featured structure with events",
      "highlights": ["available_if", "secret_if", "archivist"]
    }
  ]
}
```

### Integrazione con LLM

```python
# tutorial_llm/lex_tutor.py
import json

class LexTutor:
    def __init__(self, tutorial_context_path):
        with open(tutorial_context_path) as f:
            self.context = json.load(f)
    
    def get_context_for_question(self, user_question: str) -> str:
        """Build context for LLM prompt"""
        # Find matching tutorial prompts
        for prompt in self.context["tutorial_prompts"]:
            if self._matches_pattern(user_question, prompt["user_question_pattern"]):
                # Build context
                context = f"""
                TOPIC: {self.context['topic']}
                
                SUMMARY: {self.context['summary']}
                
                KEY CONCEPTS:
                {self._format_concepts(prompt['context_needed'])}
                
                EXAMPLE:
                {self._load_example(prompt['example_to_show'])}
                """
                return context
        
        # Default context
        return self._build_default_context()
    
    def explain_error(self, error_message: str) -> str:
        """Generate explanation for common errors"""
        # Match error patterns
        # Return explanation + fix
```

---

## Target 6: Companion LLM Context (In-Game)

### Scopo
Fornire contesto all'Archivista AI durante il gioco.

### Output
```json
// generated/companion/steam_factory_context.json
{
  "entity_id": "steam_factory",
  "entity_type": "structure",
  
  "narrative_context": {
    "historical_period": "1850-1900",
    "themes": ["industrialization", "labor_rights", "environmental_impact"],
    "archivist_personality": "cautious, observant, slightly cynical about progress"
  },
  
  "trigger_conditions": [
    {
      "condition": "first_built",
      "context": "Player just built their first Steam Factory",
      "suggested_responses": [
        {
          "response": "The age of machines has begun. May we not regret it.",
          "tone": "neutral, reflective",
          "timing": "after_construction"
        },
        {
          "response": "I smell coal in the air. Progress has a price.",
          "tone": "slightly ominous",
          "timing": "after_construction"
        }
      ]
    },
    {
      "condition": "built_in_every_city",
      "context": "Player has a Steam Factory in every city",
      "suggested_responses": [
        {
          "response": "Someone is gathering too much power in the factories...",
          "tone": "mysterious, ominous",
          "timing": "after_construction",
          "leads_to": "industrial_conspiracy_event"
        }
      ]
    },
    {
      "condition": "happiness_low_after_factory",
      "context": "Happiness dropped below 40 after building factories",
      "suggested_responses": [
        {
          "response": "The workers grow restless. Consider their welfare, or face consequences.",
          "tone": "cautious, warning",
          "timing": "end_of_turn"
        }
      ]
    }
  ],
  
  "context_for_llm": "
    ENTITY: Steam Factory
    
    ROLE: Critical structure for Steampunk era (1850-1900)
    
    MECHANICS:
    - Converts coal into energy and industry
    - Requires maintenance (2 coal/turn, 5 gold/turn)
    - Unlocks advanced technologies
    
    THEMES: Industrialization, Labor Rights, Environmental Impact
    
    COMPANION PERSONALITY (Archivist):
    - Knowledgeable about history
    - Cautious about rapid industrialization
    - Has seen civilizations rise and fall due to unchecked progress
    - Slightly cynical but helpful
    
    RELEVANT EVENTS:
    - Worker Revolution (at low happiness)
    - Industrial Conspiracy (if factories everywhere)
    
    PLAYER GUIDANCE:
    - Suggest coal production if maintenance failing
    - Warn about happiness drop
    - Comment on industrial progress
  ",
  
  "sample_dialogue": [
    {
      "situation": "Player low on coal",
      "archivist": "Your factories hunger for coal. Perhaps more mines?"
    },
    {
      "situation": "Player building many factories",
      "archivist": "Industry grows, but so does the smoke. I wonder what the future holds."
    },
    {
      "situation": "Worker strike",
      "archivist": "The workers have spoken. Will you listen, or silence them?"
    }
  ]
}
```

### Integrazione con LLM

```cpp
// engine/src/companion/llm_bridge.cpp
class CompanionLLMBridge {
public:
    std::string generateResponse(
        const std::string& situationType,
        const GameState& state,
        const std::string& entityId
    ) {
        // Load context from generated JSON
        auto context = loadCompanionContext(entityId);
        
        // Build prompt for LLM
        std::string prompt = buildPrompt(
            context.context_for_llm,
            situationType,
            state
        );
        
        // Send to local LLM (via bridge)
        std::string response = m_llmBridge->generate(prompt);
        
        // Post-process (ensure tone matches)
        return postProcess(response, context.archivist_personality);
    }
    
private:
    json loadCompanionContext(const std::string& entityId) {
        std::string path = fmt::format(
            "generated/companion/{}_context.json",
            entityId
        );
        return json::parse(std::ifstream(path));
    }
};
```

---

## Compatibilità con Altri Sistemi

### Principio di Design

Lex è **engine-agnostic** per definizione:

```
┌─────────────────────────────────────┐
│          Lex Source (.lex)          │
└───────────────┬─────────────────────┘
                │
                ▼
┌─────────────────────────────────────┐
│        Standard Output Formats      │
├─────────────────────────────────────┤
│ • Lua 5.4 (standard)                │
│ • JSON (RFC 8259)                   │
│ • LSP Protocol (standard)           │
│ • GraphQL Schema (optional)         │
│ • Protocol Buffers (optional)       │
└───────────────┬─────────────────────┘
                │
                ├─────────────────────┐
                │                     │
                ▼                     ▼
        ┌──────────────┐      ┌──────────────┐
        │  Imperium    │      │   Any Other  │
        │  Engine      │      │   Engine     │
        │  (C++/Lua)   │      │  (Rust/Go/JS)│
        └──────────────┘      └──────────────┘
```

### Motori Supportati

| Engine | Linguaggio | Integrazione | Output Usato |
|--------|------------|--------------|--------------|
| **Imperium** | C++ | LuaJIT + sol2 | Lua + JSON |
| **Unity** | C# | NLua | Lua + JSON |
| **Godot** | GDScript/C# | LuaScript | Lua + JSON |
| **Bevy** | Rust | mlua | Lua + JSON |
| **Unreal** | C++ | UnLua | Lua + JSON |
| **Custom** | Any | Standard Lua | Lua + JSON |

### Protocolli Standard

**1. LSP (Language Server Protocol)**
- Standard Microsoft
- Supportato da: VS Code, Zed, Neovim, Emacs, etc.

**2. JSON Schema**
- Standard IETF
- Validazione universale

**3. GraphQL (Optional)**
- Per API moderne
- Query runtime

---

## Integrazione con Rust Editor

### Stack Tecnologico

```
┌─────────────────────────────────────┐
│      Rust Editor (Imperium)         │
├─────────────────────────────────────┤
│ • egui (UI)                         │
│ • tree-sitter (parsing)             │
│ • tower-lsp (LSP server/client)     │
│ • serde_json (JSON handling)        │
│ • petgraph (dependency graph)       │
└───────────────┬─────────────────────┘
                │
                ▼
┌─────────────────────────────────────┐
│     Lex Generated Artifacts         │
├─────────────────────────────────────┤
│ • autocomplete.json                 │
│ • graph.json (dependencies)         │
│ • validation.json                   │
│ • outline.json                      │
│ • schemas/*.schema.json             │
└─────────────────────────────────────┘
```

### Feature Editor

**1. Syntax Highlighting**
- tree-sitter grammar generata da Lex
- Error highlighting

**2. Autocomplete**
- Keywords
- Properties
- Resource names
- Era names
- Cross-file references

**3. Dependency Graph View**
- Visualizzazione grafo
- Filtri per era/tipo
- Click to navigate

**4. Real-time Validation**
- Schema validation
- Reference checking
- Semantic analysis

**5. Refactoring**
- Rename symbol (all files)
- Extract structure
- Inline reference

---

## Esempio: Integrazione Completa

### 1. Modder Crea File

```lex
// mods/my_mod/structures.lex
structure PowerPlant {
    era: Modern
    name: "Nuclear Power Plant"
    
    cost: { Uranium: 5, Steel: 20, Gold: 200 }
    production: { Energy: 50 }
    maintenance: { Uranium: 1, Gold: 20 }
    
    available_if: {
        condition: civilization.has_technology(NuclearPhysics)
    }
    
    on_built: {
        flag: flag_nuclear_age
        archivist: "The atom splits, and so might our future."
    }
}
```

### 2. Lex Compiler Genera

```
generated/
├── lua/
│   └── structures/power_plant.lua      ← Imperium Engine
├── data/
│   └── structures/power_plant.json     ← Any Engine
├── editor/
│   ├── autocomplete.json               ← Rust Editor
│   ├── graph.json                      ← Dependency graph
│   └── outline.json                    ← Tree view
├── tutorial/
│   └── modern_era_tutorial.json        ← Tutorial LLM
└── companion/
    └── power_plant_context.json        ← Companion LLM
```

### 3. Sistemi Consumano

**Imperium Engine (C++):**
```cpp
// Carica struttura automaticamente
auto structure = LuaEngine::loadStructure("power_plant");
```

**Rust Editor:**
```rust
// Mostra autocomplete
let completions = lex_integration.get_completions(ctx);
```

**Tutorial LLM:**
```python
# Aiuta modder
tutor.explain_structure("power_plant")
```

**Companion LLM:**
```cpp
// Genera risposta Archivista
archivist.react_to("power_plant_built", gameState);
```

---

## Roadmap Integrazione

### Fase 1: Core Output (MVP)
- [ ] Lua backend
- [ ] JSON backend
- [ ] Basic schema validation

### Fase 2: Tool Support
- [ ] LSP server
- [ ] Editor autocomplete data
- [ ] Dependency graph

### Fase 3: AI Integration
- [ ] Tutorial context generator
- [ ] Companion context generator
- [ ] Error explanation templates

### Fase 4: Advanced Features
- [ ] Hot-reload
- [ ] Incremental compilation
- [ ] Watch mode

---

## Note Tecniche

### Performance

**Compilation Time:**
- Target: <100ms per file (1000 lines)
- Target: <1s per mod completo (100 files)

**Output Size:**
- Lua: ~1.5x source size
- JSON: ~2x source size
- Editor data: ~0.5x source size

### Caching

```rust
// Incremental compilation
struct LexCache {
    source_hash: u64,
    ast_hash: u64,
    last_modified: SystemTime,
}

fn compile_incremental(path: &Path, cache: &LexCache) -> Result<()> {
    let current_hash = hash_file(path)?;
    
    if current_hash == cache.source_hash {
        return Ok(());  // Skip recompilation
    }
    
    // Recompile
    let ast = parse_file(path)?;
    generate_all_outputs(&ast)?;
    
    // Update cache
    cache.source_hash = current_hash;
    cache.ast_hash = hash_ast(&ast);
}
```

---

*Ultimo aggiornamento: 2026-03-01*
