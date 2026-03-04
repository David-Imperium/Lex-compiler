# AI Context Generator — Design Document

**Version:** 1.0
**Status:** Draft
**Priority:** Core Feature
**Tier:** Free (Lex Base)

---

## 1. Problem Statement

### 1.1 The Context Problem

AI agents (including LLMs like Claude, GPT, etc.) struggle with game codebases because:

| Challenge | Traditional Code | Lex |
|-----------|------------------|-----|
| **Scattered data** | 100+ files, mixed with logic | Single .lex file |
| **Technical noise** | OOP, inheritance, patterns | Pure declarations |
| **Implicit meaning** | Variable names, comments | Explicit schema |
| **Context window** | Too much irrelevant code | Minimal, focused |

### 1.2 The Solution

**Lex AI Context Generator** transforms `.lex` files into structured context that AI agents can understand instantly.

```
.lex files → Context Generator → AI-ready context (JSON/Markdown)
```

---

## 2. Design Goals

### 2.1 Core Principles

1. **Human-First, AI-Second**: Lex remains readable by humans
2. **Zero Configuration**: Works out of the box
3. **Schema-Aware**: Understands game semantics, not just syntax
4. **Query-Ready**: Supports natural language queries

### 2.2 Non-Goals

- Not replacing game code generation
- Not a runtime system
- Not a database

---

## 3. Output Formats

### 3.1 JSON Context

**Input:**
```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
    requires: [Agriculture]
}

technology Agriculture {
    era: Ancient
    cost: { Research: 100 }
    unlocks: [Farm, Granary]
}
```

**Output:**
```json
{
  "schema": {
    "types": ["structure", "technology"],
    "version": "0.4.0"
  },
  "entities": [
    {
      "id": "Farm",
      "type": "structure",
      "summary": "A food production building from the Ancient era",
      "properties": {
        "era": "Ancient",
        "cost": {"Gold": 50},
        "production": {"Food": 5}
      },
      "references": ["Agriculture"],
      "referenced_by": [],
      "dependencies": ["Agriculture"],
      "dependents": []
    },
    {
      "id": "Agriculture",
      "type": "technology",
      "summary": "Ancient era technology that unlocks Farm and Granary",
      "properties": {
        "era": "Ancient",
        "cost": {"Research": 100}
      },
      "references": ["Farm", "Granary"],
      "referenced_by": ["Farm"],
      "dependencies": [],
      "dependents": ["Farm", "Granary"]
    }
  ],
  "graph": {
    "nodes": ["Farm", "Agriculture"],
    "edges": [
      {"from": "Farm", "to": "Agriculture", "type": "requires"},
      {"from": "Agriculture", "to": "Farm", "type": "unlocks"}
    ]
  },
  "statistics": {
    "total_entities": 2,
    "by_type": {"structure": 1, "technology": 1},
    "by_era": {"Ancient": 2}
  }
}
```

### 3.2 Markdown Context

**Output:**
```markdown
# Game Data Context

## Overview
- **Total Entities**: 2
- **Types**: structure (1), technology (1)
- **Eras**: Ancient (2)

## Structures

### Farm
> A food production building from the Ancient era

| Property | Value |
|----------|-------|
| Era | Ancient |
| Cost | Gold: 50 |
| Production | Food: 5 |
| Requires | Agriculture |

**Dependencies**: Requires Agriculture technology

---

## Technologies

### Agriculture
> Ancient era technology that unlocks Farm and Granary

| Property | Value |
|----------|-------|
| Era | Ancient |
| Cost | Research: 100 |
| Unlocks | Farm, Granary |

**Unlocks**: Farm, Granary

---

## Dependency Graph

```
Agriculture → Farm
           → Granary
```

## Queries

- "What does Farm require?" → Agriculture technology
- "What does Agriculture unlock?" → Farm, Granary
- "What costs Gold?" → Farm (50)
```

### 3.3 Minimal Context (for LLM prompts)

**Output:**
```
STRUCTURES:
Farm: Ancient era, costs 50 Gold, produces 5 Food, requires Agriculture

TECHNOLOGIES:
Agriculture: Ancient era, costs 100 Research, unlocks Farm and Granary

DEPENDENCIES:
Farm requires Agriculture
Agriculture unlocks Farm, Granary
```

---

## 4. API Design

### 4.1 CLI

```bash
# Generate JSON context
lexc game.lex --context json -o context.json

# Generate Markdown context
lexc game.lex --context md -o context.md

# Generate minimal context (for LLM prompts)
lexc game.lex --context minimal -o context.txt

# Generate all formats
lexc game.lex --context all -o context/
```

### 4.2 Library API

```cpp
#include <lex/lex.hpp>

// Generate context
lex::ContextOptions options;
options.format = lex::ContextFormat::JSON;
options.include_graph = true;
options.include_statistics = true;

lex::ContextResult result = lex::generate_context("game.lex", options);

// Access structured data
for (const auto& entity : result.entities) {
    std::cout << entity.id << ": " << entity.summary << "\n";
}

// Get dependency graph
for (const auto& edge : result.graph.edges) {
    std::cout << edge.from << " -> " << edge.to << "\n";
}
```

### 4.3 Query API

```cpp
// Natural language queries
lex::QueryResult query(const std::string& question, const ContextResult& context);

// Examples:
// "What does Farm require?" → {answer: "Agriculture technology", entities: ["Agriculture"]}
// "What costs Gold?" → {answer: "Farm costs 50 Gold", entities: ["Farm"]}
// "Show the dependency tree for Farm" → {graph: [...]}
```

---

## 5. Context Structure

### 5.1 Entity Context

```cpp
struct EntityContext {
    std::string id;                    // "Farm"
    std::string type;                  // "structure"
    std::string summary;                // AI-generated summary
    std::map<std::string, Value> properties;
    std::vector<std::string> references;     // What this entity references
    std::vector<std::string> referenced_by;   // What references this entity
    std::vector<std::string> dependencies;    // What this needs
    std::vector<std::string> dependents;      // What needs this
};
```

### 5.2 Graph Context

```cpp
struct GraphContext {
    std::vector<std::string> nodes;
    std::vector<Edge> edges;
    
    struct Edge {
        std::string from;
        std::string to;
        std::string type;  // "requires", "unlocks", "produces", etc.
    };
};
```

### 5.3 Statistics Context

```cpp
struct StatisticsContext {
    size_t total_entities;
    std::map<std::string, size_t> by_type;
    std::map<std::string, size_t> by_era;
    std::map<std::string, size_t> by_property;
};
```

---

## 6. Summary Generation

### 6.1 Template-Based (Fast, Deterministic)

```cpp
std::string generate_summary(const EntityContext& entity) {
    std::string type = entity.type;
    std::string era = get_property(entity, "era", "Unknown era");
    
    if (type == "structure") {
        auto production = get_resource_map(entity, "production");
        return fmt::format("A {} that produces {} in the {} era",
            type, format_resources(production), era);
    }
    if (type == "technology") {
        auto unlocks = get_reference_list(entity, "unlocks");
        return fmt::format("A {} era technology that unlocks {}",
            era, format_list(unlocks));
    }
    // ... more templates
}
```

### 6.2 LLM-Enhanced (Optional, Better Quality)

```cpp
// For users who want AI-enhanced summaries
std::string generate_summary_llm(const EntityContext& entity) {
    std::string prompt = fmt::format(
        "Summarize this game entity in one sentence:\n"
        "Type: {}\nProperties: {}\n",
        entity.type, format_properties(entity.properties)
    );
    return llm_client.complete(prompt);
}
```

---

## 7. Use Cases

### 7.1 For Game Developers

```bash
# Generate documentation
lexc game.lex --context md -o docs/game_data.md

# Analyze balance
lexc game.lex --context json | python analyze_balance.py
```

### 7.2 For AI Agents

```python
# Python integration
import json
from lex import Context

context = Context.from_file("game.lex")

# Ask questions
answer = context.query("What is the cheapest structure?")
# → "Farm costs 50 Gold"

# Get all dependencies
deps = context.get_dependencies("Farm")
# → ["Agriculture"]
```

### 7.3 For Modders

```bash
# Understand game data before modding
lexc game.lex --context md -o game_data.md

# Validate mod doesn't break dependencies
lexc mod.lex --context json | python check_deps.py
```

---

## 8. Implementation Plan

### Phase 1: Core Context Generation (1-2 days)

| Task | Description |
|------|-------------|
| ContextOptions struct | Configuration for context generation |
| ContextResult struct | Output structure |
| Entity summarization | Template-based summaries |
| JSON output | `--context json` |
| Markdown output | `--context md` |

### Phase 2: Graph & Statistics (1 day)

| Task | Description |
|------|-------------|
| Dependency graph | Build reference graph |
| Statistics | Count entities by type/era |
| Minimal output | `--context minimal` |

### Phase 3: Query System (2-3 days)

| Task | Description |
|------|-------------|
| Query parser | Parse natural language questions |
| Query executor | Find answers in context |
| Query API | C++ and CLI interface |

---

## 9. Integration with Lex Layers

### 9.1 Lex Base (Free)

- ✅ JSON context generation
- ✅ Markdown context generation
- ✅ Minimal context for LLMs
- ✅ Basic query system

### 9.2 Lex Core ($29/mo)

- ✅ Everything in Base
- ✅ Advanced graph analysis
- ✅ Balance reports
- ✅ Dependency visualization

### 9.3 Lex Aether ($149/mo)

- ✅ Everything in Core
- ✅ LLM-enhanced summaries
- ✅ Natural language queries
- ✅ Agent integration API
- ✅ Real-time context sync

---

## 10. Examples

### 10.1 Simple Query

```bash
$ lexc game.lex --query "What does Farm require?"
Farm requires: Agriculture technology

$ lexc game.lex --query "What costs the most?"
Market costs: 200 Gold (highest Gold cost)
```

### 10.2 Complex Query

```bash
$ lexc game.lex --query "Show me the technology tree"
Ancient Era:
  Agriculture → Farm, Granary
  Mining → Mine, Quarry

Medieval Era:
  Engineering → Workshop, Mill
  Steel Working → Forge, Armory
```

### 10.3 LLM Integration

```python
# Use with Claude/GPT
import openai
from lex import Context

context = Context.from_file("game.lex")

prompt = f"""
Game Data:
{context.to_minimal()}

Question: What strategies work well in the Ancient era?
"""

response = openai.ChatCompletion.create(
    model="gpt-4",
    messages=[{"role": "user", "content": prompt}]
)
```

---

## 11. Success Metrics

| Metric | Target |
|--------|--------|
| Context generation time | <100ms for 1000 entities |
| JSON output size | <10KB per 100 entities |
| Query response time | <10ms |
| LLM context reduction | 90%+ vs raw code |

---

## 12. Conclusion

The AI Context Generator is a **core feature** of Lex, not an add-on. It transforms the fundamental advantage of Lex (declarative, structured data) into a direct benefit for AI agents.

**Key Insight:** While traditional game code requires AI to parse, understand, and filter thousands of lines of implementation, Lex provides the semantic context directly.

This makes Lex the **natural choice** for:
- Game developers using AI assistants
- AI agents working with game data
- Modders understanding game systems
- Teams documenting their games

---

**Next Steps:**
1. Implement Phase 1 (Core Context Generation)
2. Add to Lex Base (free tier)
3. Document API for agent integration