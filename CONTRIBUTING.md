# Contributing to Lex

Thanks for your interest in contributing!

## Development Setup

```bash
git clone https://github.com/David-Imperium/Lex-compiler.git
cd lex
cmake -B build -S .
cmake --build build --config Release
```

## Running Tests

```bash
.\build\tests\Debug\lexer_tests.exe
```

## Code Style

- Namespace: `lex {}`
- Classes: `PascalCase`
- Methods: `snake_case`
- Members: `snake_case_`
- Constants: `UPPER_CASE`
- Include guards: `#pragma once`

See [AGENTS.md](AGENTS.md) for detailed conventions.

## Pull Request Process

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Run tests and ensure they pass
5. Commit with clear messages (`feat: add amazing feature`)
6. Push to your fork
7. Open a Pull Request

## Commit Message Format

```
type: description

[optional body]
```

Types: `feat`, `fix`, `docs`, `test`, `refactor`, `chore`

## Questions?

Open an issue or discussion on GitHub.
