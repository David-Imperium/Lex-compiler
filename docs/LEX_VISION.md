# Lex — Project Vision

**Version:** 0.3.0
**Last Updated:** 2026-03-02

---

## What is Lex?

Lex is a declarative transpiler for game content. Write `.lex` files, get Lua, JSON, and TypeScript output.

**Core principle:**
> Describe *what*, not *how*.

---

## Why Lex Exists

### Without Lex
To add a building to a game, a modder must:
1. Create JSON data file
2. Write Lua logic script
3. Build React UI component
4. Keep everything synchronized

### With Lex
One `.lex` file describes the intent. The compiler generates everything else.

---

## Target Users

### 1. Modders
- Add content (structures, units, technologies)
- Don't want to learn multiple languages
- Output: Lua, JSON, React

### 2. Game Developers
- Configure complex game systems
- Single source of truth for data
- Output: Multiple backends

---

## Design Philosophy

1. **Simplicity First** — Zero unnecessary boilerplate
2. **Declarative** — Describe what exists, not how to execute
3. **Multi-Target** — One file, multiple outputs
4. **Human Readable** — Clean syntax, English keywords
5. **Helpful Errors** — Every error tells you how to fix it

---

## Current Capabilities

| Feature | Status |
|---------|--------|
| Lexer | ✅ Complete |
| Parser | ✅ Complete |
| Expression Parser | ✅ Complete |
| Type Checker | ✅ Complete |
| Lua Backend | ✅ Complete |
| JSON Backend | ✅ Complete |
| CLI | ✅ Complete |

## Future Plans

| Feature | Priority |
|---------|----------|
| React/TypeScript Backend | High |
| Lore Backend | Medium |
| VS Code Extension | Medium |
| Layer System | Low |

---

## What Lex is NOT

- Not a replacement for C++ — engine core stays in C++
- Not a general-purpose language — specific to game content
- Not magic — generates code, doesn't replace thinking

---

*"The best language lets you say what you want, not what the compiler wants."*
