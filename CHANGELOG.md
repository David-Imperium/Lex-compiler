# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [0.3.2] - 2026-03-03

### Added
- **Dynamic Lua Backend** - Runtime module loading and hot-reload support
- **Library Refactor** - Core compiler as reusable library
- **VS Code Extension** - Syntax highlighting and basic language support
- **GitHub Actions** - Automated CI/CD with binary releases

### Changed
- Improved CLI architecture with better error messages

## [0.3.1] - 2026-03-02

### Added
- **Godot Backend** - Generate GDScript for Godot Engine projects
- **Unity Backend** - Generate C# scripts for Unity game development
- **Love2D Backend** - Generate Lua scripts optimized for LÖVE framework

## [0.3.0] - 2026-03-02

### Added
- **Core Compiler** - Complete lexer, parser, AST, type checker
- **Lua Backend** - Generate gameplay scripts from `.lex` files
- **JSON Backend** - Generate data definitions for game engines
- **CLI Tool** (`lexc`) - Multi-target compilation with validation
- **Expression Parser** - Full operator precedence support
- **Semantic Validation** - Type checking and cross-reference validation
- **257 test assertions** - Comprehensive test coverage

### Documentation
- Language Specification
- Architecture Guide
- Keywords Reference
- Example files with generated outputs

### Supported Definitions
- `era` - Historical periods
- `structure` - Buildings and facilities
- `unit` - Military and civilian units
- `technology` - Research and unlocks
- `resource` - Game resources
- `choice` - Player decisions
- `event` - Game events
- `ending` - Victory/defeat conditions
- `secret` - Hidden content

### Conditions
- `when` - Activation conditions
- `if` - Conditional logic
- `available_if` - Availability requirements
- `secret_if` - Secret unlock conditions
- `active_if` - Active state conditions
- `bonus_if` - Bonus application conditions

## [0.1.0] - 2026-03-01

### Added
- Initial lexer implementation
- Basic parser with recursive descent
- AST node structure
- Lua code generation prototype
- Catch2 test framework setup

---

## Roadmap

| Version | Target | Features |
|---------|--------|----------|
| 0.4.0 | TBD | Binary releases, expanded tutorials |
| 0.5.0 | TBD | TypeScript backend |
| 1.0.0 | TBD | Stable release, full documentation |
