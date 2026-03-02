# Lex Development Roadmap

**Version:** 0.2.0
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
