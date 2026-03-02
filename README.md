# Lex Compiler

A declarative, multi-target transpiler for game content.

**Status:** v0.3.0 — Core Complete

## What is Lex?

Lex compiles `.lex` files to multiple output formats:

```
game.lex ──► lexc ──┬──► game.lua    (gameplay logic)
                    ├──► game.json   (data definitions)
                    └──► game.ts     (UI types — planned)
```

**Principle:** *Describe what, not how.*

## Example

```lex
structure SteamFactory {
    era: Steampunk
    name: "Steam Factory"
    
    cost: {
        Coal: 8,
        Steel: 5,
        Gold: 50
    }
    
    production: {
        Energy: 15,
        Industry: 10
    }
    
    available_if has_technology(SteamEngine) {
    }
}
```

Compiles to valid Lua and JSON.

## Features

- **Declarative syntax** — Clean, readable definitions
- **Multi-target** — Lua and JSON backends (more planned)
- **Type-safe** — Strong typing with inference
- **Semantic validation** — Catches errors before runtime
- **Expression parser** — Full operator precedence support
- **English keywords** — Accessible to international community

## Build

```bash
cmake -B build -S .
cmake --build build --config Release
```

## Usage

```bash
lexc input.lex -o output/ -t lua,json
```

Options:
- `-o, --output <dir>` — Output directory
- `-t, --target <fmt>` — Output format(s): lua, json
- `--no-validate` — Skip semantic validation
- `-h, --help` — Show help

## Project Structure

```
Lex/
├── src/
│   ├── lexer/         # Tokenization
│   ├── parser/        # AST generation
│   ├── ast/           # AST definitions
│   ├── semantic/      # Type checking, validation
│   ├── codegen/       # Backend implementations
│   └── main.cpp       # CLI entry point
├── docs/              # Documentation
├── examples/          # Example .lex files
└── tests/             # Unit tests
```

## Documentation

- [Language Specification](docs/LEX_SPECIFICATION.md)
- [Architecture](docs/LEX_ARCHITECTURE.md)
- [Roadmap](docs/ROADMAP.md)
- [RFCs](docs/rfc/) — Future feature proposals

## Roadmap

| Version | Status | Features |
|---------|--------|----------|
| v0.3.0 | ✅ Current | Core complete, Lua/JSON backends |
| v0.3.1 | 🔜 Next | React/TypeScript backend |
| v0.4.0 | 📋 Planned | VS Code extension |

## Testing

```bash
# Unit tests
.\build\tests\Debug\lexer_tests.exe

# Lua output validation
python tests/lua_validator.py output/game.lua
```

## License

MIT

## Author

David (Imperium Game Engine)
