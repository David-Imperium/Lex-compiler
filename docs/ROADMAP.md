# Lex Compiler Roadmap

**Current Version:** 0.4.1  
**License:** MIT (Free Tier Complete)

---

## Released Versions

### v0.4.1 — Accessibility & Documentation (2026-03-05)

**Status:** ✅ Released

**New Features:**
- Easy installation scripts (`install.ps1`, `install.sh`)
- Unix man page (`docs/lexc.1`)
- GitHub Pages website (`docs/index.html`)
- Integration guides for Godot, Unity, LÖVE2D
- Enhanced CLI help with examples
- `-v/--version` flag

**Bug Fixes:**
- Fixed Godot backend: dynamic class names from filename
- Fixed CLI: pass targets correctly to compile_file()

---

### v0.4.0 — AI Context & Query System (2026-03-04)

**Status:** ✅ Released

**New Features:**
- AI Context Generator (JSON, Markdown, Minimal formats)
- Query System (natural language queries via `--query`)
- Love2D backend (LÖVE2D framework)
- Defold backend (Defold engine)

**Complete Backends (6):**
| Backend | Target | Status |
|---------|--------|--------|
| `lua` | Generic Lua | ✅ |
| `json` | Universal JSON | ✅ |
| `gd` | Godot 4.x | ✅ |
| `cs` | Unity | ✅ |
| `love2d` | LÖVE2D | ✅ |
| `defold` | Defold | ✅ |

**Test Coverage:** 325 assertions

---

### v0.3.4 — Module System (2026-03-04)

**Status:** ✅ Released

**Features:**
- Multi-file compilation with `use` keyword
- Module namespaces with `module` keyword
- Visibility modifiers (`public`, `internal`, `private`)
- `--mode modder|developer` CLI flag

---

### v0.3.3 — Fully Agnostic Core (2026-03-04)

**Status:** ✅ Released

**Features:**
- Schema-driven validation (no Imperium hardcoding)
- Generic `Definition` class (replaced 9 concrete classes)
- Custom schema support via `--types`
- Reference validation across definitions

---

### v0.3.0 — Core Compiler (2026-03-02)

**Status:** ✅ Released

**Features:**
- Complete lexer, parser, AST
- Expression parser with operator precedence
- Semantic validation
- Lua and JSON backends
- CLI tool (`lexc`)
- 257 test assertions

---

## Planned Versions

### v0.5.0 — TypeScript Backend

**Status:** 📋 Planned

**Goals:**
- TypeScript/JavaScript code generation
- Type definitions output (`.d.ts`)
- Node.js module format
- Web game support (Phaser, PixiJS, etc.)

**Stretch:**
- Source maps for debugging

---

### v0.6.0 — Developer Experience

**Status:** 📋 Planned

**Goals:**
- Improved error messages with source context
- VS Code extension improvements (diagnostics, hover)
- Watch mode (`--watch` for live recompilation)
- Incremental compilation

**Stretch:**
- Language Server Protocol (LSP)

---

### v0.7.0 — Python Backend

**Status:** 📋 Planned

**Goals:**
- Python code generation
- Dataclass-based output
- Pydantic models for validation
- Godot 4.x Python (GDExtension) support

---

### v1.0.0 — Stable Release

**Status:** 🔮 Future

**Requirements:**
- All backends production-tested
- Complete documentation
- Stable CLI API (no breaking changes)
- Community feedback integrated
- Real-world projects using Lex

---

## Future Considerations

**Domain-Specific Backends:**
- Shader DSL (GLSL/HLSL generation)
- Aether integration (Imperium Engine)
- Dialogue system output (Ink, YarnSpinner)

**Tooling:**
- Web playground (compile in browser)
- Package registry (share `.lex` modules)
- VS Code debugger integration

**Distribution:**
- Homebrew (macOS): `brew install lexc`
- Chocolatey (Windows): `choco install lexc`
- AUR (Arch Linux): `lexc-bin`
- npm (Node.js wrapper): `npm install -g lexc`

**Performance:**
- Parallel compilation for large projects
- Binary intermediate format for faster loads

---

## Version Naming

- **Patch** (0.4.1) — Bug fixes, minor improvements
- **Minor** (0.5.0) — New features, backends, backwards compatible
- **Major** (1.0.0) — Breaking changes, stable API guarantee
