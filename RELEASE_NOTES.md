# Lex Compiler Release Notes

---

## v0.4.1 — Accessibility & Documentation (2026-03-05)

### New Features

**Easy Installation**
- `install.ps1` — Windows PowerShell installer
- `install.sh` — Linux/macOS Bash installer
- One-line install: `irm ... | iex` (Windows) or `curl ... | bash` (Unix)

**Documentation**
- `docs/lexc.1` — Unix man page
- `docs/index.html` — GitHub Pages website
- `docs/INTEGRATION_GODOT.md` — Godot 4.x integration guide
- `docs/INTEGRATION_UNITY.md` — Unity integration guide
- `docs/INTEGRATION_LOVE2D.md` — LÖVE2D integration guide

**CLI Improvements**
- `-v/--version` flag to show version
- Enhanced `--help` with examples and backend descriptions
- Better error messages for unknown definition types

**Bug Fixes**
- Fixed Godot backend: dynamic class names from input filename
- Fixed CLI: pass targets correctly to compile_file()

---

## v0.4.0 — AI Context & Query System (2026-03-04)

### New Features

**AI Context Generator**
- `--context json` — Structured JSON context for LLMs
- `--context md` — Markdown documentation
- `--context minimal` — Compact format for prompts
- `--context all` — Generate all formats

**Query System**
- `--query "What costs the most?"` — Natural language queries
- `--query "What does Farm require?"` — Dependency queries
- Automatic query interpretation and response

**New Backends**
- `love2d` — LÖVE2D framework module format
- `defold` — Defold engine module format

**Complete Backend List (6)**
| Backend | Target | Status |
|---------|--------|--------|
| `lua` | Generic Lua | ✅ |
| `json` | Universal JSON | ✅ |
| `gd` | Godot 4.x | ✅ |
| `cs` | Unity | ✅ |
| `love2d` | LÖVE2D | ✅ |
| `defold` | Defold | ✅ |

**Test Coverage**: 325 assertions

---

## v0.3.4 — Module System (2026-03-04)

### New Features

- Multi-file compilation with `use` keyword
- Module namespaces with `module` keyword
- Visibility modifiers (`public`, `internal`, `private`)
- `--mode modder|developer` CLI flag for content visibility

---

## v0.3.3 — Fully Agnostic Core (2026-03-04)

### New Features

- Schema-driven validation (no Imperium hardcoding)
- Generic `Definition` class (replaced 9 concrete classes)
- Custom schema support via `--types` flag
- Reference validation across definitions

---

## v0.3.0 — Core Compiler (2026-03-02)

### New Features

- Complete lexer, parser, AST
- Expression parser with operator precedence
- Semantic validation
- Lua and JSON backends
- CLI tool (`lexc`)
- 257 test assertions

---

## Installation

### Windows
```powershell
irm https://raw.githubusercontent.com/David-Imperium/Lex-compiler/main/install.ps1 | iex
```

### Linux/macOS
```bash
curl -fsSL https://raw.githubusercontent.com/David-Imperium/Lex-compiler/main/install.sh | bash
```

### From Source
```bash
git clone https://github.com/David-Imperium/Lex-compiler.git
cd Lex-compiler
cmake -B build -S .
cmake --build build --config Release
```

---

## Roadmap

See [ROADMAP.md](docs/ROADMAP.md) for planned features:
- **v0.5.0** — TypeScript backend
- **v0.6.0** — Developer experience (watch mode, VS Code)
- **v0.7.0** — Python backend
- **v1.0.0** — Stable release
