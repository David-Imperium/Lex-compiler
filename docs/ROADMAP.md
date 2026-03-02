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

3. **Layer System Design** (New Feature)
   - Abstract/Concrete layer separation
   - Namespace-based organization
   - Cross-layer references

### Success Criteria
```bash
# Multi-target output:
lexc game.lex -o output/ -t lua,json,react
# Produces: output/*.lua, output/*.json, output/*.ts
```

---

## Medium Term: v0.3.0 (Target: 2026-03-22)

### Goals
Developer experience and direct engine integration.

### Features
1. **Layer System** - Abstract/Concrete separation like Aether
   - `namespace aether { }` for abstract definitions
   - Cross-layer inheritance and references
   - Validation across layers

2. **Direct Imperium Integration**
   - Output to `C:\Imperium 2.0\data\`
   - Hot-reload support for development
   - Source mapping for debugging

3. **VS Code Extension**
   - Syntax highlighting
   - Autocomplete
   - Go to definition

### Integration
- ModManager Imperium plugin
- Build system hooks (CMake, Make)

---

## Long Term: v0.3.0+ (Target: 2026-04+)

### Developer Experience
- VS Code extension
  - Syntax highlighting
  - Autocomplete
  - Go to definition
- Language Server Protocol (LSP)
- User-friendly error messages
- Debug mode with source mapping

### Quality
- Comprehensive test suite
- Performance benchmarks
- Documentation website
- Tutorial and examples

---

## v1.0.0 Release Criteria

- [ ] All 4 backends functional (Lua, JSON, React, Lore)
- [ ] CLI stable and documented
- [ ] 90%+ test coverage
- [ ] Complete documentation
- [ ] 3+ real mods built with Lex
- [ ] Community feedback integrated

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

### 2026-03-01: Separate v0.1.0 and v0.1.1
**Decision:** Core (lexer, parser, AST, Lua) is v0.1.0. CLI + Validator is v0.1.1.
**Reason:** Core is complete and tested. CLI/Validator are distinct deliverables.

### 2026-03-01: Validator Not Required for v0.1.0
**Decision:** v0.1.0 is "feature complete" for core transpilation.
**Reason:** Validator is enhancement, not blocker for basic functionality.
