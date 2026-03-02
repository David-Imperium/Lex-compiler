# Lex Compiler

A declarative, multi-target transpiler for game modding.

**Status:** v0.2.0 - MVP Complete

## What is Lex?

Lex is a domain-specific language that compiles `.lex` files to multiple output formats:

```
game.lex ──► lexc ──┬──► game.lua    (gameplay logic)
                    ├──► game.json   (data definitions)
                    └──► game.ts     (UI types)
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
}
```

Compiles to valid Lua, JSON, and TypeScript.

## Features

- **Declarative syntax** - Clean, readable definitions
- **Multi-target** - Lua, JSON, TypeScript backends
- **Type-safe** - Strong typing with inference
- **Semantic validation** - Catches errors before runtime
- **English keywords** - Accessible to international community

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
- `-o, --output <dir>` - Output directory
- `-t, --target <fmt>` - Output format(s): lua, json (comma-separated)
- `--no-validate` - Skip semantic validation
- `-h, --help` - Show help

## Project Structure

```
Lex/
├── src/
│   ├── lexer/         # Tokenization
│   ├── parser/        # AST generation
│   ├── ast/           # AST node definitions
│   ├── codegen/       # Backend implementations
│   └── main.cpp       # CLI entry point
├── docs/              # Documentation
├── examples/          # Example .lex files
└── tests/             # Unit tests
```

## Roadmap

| Version | Status | Features |
|---------|--------|----------|
| v0.2.0 | ✅ Current | CLI, Validator, JSON backend |
| v0.3.0 | 🔜 Planned | Layer system, React backend |
| v1.0.0 | 🎯 Target | VS Code extension, LSP |

See [docs/ROADMAP.md](docs/ROADMAP.md) for details.

## Documentation

- [Language Specification](docs/LEX_SPECIFICATION.md)
- [Architecture](docs/LEX_ARCHITECTURE.md)
- [Vision](docs/LEX_VISION.md)

## License

MIT

## Author

David (Imperium Game Engine)
