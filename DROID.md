# Lex Compiler — Droid Instructions

> Read automatically at session start.

## CRITICAL RULES

1. **UPDATE `.planning/STATE.md`** after EVERY completed task
2. **READ `.planning/STATE.md`** at session start
3. **READ `AGENTS.md`** for code conventions
4. **One problem at a time** — small, verifiable goals
5. **Test before proceeding** — every change must compile and pass tests

## STRUCTURE

- `src/lexer/` — Lexical analysis
- `src/parser/` — Syntax parsing
- `src/ast/` — AST nodes
- `src/semantic/` — Type checking, validation
- `src/codegen/` — Backends (Lua, JSON)
- `tests/` — Unit tests
- `docs/` — Documentation
- `docs/rfc/` — Future feature proposals

## BUILD

```bash
cmake -B build -S .
cmake --build build
.\build\tests\Debug\lexer_tests.exe
```

## CONTEXT SYSTEM

- `.planning/STATE.md` — Current state
- `AGENTS.md` — Code style and architecture
- `docs/ROADMAP.md` — Development roadmap

## STYLE

- Namespace `lex {}`, `#pragma once`
- `PascalCase` classes, `snake_case` methods
- Member vars with trailing underscore: `source_`
- Smart pointers (`unique_ptr`), `const` correctness
- Commits in English, never push without permission
