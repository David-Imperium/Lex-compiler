# Lex Development Roadmap

**Version:** 0.3.0
**Last Updated:** 2026-03-02

---

## Current Status: v0.3.0 (Core Complete)

### Completed Components
| Component | Status | Notes |
|-----------|--------|-------|
| Lexer | ✅ Done | 50+ keywords, full literal support |
| Parser | ✅ Done | 10 definition types, conditions, error recovery |
| Expression Parser | ✅ Done | Pratt parser with operator precedence |
| AST | ✅ Done | Type-safe, visitor pattern, typed accessors |
| Type Checker | ✅ Done | Semantic type validation |
| Validator | ✅ Done | Errors/warnings separation |
| Lua Backend | ✅ Done | Full expression/condition generation |
| JSON Backend | ✅ Done | Imperium engine integration |
| CLI | ✅ Done | Multi-target support (`-t lua,json`) |
| Tests | ✅ Active | 257 assertions in 23 test cases |

---

## Next: v0.3.1 (Short Term)

### React/TypeScript Backend
Generate TypeScript interfaces from definitions for UI consumption.

```bash
lexc game.lex -o output/ -t lua,json,react
# Produces: *.lua, *.json, *.ts
```

**Tasks:**
- [ ] TypeScript interface generator
- [ ] React component props generation
- [ ] Type-safe game data hooks

### Lore Backend
Generate AI context for narrative systems.

**Tasks:**
- [ ] Markdown output format
- [ ] Structured text for companion AI

---

## Future: v0.4.0+ (Planned)

| Feature | Description | Priority |
|---------|-------------|----------|
| VS Code Extension | Syntax highlighting, autocomplete | Medium |
| Layer System | Abstract/Concrete separation | Low |
| C++ Backend | Engine bindings generation | Low |
| Rust Backend | Editor interfaces | Low |

---

## v1.0.0 Release Criteria

- [ ] 4 backends functional (Lua, JSON, React, Lore)
- [ ] CLI stable and documented
- [ ] 90%+ test coverage
- [ ] Complete documentation
- [ ] 3+ real mods built with Lex

---

## Decision Log

### 2026-03-02: Phase 1 Complete
**Commits:** 1466c3f, 1206cdc, ddcfbd7
**Result:** Core compiler feature-complete with 257 test assertions passing

### 2026-03-02: Roadmap Simplified
**Decision:** Removed speculative features (Shader DSL, Neural, Aether Extended)
**Reason:** Focus on concrete, deliverable features first
