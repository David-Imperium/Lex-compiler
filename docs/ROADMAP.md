# Lex Compiler Roadmap

**Current Version:** 1.1.0
**License:** MIT

---

## Released Versions

### v1.1.0 — License & Quality (2026-03-08)

**Status:** ✅ Released

**Architecture:**
- Type checking integrated into compilation pipeline
- Modular CLI architecture (5 header-only components)
- Nested data structures (ObjectValue/ArrayValue)

**Features:**
- `TypeCheckPass` validates type annotations
- Object and array values in definitions
- Refactored CLI with AnimatedSpinner (8 styles)
- File watching with cross-platform FileWatcher
- License system for premium features
- Package system for Lex-Plus extensions

**Premium Backends:**
- Unreal Engine (UCLASS, USTRUCT, UENUM)
- RPC protocol generation

**Code Quality:**
- main.cpp reduced from ~650 to ~180 lines
- Better separation of concerns
- Header-only modules for easy extension

---

### v1.0.1 — Fixes & Docs (2026-03-05)

**Status:** ✅ Released

**Features:**
- Animated spinner and colored output
- React demo for code transformation
- Schema parameter fixes

---

### v1.0.0 — First Stable Release (2026-03-05)

**Status:** ✅ Released

**Architecture:**
- Modular Pass system (Lexer → Parser → Validation → Visibility → Codegen)
- BackendRegistry for extensible backends
- CompilerPipeline orchestrator

**Features:**
- 6 production backends: Lua, JSON, Godot, Unity, Love2D, Defold
- AI Context Generator for LLM integration
- Natural language Query System
- VS Code extension with syntax highlighting

**Quality:**
- 35/35 tests passing
- Clear error messages with codes and positions
- Complete documentation (Tutorial, API Reference, Integration Guides)

---

### v0.4.1 — Accessibility & Documentation (2026-03-05)

**Status:** ✅ Released

**New Features:**
- Easy installation scripts (`install.ps1`, `install.sh`)
- Unix man page (`docs/lexc.1`)
- GitHub Pages website (React web app)
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

### v1.1.0 — TypeScript Backend

**Status:** 📋 Planned

**Goals:**
- TypeScript/JavaScript code generation
- Type definitions output (`.d.ts`)
- Node.js module format
- Web game support (Phaser, PixiJS, etc.)

---

### v1.2.0 — Developer Experience

**Status:** 📋 Planned

**Goals:**
- Watch mode (`--watch` for live recompilation)
- Incremental compilation
- Language Server Protocol (LSP)
- VS Code diagnostics (real-time error checking)

---

### v1.3.0 — WebAssembly Demo

**Status:** 📋 Planned

**Goals:**
- WebAssembly compilation target (Emscripten)
- JavaScript bindings for browser integration
- Interactive web playground (write Lex, see output in real-time)
- No backend required — pure client-side compilation

**Technical:**
- `lexc.wasm` module with JS bindings
- String-based I/O (no filesystem required)
- Integration with existing React web demo

---

## Commercial Tiers

Lex is available in two tiers:

| Tier | Target | License |
|------|--------|---------|
| **Lex (Free)** | Modders, Hobbyists | MIT (Free) |
| **Lex-Plus Core** | Indie Developers | Commercial ($49) |

### Lex (Free)
- 6 engine backends: Lua, JSON, Godot, Unity, Love2D, Defold
- Custom schemas via `--types`
- AI Context Generator
- Query System
- VS Code extension
- GitHub public repository

### Lex-Plus Core ($49 lifetime)
Premium backends for professional game developers:
- **Ready:** Unreal, RPC
- **Planned:** GameMaker, Rust, C++, Python, TypeScript, Go, Java, C
- Multi-file compilation
- Advanced expressions
- Codeberg private repository (invite-only)

**Early Bird Pricing:** $49 lifetime (locks in before subscription model)

---

## Future Considerations

**Domain-Specific Backends:**
- Shader DSL (GLSL/HLSL generation)
- Aether integration (Imperium Engine)
- Dialogue system output (Ink, YarnSpinner)

**Tooling:**
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
