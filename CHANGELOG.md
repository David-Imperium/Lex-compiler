# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [0.4.0] - 2026-03-04

### Added
- **AI Context Generator** — Generate structured context for AI agents
  - JSON format for AI tools
  - Markdown format for human-readable docs
  - Minimal format for LLM prompts
  - `--context json|md|minimal|all` CLI option

- **Query System** — Natural language queries
  - "What does X require?"
  - "What unlocks X?"
  - "Show all structures"
  - "What produces Gold?"
  - "What costs the most?"
  - `--query "..."` CLI option

- **Love2D Backend** — LÖVE2D framework code generation
  - Generates `GameData` module with helper functions
  - `love.filesystem` integration for hot reload
  - State serialization for save/load

- **Defold Backend** — Defold engine code generation
  - Generates `M` module (Defold convention)
  - Collection factory URL integration

### Changed
- **Free Tier Complete** — All 6 backends available (MIT License):
  - Lua, JSON, Godot, Unity, Love2D, Defold

- **325 test assertions** — Full coverage

## [0.3.4] - 2026-03-04

### Added
- **Module System** — Multi-file compilation with visibility modifiers
  - `use` keyword for importing modules
  - `module` keyword for declaring module namespaces
  - `public`, `internal`, `private` visibility modifiers
  - `--mode modder|developer` CLI flag for output filtering
  - Modders see only `public` definitions, developers see everything

### Changed
- **Visibility Filtering** — Phase 3.5 in compilation pipeline
  - Internal/private definitions hidden in modder mode
  - Warnings emitted when definitions are skipped

## [0.3.3] - 2026-03-04

### Changed
- **Fully Agnostic Core** - Removed all Imperium hardcoding from compiler
  - Removed 9 concrete definition classes (EraDefinition, StructureDefinition, etc.)
  - Single generic `parse_definition()` instead of 9 duplicate methods
  - Schema-driven validation (reads required props from SchemaRegistry)
  - Schema-driven type checking (uses schema type_hints)
  - Added `reference_type` to PropertySchema for cross-reference validation

### Breaking Changes
- Removed concrete AST classes - use generic `Definition` with `definition_type` field

### Added
- Test for custom RPG schema (character, item, quest)
- `examples/test_rpg.lex` - demonstrates custom schema usage

### Fixed
- Centralized version macros in `src/lex/version.h`
- Removed ~200 lines of dead code from json_backend

- Removed ~200 lines of dead code from json_backend
- Removed duplicate `parse_definition()` method in favor of generic `Definition` approach
- Schema-driven validation (reads required props from SchemaRegistry)
- Schema-driven type checking (uses schema type hints)
- Added `reference_type` to PropertySchema for cross-reference validation

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
