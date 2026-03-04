# Architettura del Sistema

Lex è organizzato in tre layer.

```
┌─────────────────────────────────────┐
│          Neural Layer               │  ← Agenti AI, generazione codice
├─────────────────────────────────────┤
│          Core Layer                 │  ← Parser, compilatore, validatore
├─────────────────────────────────────┤
│          Base Layer                 │  ← Lexer, AST, tipi
└─────────────────────────────────────┘
```

## Base Layer

Componenti fondamentali:

| Componente | Scopo |
|------------|-------|
| Lexer | Tokenizza il codice sorgente |
| AST | Nodi Abstract Syntax Tree |
| Types | Definizioni sistema tipi |
| Diagnostics | Segnalazione errori |

```
src/base/
├── lexer.cpp
├── ast.cpp
├── types.cpp
└── diagnostics.cpp
```

## Core Layer

Pipeline compilatore:

| Componente | Scopo |
|------------|-------|
| Parser | Costruisce AST da token |
| Analizzatore Semantico | Type checking, risoluzione scope |
| Validatore | Validazione schema |
| Generatore Codice | Output backend |

```
src/core/
├── parser.cpp
├── semantic.cpp
├── validator.cpp
└── codegen.cpp
```

## Neural Layer

Funzionalità AI:

| Componente | Scopo |
|------------|-------|
| AI Bridge | Comunicazione con agenti AI |
| Auto-complete | Supporto IDE |
| Refactoring | Trasformazioni codice |
| Generation | Creazione codice assistita AI |

```
src/neural/
├── ai_bridge.cpp
├── autocomplete.cpp
└── generator.cpp
```

## Flusso Dati

```
Sorgente → Lexer → Token → Parser → AST → 
Semantico → Validatore → CodeGen → Output
                ↑
          Schema Gioco
```
