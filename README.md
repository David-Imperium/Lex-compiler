# Lex Compiler

[![CI](https://github.com/David-Imperium/Lex-compiler/actions/workflows/ci.yml/badge.svg)](https://github.com/David-Imperium/Lex-compiler/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-1.0.1-blue.svg)](https://github.com/David-Imperium/Lex-compiler)
[![Live Demo](https://img.shields.io/badge/demo-live-green.svg)](https://david-imperium.github.io/Lex-compiler/)

<p align="center">
  <a href="https://david-imperium.github.io/Lex-compiler/">
    <img src="https://img.shields.io/badge/Try%20Live%20Demo-%F0%9F%9A%80-brightgreen?style=for-the-badge" alt="Live Demo">
  </a>
</p>

<p align="center">
  <video src="docs/assets/lex_showcase.mp4" autoplay loop muted playsinline width="800"></video>
</p>

**Stop writing boilerplate. Start describing worlds.**

Lex is a declarative, multi-target transpiler that turns simple definitions into ready-to-use game code. Write once, compile to Lua, JSON, Godot, Unity, LÖVE2D, Defold — and more coming.

*Your content. Your engine. Your rules.*

---

## The Problem

Every game engine wants data in a different format:

```lua
-- Lua wants this
items = { Sword = { damage = 50 } }
```

```json
// JSON wants this
{ "items": { "Sword": { "damage": 50 } } }
```

```gdscript
# Godot wants this
var items = { "Sword": { "damage": 50 } }
```

```csharp
// Unity wants this
[CreateAssetMenu] public class ItemData : ScriptableObject { ... }
```

**You're not making games. You're translating dictionaries.**

---

## The Solution

One source of truth. Multiple outputs.

```lex
item Sword {
    name: "Excalibur"
    damage: 50
    rarity: "legendary"
}
```

Compile:

```bash
lexc items.lex -t lua,json,gd,cs
```

Get clean, ready-to-use code for every engine. No boilerplate. No manual translation.

---

## Quick Start

```bash
# Build from source
git clone https://github.com/David-Imperium/Lex-compiler.git
cd Lex-compiler
cmake -B build -S .
cmake --build build --config Release

# Compile your first file
# Windows:
.\build\Release\lexc.exe examples\imperium_minimal.lex -t lua,json
# Linux/macOS:
./build/Release/lexc examples/imperium_minimal.lex -t lua,json
```

## Usage

```bash
# Basic usage
lexc input.lex -t lua,json

# Your game, your schema
lexc game.lex --types tower,character,scene -t json

# Multiple outputs, one command
lexc content.lex -o output/ -t lua,json,gd,cs,love2d,defold
```

**Options:**

- `-o, --output <dir>` — Where to write files
- `-t, --target <fmt>` — Output format(s): lua, json, gd, cs, love2d, defold
- `--types <list>` — Custom definition types (comma-separated)
- `--mode <mode>` — Visibility: modder (default) or developer
- `--no-validate` — Skip semantic validation (not recommended)
- `-v, --version` — Show version
- `-h, --help` — Show help

---

## Output Backends

| Backend | Target | Ready? |
|---------|--------|--------|
| `lua` | Generic Lua | ✅ |
| `json` | Universal JSON | ✅ |
| `gd` / `godot` | Godot 4.x | ✅ |
| `cs` / `unity` | Unity C# | ✅ |
| `love2d` | LÖVE2D | ✅ |
| `defold` | Defold | ✅ |
| `typescript` | TypeScript/JS | 📋 v1.1.0 |
| `unreal` | Unreal Engine | 📋 Lex Core |

**6 backends today. 10+ coming.**

---

## Custom Schemas

**Lex doesn't care about your game.** Tower defense? RPG? Visual novel? Define your own types:

```lex
character Hero {
    name: "Arthur"
    level: 5
    class: "Warrior"
    stats: { HP: 100, MP: 30 }
}

item Sword {
    name: "Excalibur"
    damage: 50
    rarity: "legendary"
}

quest SaveVillage {
    name: "Save the Village"
    reward: { Gold: 500, XP: 100 }
}

# Your game, your rules
entity whatever_you_want {
    any_property: "any_value"
    nested: { deep: { data: true } }
}
```

Compile:

```bash
lexc game.lex --types character,item,quest,entity -t json
```

Output:

```json
{
  "characters": {
    "Hero": { "id": "Hero", "type": "character", "name": "Arthur", "level": 5, "class": "Warrior", "stats": { "HP": 100, "MP": 30 } }
  },
  "items": {
    "Sword": { "id": "Sword", "type": "item", "name": "Excalibur", "damage": 50, "rarity": "legendary" }
  },
  "quests": {
    "SaveVillage": { "id": "SaveVillage", "type": "quest", "name": "Save the Village", "reward": { "Gold": 500, "XP": 100 } }
  }
}
```

---

## Features You'll Love

**Declarative Syntax** — Write what you mean, not how to implement it.

**Semantic Validation** — Catches typos, broken references, and logic errors at compile time. Your modders will thank you.

**Type Inference** — Strings, numbers, booleans, nested objects. Lex figures it out.

**English Keywords** — `structure`, `item`, `quest` — not `struct`, `itm`, `qst`. Accessible to everyone.

**AI-Ready** — Built-in context generator for LLM integration. Query your content in natural language.

**Blazing Fast** — Pure C++. No runtime dependencies. Parses thousands of nodes in milliseconds.

---

## Documentation

**Start here:** [Tutorial: Your First Mod](docs/TUTORIAL.md) — Build something real in 10 minutes.

**Deep dive:** [API Reference](docs/API_REFERENCE.md) — C++ library documentation.

**Integrate with your engine:**

- [Godot 4.x](docs/INTEGRATION_GODOT.md)
- [Unity](docs/INTEGRATION_UNITY.md)
- [LÖVE2D](docs/INTEGRATION_LOVE2D.md)

---

## Roadmap

| Version | Status | What's New |
|---------|--------|------------|
| v1.0.0 | ✅ Current | Stable release, 6 backends |
| v1.1.0 | 📋 Planned | TypeScript backend |
| v1.2.0 | 📋 Planned | LSP, watch mode |
| Lex Core | 🔜 Coming | Professional tier |

See [ROADMAP.md](docs/ROADMAP.md) for details.

---

## Testing

```bash
# Run all tests
# Windows:
.\build\tests\Release\lexer_tests.exe
# Linux/macOS:
./build/tests/lexer_tests

# 37 test cases. 392 assertions. All pass.
```

---

## License

**Lex Base** — MIT License. Free forever.

**Lex Core** — Commercial license. Coming soon.

---

## Author

Built by [David](https://github.com/David-Imperium) for the Imperium Game Engine.

*Why? Because I was tired of maintaining the same content in 5 different formats.*

---

## Documentation Languages

- **English** (current)
- **[Italiano / Italian](README_IT.md)**
