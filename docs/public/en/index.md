# Lex Language Documentation

A domain-specific language for game logic and scripting.

## Overview

Lex is designed to be:
- **Simple**: Clean syntax inspired by modern languages
- **Safe**: Built-in validation catches errors before runtime
- **Flexible**: Multiple backend targets (Lua, JSON, Godot)

## Documentation

### Specification
- [Language Reference](./spec/language.md) - Syntax, types, keywords
- [Validation Rules](./spec/validation.md) - What lex_engine validates
- [Output Backends](./spec/outputs.md) - Supported compilation targets

### Architecture
- [System Layers](./architecture/layers.md) - Base, Core, Neural layers

### Integrations
- [Imperium Engine](./integrations/imperium-engine.md) - C++ FFI bridge

### Guides
- [Getting Started](./guides/tutorial.md) - Quick start tutorial

## Quick Example

```lex
fn main() {
    let x: int = 42;
    let name: string = "Hello";
    
    if x > 10 {
        spawn("worker_unit", x, 0, 0);
    }
}
```

## Repository

- **Compiler**: `src/` - Lexer, parser, code generator
- **Engine Bridge**: `lex_engine/` - FFI for Imperium integration
- **Tests**: `tests/` - Test suite
