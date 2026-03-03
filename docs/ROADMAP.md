# Lex Development Roadmap

**Version:** 0.3.2
**Last Updated:** 2026-03-03

---

## Current Status: v0.3.2 (Agnostic Core)

### Completed Components
| Component | Status | Notes |
|-----------|--------|-------|
| Lexer | ✅ Done | 50+ keywords, full literal support |
| Parser | ✅ Done | Dynamic types via `--types`, conditions, error recovery |
| Expression Parser | ✅ Done | Pratt parser with operator precedence |
| AST | ✅ Done | Type-safe, visitor pattern, typed accessors |
| Type Checker | ✅ Done | Semantic type validation |
| Validator | ✅ Done | Errors/warnings separation |
| Lua Backend | ✅ Done | Full expression/condition generation |
| JSON Backend | ✅ Done | Dynamic output for any type |
| CLI | ✅ Done | `--types` flag for custom schemas |
| Tests | ✅ Active | 257 assertions in 23 test cases |

### New in v0.3.2
- **`--types` CLI flag** — Define custom definition types
- **Agnostic core** — Not tied to any specific game
- **Dynamic JSON output** — Works with any schema

---

## Roadmap: Accessibility (v0.4.0)

Making Lex usable by everyone, not just C++ developers.

### 1. Binary Releases
| Task | Status |
|------|--------|
| Windows `.exe` release | 📋 Planned |
| Linux binary | 📋 Planned |
| macOS binary | 📋 Planned |
| GitHub Releases workflow | 📋 Planned |

### 2. Documentation
| Task | Status |
|------|--------|
| Tutorial: "Your first mod" | 📋 Planned |
| API reference | 📋 Planned |
| Error message catalog | 📋 Planned |
| Video walkthrough | 📋 Planned |

### 3. Error Messages
| Task | Status |
|------|--------|
| User-friendly wording | 📋 Planned |
| Suggestions on typos | 📋 Planned |
| Error codes reference | 📋 Planned |
| `--verbose` mode | 📋 Planned |

### 4. Editor Support
| Task | Status |
|------|--------|
| VS Code extension | 📋 Planned |
| Syntax highlighting | 📋 Planned |
| Autocomplete | 📋 Planned |
| Error squiggles | 📋 Planned |

### 5. Examples
| Task | Status |
|------|--------|
| Complete mini-game example | 📋 Planned |
| Multiple schema examples | 📋 Planned |
| Integration examples | 📋 Planned |

---

## Roadmap: Backends (v0.5.0) — Planned

### Godot Backend ✅ Complete
GDScript generation for Godot Engine integration.

### Unity Backend ✅ Complete  
C# generation for Unity integration.

### TypeScript/React Backend — Planned
Generate TypeScript interfaces from definitions.

```bash
lexc game.lex -o output/ -t lua,json,ts
# Produces: *.lua, *.json, *.ts
```

**Tasks:**
- [ ] TypeScript interface generator
- [ ] React component props generation
- [ ] Type-safe game data hooks

---

## v1.0.0 Release Criteria

- [ ] Binary releases for all platforms
- [ ] Complete tutorial documentation
- [ ] VS Code extension published
- [ ] 4 backends functional (Lua, JSON, Godot, Unity)
- [ ] 90%+ test coverage
- [ ] 3+ real projects built with Lex

---

## Changelog

### 2026-03-03: v0.3.2 — Documentation Update
- Fixed EBNF EraDefinition syntax error in LEX_SPECIFICATION.md
- Updated JSON Backend status from "Future" to "Complete"
- Added Godot and Unity backends as complete
- TypeScript/Python backends marked as "Planned"
- Keywords count aligned to 50+ (actual implementation)

### 2026-03-02: v0.3.1 — Agnostic Core
- Added `--types` CLI flag for custom schemas
- Decoupled from Imperium (now uses default schema)
- Dynamic JSON backend for any definition type

### 2026-03-02: v0.3.0 — Core Complete
**Commits:** 1466c3f, 1206cdc, ddcfbd7
**Result:** Core compiler feature-complete with 257 test assertions passing
