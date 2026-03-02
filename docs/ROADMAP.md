# Lex Development Roadmap

**Version:** 0.1.0
**Last Updated:** 2026-03-01

---

## Current Status: v0.1.0 (Core Complete)

### Completed
| Component | Status | Notes |
|-----------|--------|-------|
| Lexer | Done | 50+ keywords, full literal support |
| Parser | Done | 10 definition types, conditions, error recovery |
| AST | Done | Type-safe, visitor pattern |
| Lua Backend | Done | Code generation functional |
| Tests | Active | 17+ test cases with Catch2 |

### Missing for v0.1.1
- [ ] CLI tool (`lexc`) - read files, invoke pipeline, write output
- [ ] Semantic Validator - cross-reference checking, required properties
- [ ] Integration tests with real `.lex` files

---

## Short Term: v0.1.1 (Target: 2026-03-08)

### Goals
Complete the transpiler into a usable CLI tool.

### Tasks
1. **CLI Implementation** (`src/main.cpp`)
   - Parse command line arguments
   - Read `.lex` source files
   - Invoke: Lexer → Parser → Validator → Backend
   - Write output files
   - Error reporting with file:line:column format

2. **Semantic Validator** (`src/parser/validator.cpp`)
   - Symbol table construction
   - Cross-reference validation (references exist)
   - Required property checking per definition type
   - Type compatibility checks

3. **Integration Testing**
   - Test with `docs/examples/*.lex` files
   - Verify Lua output is valid and runnable
   - Error case testing

### Success Criteria
```bash
# This should work:
lexc input.lex -o output/ --target lua
# Produces: output/*.lua
```

---

## Medium Term: v0.2.0 (Target: 2026-03-22)

### Goals
Multi-target code generation.

### New Backends
1. **JSON Backend** - Data definitions for game configs
2. **React/TypeScript Backend** - UI component generation
3. **Lore Backend** - AI context for narrative systems

### Features
- Backend selection via CLI: `--target lua,json,react`
- Hot-reload support for development
- Source mapping for debugging

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
