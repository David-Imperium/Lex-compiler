# Lex Language Documentation

**Version:** 1.0.0
**Status:** Active Development

---

## What is Lex?

Lex is a **declarative, multi-target transpiler** for game content. Write once, compile to multiple backends.

```
game.lex → Lua, JSON, Godot, Unity, Love2D, Defold, AI Context
```

---

## Quick Links

| Document | Purpose |
|----------|---------|
| [Language Spec](./spec/language.md) | Syntax, types, keywords |
| [Validation Rules](./spec/validation.md) | What lex_engine validates |
| [Output Backends](./spec/outputs.md) | Lua, JSON, Godot, Unity, Love2D, Defold |
| [Architecture](./architecture/layers.md) | How Lex is structured |
| [Tutorial](./guides/tutorial.md) | Getting started |

**New in v0.4.0:** [AI Context Generator](./spec/outputs.md#7-ai-context-generator) — Generate AI-ready context from .lex files

---

## Architecture Overview

```
┌─────────────────────────────────────────────┐
│                 Lex Neural                  │  AI Context, Lore
├─────────────────────────────────────────────┤
│                  Lex Core                   │  Validation, references
├─────────────────────────────────────────────┤
│                  Lex (Free)                  │  Lexer, parser, AST
└─────────────────────────────────────────────┘
```

---

## 30 Second Example

```lex
era Ancient {
    name: "Ancient Era"
}

structure Farm {
    era: Ancient
    name: "Farm"
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

Compiles to:

```lua
Buildings.Farm = {
    era = "Ancient",
    cost = { Gold = 50 },
    production = { Food = 5 }
}
```

---

## For AI Agents

- Use **English keywords only** (`structure`, not `struttura`)
- Read [spec/validation.md](./spec/validation.md) for error codes
- All definitions are declarative — describe WHAT, not HOW
