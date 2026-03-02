# Lex Aether Extended — RFC

**Version:** 0.1.0-draft
**Date:** 2026-03-02
**Status:** Draft - Community Feedback Needed
**Author:** David
**Target:** Lex v0.3.0

---

## Overview

Aether Extended espande il sistema Aether esistente per supportare **agenti AI multipli** oltre ai LLM del gioco. Lo stesso file `.lex` può generare:

1. **Game Runtime Data** (Lua, JSON) - per il motore di gioco
2. **Game AI Context** (Markdown) - per Archivista e God AI
3. **Developer AI Context** (Markdown/JSON) - per agenti di sviluppo (Droid, Cursor, Claude)
4. **Documentation** (Markdown) - per umani

---

## Motivation

### Current State

Aether attualmente supporta solo:
- **Archivista** - Companion narrativo per il giocatore
- **God AI** - Sistema di osservazione per sviluppatori

Ma manca:
- Contesto per **agenti di sviluppo** (Droid, Cursor, Claude)
- **Documentazione automatica** dal codice
- **Onboarding** per nuovi sviluppatori
- **Tracciabilità** tra design e implementazione

### The Problem

```
┌─────────────────────────────────────────────────────────────┐
│                    CURRENT WORKFLOW                          │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│   Game Designer ──► docs/design/*.md ──► Developer          │
│                              │                               │
│                              ▼                               │
│                         IMPLEMENTAZIONE                      │
│                              │                               │
│                              ▼                               │
│         ┌────────────────────┴────────────────────┐         │
│         │                                         │         │
│    data/*.json                              scripts/*.lua   │
│         │                                         │         │
│         └────────────────────┬────────────────────┘         │
│                              │                               │
│                              ▼                               │
│                        OUT OF SYNC!                          │
│                                                              │
│   Design docs ≠ JSON data ≠ Lua scripts ≠ AI context        │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### The Solution

```
┌─────────────────────────────────────────────────────────────┐
│                    LEX AETHER WORKFLOW                       │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│   game_data.lex ──► Lex Compiler ──┬──► data/*.json        │
│                                    ├──► scripts/*.lua       │
│                                    ├──► aether/game_ai.md   │
│                                    ├──► aether/dev_ai.md    │
│                                    └──► docs/generated/*.md │
│                                                              │
│   SINGLE SOURCE OF TRUTH                                     │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## Proposed Syntax

### Extended Definition with AI Context

```lex
era Steampunk {
    // ========================================
    // GAME DATA (existing)
    // ========================================
    name: "Steampunk Era"
    period: "1850-1920"
    description: "The age of steam, mechanical engineering, and industrial revolution."
    
    atmosphere: "industrial, foggy, mechanical"
    music: "steampunk_ost"
    dominant_color: #8B6914
    
    // ========================================
    // AI CONTEXT (new)
    // ========================================
    
    // Context for Game AI (Archivista)
    narrative: {
        themes: ["progress", "pollution", "class struggle", "industrialization"]
        mood: "tense, industrial, hopeful"
        key_events: [
            "Invention of the steam engine",
            "Urban migration from countryside",
            "Rise of factory system",
            "Labor movements emerge"
        ]
        
        // Dialogue prompts for Archivista
        dialogue_hints: {
            era_enter: "The smog clears to reveal a landscape of iron and steam..."
            technology_discover: "The {technology_name} will change everything."
            crisis: "The workers grow restless. The factories never sleep."
        }
        
        // Memory priorities (what Archivista remembers)
        memory_priority: high
        always_remember: ["era_name", "key_technologies", "major_events"]
    }
    
    // Context for Developer AI (Droid, Cursor, Claude, etc.)
    developer: {
        // Architecture notes
        architecture_notes: """
            Era transition triggers:
            1. Shader variant switch (fog density changes)
            2. UI theme update (copper/brass colors)
            3. Audio layer change (mechanical ambience)
            
            Dependencies: RenderGraph, UIManager, AudioManager
        """
        
        // Implementation hints
        implementation_hints: {
            shader_variants: ["steampunk_fog", "industrial_lighting"]
            ui_theme: "copper"
            performance_risks: ["Particle effects on low-end GPUs"]
        }
        
        // Related code locations
        code_references: [
            "engine/src/Gameplay/EraManager.cpp",
            "engine/src/Graphics/EraShaderVariants.cpp",
            "assets/shaders/era_specific/"
        ]
        
        // Testing notes
        test_coverage: {
            required: ["era_transition", "resource_unlocks", "technology_tree"]
            edge_cases: ["era_skip_via_cheat", "save_load_during_transition"]
        }
    }
    
    // Context for Documentation
    documentation: {
        summary: "Second era in Imperium, focused on industrial development."
        
        player_guide: """
            ## Steampunk Era
            
            Focus on:
            - Building factories for production
            - Researching steam technologies
            - Managing worker happiness
            - Preparing for the Modern Era
        """
        
        modding_guide: """
            To add Steampunk content:
            1. Create structure/unit/tech definitions in `.lex` files
            2. Set `era: Steampunk` property
            3. Run `lexc your_content.lex -o data/`
        """
    }
}
```

### Structure with AI Context

```lex
structure SteamFactory {
    // Game Data
    era: Steampunk
    name: "Steam Factory"
    
    cost: { Coal: 8, Steel: 5, Gold: 50 }
    production: { Energy: 15, Industry: 10 }
    maintenance: { Coal: 2, Gold: 5 }
    
    description: "Converts coal into energy and industrial output."
    
    // AI Context
    narrative: {
        visual_description: "A sprawling complex of brass pipes and smokestacks"
        ambient_sounds: ["hissing_steam", "grinding_gears", "distant_whistle"]
        worker_comments: [
            "Another day in the furnaces...",
            "The machines never stop.",
            "Watch your fingers!"
        ]
    }
    
    developer: {
        implementation_notes: """
            Building uses:
            - PBR material with metallic=0.8, roughness=0.4
            - Particle system for smoke (configurable density)
            - Point lights for furnace glow
        """
        
        performance_hints: {
            max_per_city: 5  // More than 5 impacts frame rate
            lod_distance: 100
            culling: "occlusion_culling_enabled"
        }
        
        dependencies: ["CoalResource", "SteelResource", "EnergyResource"]
    }
}
```

---

## Generated Output

### File Structure

```
output/
├── game/
│   ├── data/
│   │   ├── eras.json
│   │   └── structures.json
│   └── scripts/
│       └── generated/
│           └── buildings.lua
│
├── aether/
│   ├── game_ai/
│   │   ├── eras/
│   │   │   └── steampunk.md
│   │   └── structures/
│   │       └── steam_factory.md
│   │
│   └── dev_ai/
│       ├── context.json          # Machine-readable for agents
│       └── architecture.md       # Human-readable overview
│
└── docs/
    ├── generated/
    │   ├── player_guide.md
    │   └── modding_guide.md
    └── schema/
        └── validation_rules.json
```

### Game AI Context (for Archivista)

```markdown
# Steampunk Era — Game AI Context

## Quick Reference

| Property | Value |
|----------|-------|
| Name | Steampunk Era |
| Period | 1850-1920 |
| Mood | tense, industrial, hopeful |

## Themes

- Progress
- Pollution
- Class struggle
- Industrialization

## Key Events

1. Invention of the steam engine
2. Urban migration from countryside
3. Rise of factory system
4. Labor movements emerge

## Dialogue Templates

### Era Enter
> "The smog clears to reveal a landscape of iron and steam..."

### Technology Discovery
> "The {technology_name} will change everything."

### Crisis
> "The workers grow restless. The factories never sleep."

## Memory Configuration

- **Priority:** High
- **Always Remember:** era_name, key_technologies, major_events
- **Forget After:** 10 turns (routine comments)
```

### Developer AI Context (for Droid/Cursor/Claude)

```json
{
  "version": "1.0",
  "generated_by": "Lex Compiler v0.3.0",
  "source_file": "game_data.lex",
  
  "project_overview": {
    "name": "Imperium 2.0",
    "type": "4X Strategy Game",
    "engine": "C++ + Vulkan",
    "scripting": "Lua"
  },
  
  "definitions": {
    "eras": {
      "Steampunk": {
        "class": "EraDefinition",
        "dependencies": ["RenderGraph", "UIManager", "AudioManager"],
        "code_references": [
          "engine/src/Gameplay/EraManager.cpp",
          "engine/src/Graphics/EraShaderVariants.cpp"
        ],
        "implementation_hints": {
          "shader_variants": ["steampunk_fog", "industrial_lighting"],
          "ui_theme": "copper",
          "performance_risks": ["Particle effects on low-end GPUs"]
        },
        "test_coverage": {
          "required": ["era_transition", "resource_unlocks"],
          "edge_cases": ["era_skip_via_cheat"]
        }
      }
    },
    
    "structures": {
      "SteamFactory": {
        "class": "StructureDefinition",
        "dependencies": ["CoalResource", "SteelResource", "EnergyResource"],
        "implementation_notes": "PBR material with metallic=0.8, roughness=0.4",
        "performance_hints": {
          "max_per_city": 5,
          "lod_distance": 100
        }
      }
    }
  },
  
  "architecture_notes": [
    {
      "topic": "Era System",
      "description": "Era transition triggers shader variant switch, UI theme update, audio layer change",
      "files": ["engine/src/Gameplay/EraManager.cpp"]
    }
  ],
  
  "dependencies_graph": {
    "Steampunk": ["RenderGraph", "UIManager", "AudioManager"],
    "SteamFactory": ["CoalResource", "SteelResource", "EnergyResource"]
  }
}
```

### Documentation Output

```markdown
# Imperium 2.0 — Player Guide

Generated by Lex Compiler v0.3.0

## Eras

### Steampunk Era (1850-1920)

The age of steam, mechanical engineering, and industrial revolution.

**Focus on:**
- Building factories for production
- Researching steam technologies
- Managing worker happiness
- Preparing for the Modern Era

## Structures

### Steam Factory

Converts coal into energy and industrial output.

**Cost:**
- Coal: 8
- Steel: 5
- Gold: 50

**Production:**
- Energy: 15
- Industry: 10

**Maintenance:**
- Coal: 2
- Gold: 5
```

---

## Architecture

### Multi-Agent Context Generation

```
┌─────────────────────────────────────────────────────────────┐
│                    LEX AETHER ARCHITECTURE                   │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│   .lex file                                                  │
│       │                                                      │
│       ▼                                                      │
│   ┌─────────────────────────────────────────────────────┐   │
│   │                   Lex Compiler                       │   │
│   │                                                     │   │
│   │  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐  │   │
│   │  │   Lexer     │→ │   Parser    │→ │     AST     │  │   │
│   │  └─────────────┘  └─────────────┘  └─────────────┘  │   │
│   │                                            │         │   │
│   │  ┌─────────────────────────────────────────▼───────┐ │   │
│   │  │              Aether Context Builder             │ │   │
│   │  │                                                  │ │   │
│   │  │  ┌──────────────┐  ┌──────────────┐            │ │   │
│   │  │  │   Game AI    │  │   Dev AI     │            │ │   │
│   │  │  │   Builder    │  │   Builder    │            │ │   │
│   │  │  └──────────────┘  └──────────────┘            │ │   │
│   │  │                                                  │ │   │
│   │  │  ┌──────────────┐  ┌──────────────┐            │ │   │
│   │  │  │    Docs      │  │   Validator  │            │ │   │
│   │  │  │   Builder    │  │   Generator  │            │ │   │
│   │  │  └──────────────┘  └──────────────┘            │ │   │
│   │  └──────────────────────────────────────────────────┘ │   │
│   └─────────────────────────────────────────────────────┘   │
│                               │                              │
│                               ▼                              │
│   ┌─────────────────────────────────────────────────────┐   │
│   │                   Output Targets                     │   │
│   │                                                     │   │
│   │   game/data/*.json     → Engine runtime             │   │
│   │   aether/game_ai/*.md  → Archivista + God AI        │   │
│   │   aether/dev_ai/*.json → Droid, Cursor, Claude      │   │
│   │   docs/generated/*.md  → Human developers           │   │
│   │                                                     │   │
│   └─────────────────────────────────────────────────────┘   │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Agent-Specific Context

| Agent Type | Input | Output Format | Use Case |
|------------|-------|---------------|----------|
| **Archivista** | `narrative:` block | Markdown | In-game companion |
| **God AI** | `narrative:` + metrics | Markdown | Developer reports |
| **Droid** | `developer:` block | JSON + Markdown | Code assistance |
| **Cursor** | `developer:` block | JSON | Code completion |
| **Claude** | `developer:` block | JSON + Markdown | Architecture advice |
| **Human** | `documentation:` block | Markdown | Player/modder guides |

---

## Implementation Plan

### Phase 1: Core Infrastructure (v0.3.0)

1. **AST Extensions**
   - `NarrativeBlock` - Game AI context
   - `DeveloperBlock` - Dev AI context
   - `DocumentationBlock` - Human docs

2. **Aether Backend**
   - `AetherBackend` base class
   - `GameAiBuilder` - Generates narrative context
   - `DevAiBuilder` - Generates developer context
   - `DocsBuilder` - Generates documentation

3. **CLI Integration**
   ```bash
   lexc game.lex -o output/ -t json,lua,aether,docs
   ```

### Phase 2: Agent Integration (v0.4.0)

1. **Droid Integration**
   - Auto-generate `.factory/context/` files
   - Update on `.lex` file changes
   - Include in RAG index

2. **Cursor Integration**
   - Generate `.cursorrules` from `developer:` blocks
   - Context for code completion

3. **Claude Integration**
   - Generate `CLAUDE.md` with project context
   - Architecture decision records

### Phase 3: Live Updates (v0.5.0)

1. **Watch Mode**
   ```bash
   lexc --watch game.lex -o output/
   ```

2. **Hot Context Update**
   - Dev agents auto-refresh context
   - Game AI updates without restart

---

## Open Questions

1. **Context Size**: How much context is too much?
   - Option A: Summarize long content automatically
   - option B: Let user configure verbosity

2. **Language**: Support multilingual context?
   - `narrative_it:`, `narrative_en:` blocks?

3. **Versioning**: How to handle context versioning?
   - Include in output metadata?
   - Track changes over time?

4. **Security**: Any sensitive data concerns?
   - Filter out `developer:` blocks from public builds?

---

## Success Criteria

| Criterion | Metric |
|-----------|--------|
| Dev Agent Accuracy | 90%+ relevant code suggestions |
| Onboarding Time | < 30 min to productive contribution |
| Documentation Sync | 100% match with implementation |
| Game AI Quality | Player satisfaction > 4/5 |

---

## Risks

| Risk | Mitigation |
|------|------------|
| Context bloat | Summarization, configurable verbosity |
| Stale context | Watch mode, file timestamps |
| Agent confusion | Clear structure, validation |
| Privacy | Filter blocks per target |

---

**Next Steps:**

1. Implement `narrative:`, `developer:`, `documentation:` blocks in parser
2. Create `AetherBackend` class
3. Test with Droid agent integration
4. Document best practices for context writing
