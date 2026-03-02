# Lex Development Roadmap

**Version:** 0.3.0
**Last Updated:** 2026-03-02

---

## Current Status: v0.2.0 (MVP Complete)

### Completed
| Component | Status | Notes |
|-----------|--------|-------|
| Lexer | Done | 50+ keywords, full literal support |
| Parser | Done | 10 definition types, conditions, error recovery, `{}` resource maps |
| AST | Done | Type-safe, visitor pattern |
| Validator | Done | Semantic analysis with errors/warnings separation |
| Lua Backend | Done | Code generation functional |
| JSON Backend | Done | Imperium engine integration |
| CLI | Done | Multi-target support (`-t lua,json`) |
| Tests | Active | 17+ test cases with Catch2 |

### RFCs Published
| RFC | Status | Description |
|-----|--------|-------------|
| `LEX_SHADER_RFC.md` | Draft | Shader DSL for GLSL/HLSL/MSL generation |
| `LEX_AETHER_RFC.md` | Draft | Multi-agent AI context generation |
| `AETHER_DEBUG_RFC.md` | Draft | Debug protocol for AI-assisted crash analysis |

### Latest Commit
`055690f` - feat: Add JSON backend, CLI multi-target, and semantic validator

---

## Problemi Critici del Core da Risolvere

Prima di costruire i layer avanzati, il compilatore core ha bisogni critici.

### Expression Parser (Priorità Critica)
Il parser attuale raccoglie le condizioni come stringhe grezze:
```cpp
// Attuale — non funziona davvero
while (!check(TokenType::LEFT_BRACE)) {
    expr_stream << current().lexeme << " ";
}
```

Serve un expression parser vero con precedenza degli operatori:
- Confronti: `>`, `<`, `>=`, `<=`, `==`, `!=`
- Logica: `and`, `or`, `not`
- Aritmetica: `+`, `-`, `*`, `/`
- Riferimenti: `civilization.population`, `city.happiness`
- Chiamate: `has_technology(SteamEngine)`

### AST Tipizzato (Priorità Alta)
L'AST attuale è generico — ogni definizione è `Definition` con `vector<Property>`. I backend fanno ricerche lineari su ogni accesso. Serve un AST dove `StructureDefinition` ha campi tipizzati espliciti.

### Type Checker (Priorità Alta)
Nessun controllo di tipo tra proprietà. `era: 123` passa senza errori. Serve un phase di type checking che validi i tipi prima della generazione.

### Lua Backend Completo (Priorità Media)
Il backend Lua attuale genera solo strutture. Non genera codice per condizioni, eventi, o logica. Serve che generi codice eseguibile da condizioni reali.

---

## Piano di Sviluppo

### Fase 1 — Core Solido (Prima di tutto)
1. Expression parser con precedenza operatori e test
2. AST tipizzato per i tipi base (structure, unit, technology, era)
3. Type checker base
4. Lua backend completo con generazione da condizioni reali
5. Test end-to-end su un mod Imperium reale

**Criterio di successo:** un modder può scrivere un file `.lex` con condizioni e ottenere Lua che gira in Imperium.

### Fase 2 — Lex Core
1. Definizione formale della sintassi Core (superset di Base)
2. Backend C++ — generazione di bindings engine
3. Backend Rust — interfacce editor
4. Sistema di tipi avanzato (variabili, espressioni, funzioni)
5. Integrazione con sol2 per esposizione al layer Lua

**Criterio di successo:** puoi definire un sistema di gioco in Lex Core e l'engine lo carica senza scrivere C++ a mano.

### Fase 3 — Lex Shader
1. Definizione sintassi Shader (material, pass, parameters)
2. Backend RenderGraph — genera chiamate API al rendergraph esistente
3. Backend GLSL — genera shader code da definizioni dichiarative
4. Sistema LOD dichiarativo
5. Integrazione con il pipeline Vulkan

**Criterio di successo:** puoi definire un materiale in Lex Shader e vederlo renderizzato in Imperium.

### Fase 4 — Lex Neural
1. Design API C++ Neural (pensato per Lex fin dall'inizio)
2. Implementazione backend Nvidia DLSS
3. Implementazione backend Intel XeSS
4. Implementazione backend AMD FSR
5. Sistema di fallback automatico
6. Backend Lex Neural — genera codice C++ con selezione runtime
7. Integrazione con il RenderGraph per input/output buffer

**Criterio di successo:** scrivi `neural Upscaler { mode: quality }` e funziona su qualsiasi GPU supportata, con fallback su hardware legacy.

### Fase 5 — Aether
1. Generazione automatica contesto Aether dal compilatore Lex
2. Bridge Python per Archivista (Ollama locale)
3. Bridge Python per God AI (monitoring e report)
4. Sistema di memoria selettiva per Archivista
5. Sistema di visione opzionale
6. Report engine per God AI

**Criterio di successo:** l'Archivista reagisce alle scelte del giocatore con contesto reale della partita. God AI genera report leggibili sulle sessioni.

---

## Short Term: v0.2.1 (Target: 2026-03-09)

### Goals
Enhanced developer experience and additional backends.

### Tasks
1. **React/TypeScript Backend** (`src/codegen/react_backend.cpp`)
   - Generate TypeScript interfaces from definitions
   - React component props for UI panels
   - Type-safe game data consumption

2. **Lore Backend** (`src/codegen/lore_backend.cpp`)
   - AI context generation for narrative systems
   - Markdown/structured text output
   - Companion AI integration

### Success Criteria
```bash
# Multi-target output:
lexc game.lex -o output/ -t lua,json,react
# Produces: output/*.lua, output/*.json, output/*.ts
```

---

## Medium Term: v0.3.0 — Revolutionary Features (Target: 2026-03-22)

### Goals
Game-changing features that make Lex unique.

### Feature 1: Shader DSL
**RFC:** `docs/LEX_SHADER_RFC.md`

Declarative shader definitions with multi-target compilation.

```lex
shader PBRMaterial {
    name: "pbr_model"
    targets: [glsl, hlsl, msl]
    
    uniforms: {
        albedo: texture2D
        metallic: float = 0.5
        roughness: float = 0.5
    }
    
    variants: {
        low: { samples: 1, shadows: false }
        high: { samples: 16, shadows: true, ssao: true }
    }
}
```

**Output:** GLSL, HLSL, MSL, C++ headers, documentation

### Feature 2: Aether Extended
**RFC:** `docs/LEX_AETHER_RFC.md`

Multi-agent AI context generation from single source.

```lex
era Steampunk {
    // Game data (existing)
    name: "Steampunk Era"
    
    // Game AI context (new)
    narrative: {
        themes: ["progress", "pollution"]
        mood: "industrial, tense"
    }
    
    // Developer AI context (new)
    developer: {
        architecture_notes: "Era transition triggers shader switch"
        dependencies: ["RenderGraph", "UIManager"]
    }
}
```

**Output:** 
- Game runtime (Lua, JSON)
- Game AI context (for Archivista)
- Developer AI context (for Droid, Cursor, Claude)
- Documentation (for humans)

### Feature 3: Layer System
Abstract/Concrete layer separation.

```lex
layer abstract {
    unit Scout { ... }  // Concept definition
}

layer game {
    unit Scout extends abstract.Scout {
        // Concrete implementation
        cost: { Gold: 50 }
    }
}
```

---

## Long Term: v0.4.0+ (Target: 2026-04+)

### Developer Experience
- **VS Code Extension**
  - Syntax highlighting
  - Autocomplete
  - Go to definition
- **Language Server Protocol (LSP)**
- **Hot Reload Integration**
- **Live Preview** (for shaders)

### Advanced Features
- **Compute Shaders** (GPU compute)
- **Ray Tracing Shaders** (VK_KHR_ray_tracing)
- **Visual Shader Editor** (node-based)

### Integration
- ModManager Imperium plugin
- Build system hooks (CMake, Make)
- CI/CD pipeline integration

---

## v1.0.0 Release Criteria

- [ ] All 4 backends functional (Lua, JSON, React, Lore)
- [ ] Shader DSL with multi-target compilation
- [ ] Aether Extended for AI context
- [ ] CLI stable and documented
- [ ] 90%+ test coverage
- [ ] Complete documentation
- [ ] 3+ real mods built with Lex
- [ ] Community feedback integrated

---

## Confine Base / Core

| Feature | Base | Core |
|---------|------|------|
| Strutture, unità, ere, tecnologie | ✅ | ✅ |
| Condizioni semplici | ✅ | ✅ |
| Espressioni aritmetiche | ❌ | ✅ |
| Variabili e stato | ❌ | ✅ |
| Accesso API engine C++ | ❌ | ✅ |
| Definizione sistemi di gioco | ❌ | ✅ |
| Shader e materiali | ❌ | Shader |
| Neural backends | ❌ | Neural |
| Contesto agenti AI | automatico | automatico |

---

## Related Projects

### Aether Debug Protocol
**RFC:** `docs/AETHER_DEBUG_RFC.md`

Separate project for AI-assisted crash analysis. Not part of Lex core, but integrates with Imperium 2.0.

**Key Features:**
- JSON crash output for agent consumption
- Pattern-based crash analysis
- Hypothesis generation with confidence scores
- Integration with Droid/Cursor/Claude

**Implementation:** Simplified version (watcher + JSON output) - 2-3 days

---

## Documentation Maintenance

> **IMPORTANTE:** Dopo ogni milestone completata, aggiornare:
> - `CHANGELOG.md` - feature completate, breaking changes
> - `STATE.md` - stato attuale, prossimi task
> - `LEX_ARCHITECTURE.md` - nuovi componenti
> - `LEX_SPECIFICATION.md` - nuove keywords/sintassi
> - `ROADMAP.md` - questo file

---

## Risk Mitigation

| Risk | Mitigation | Status |
|------|------------|--------|
| Scope creep | Strict MVP definition | Active |
| Complex syntax | Iteration with real modders | Planned |
| Poor error messages | Dedicated error formatting phase | Future |
| Performance | Incremental compilation | Future |

---

## Decision Log

### 2026-03-02: Master Design Integration
**Decision:** Integrato LEX_MASTER_DESIGN.md nei documenti esistenti
**Reason:** Evitare duplicazione e mantenere singola source of truth
**Changes:** Vision, Architecture, Roadmap aggiornati con contenuti del Master Design

### 2026-03-02: RFC Trio Published
**Decision:** Published 3 RFCs for revolutionary features (Shader DSL, Aether Extended, Debug Protocol)
**Reason:** Document design before implementation, gather feedback

### 2026-03-02: Simplified Debug Protocol
**Decision:** Implement simplified Aether Debug (watcher + JSON) instead of full RFC
**Reason:** 70% of value with 20% of effort, extensible later

### 2026-03-01: Separate v0.1.0 and v0.1.1
**Decision:** Core (lexer, parser, AST, Lua) is v0.1.0. CLI + Validator is v0.1.1.
**Reason:** Core is complete and tested. CLI/Validator are distinct deliverables.

### 2026-03-01: Validator Not Required for v0.1.0
**Decision:** v0.1.0 is "feature complete" for core transpilation.
**Reason:** Validator is enhancement, not blocker for basic functionality.
