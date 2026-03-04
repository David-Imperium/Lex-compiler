# System Architecture

Lex is organized in three layers.

```
┌─────────────────────────────────────┐
│           Neural Layer              │  ← AI agents, code generation
├─────────────────────────────────────┤
│           Core Layer                │  ← Parser, compiler, validator
├─────────────────────────────────────┤
│           Base Layer                │  ← Lexer, AST, types
└─────────────────────────────────────┘
```

## Base Layer

Foundation components:

| Component | Purpose |
|-----------|---------|
| Lexer | Tokenizes source code |
| AST | Abstract Syntax Tree nodes |
| Types | Type system definitions |
| Diagnostics | Error reporting |

```
src/base/
├── lexer.cpp
├── ast.cpp
├── types.cpp
└── diagnostics.cpp
```

## Core Layer

Compiler pipeline:

| Component | Purpose |
|-----------|---------|
| Parser | Builds AST from tokens |
| Semantic Analyzer | Type checking, scope resolution |
| Validator | Schema validation |
| Code Generator | Backend output |

```
src/core/
├── parser.cpp
├── semantic.cpp
├── validator.cpp
└── codegen.cpp
```

## Neural Layer

AI-powered features:

| Component | Purpose |
|-----------|---------|
| AI Bridge | Communication with AI agents |
| Auto-complete | IDE support |
| Refactoring | Code transformations |
| Generation | AI-assisted code creation |

```
src/neural/
├── ai_bridge.cpp
├── autocomplete.cpp
└── generator.cpp
```

## Data Flow

```
Source → Lexer → Tokens → Parser → AST → 
Semantic → Validator → CodeGen → Output
                ↑
         Game Schema
```
