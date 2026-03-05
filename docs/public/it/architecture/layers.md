# Architettura Lex

**Versione:** 0.4.0

---

## Sistema a Layer

```
┌─────────────────────────────────────────────┐
│                 Lex Neural                  │  Contesto AI, Generazione Lore
├─────────────────────────────────────────────┤
│                  Lex Core                   │  Validazione, Riferimenti incrociati
├─────────────────────────────────────────────┤
│                  Lex Base                   │  Lexer, Parser, AST
└─────────────────────────────────────────────┘
```

---

## 1. Lex Base

**Scopo:** Layer fondazione

**Componenti:**
- `Lexer` - Tokenizza il codice sorgente
- `Parser` - Costruisce l'AST
- `AST` - Albero della Sintassi Astratta
- `SourceManager` - Gestione file/stringhe

**Dipendenze:** Nessuna

**Output:** AST

---

## 2. Lex Core

**Scopo:** Layer contenuti di gioco

**Componenti:**
- `Validator` - Validazione semantica
- `TypeChecker` - Inferenza dei tipi
- `Schema` - Registro definizioni
- `ReferenceResolver` - Risoluzione riferimenti incrociati

**Dipendenze:** Lex Base

**Output:** AST validato

---

## 3. Lex Neural

**Scopo:** Layer integrazione AI

**Componenti:**
- `AIContextGenerator` - Genera contesto AI-ready (JSON/Markdown/Minimal)
- `QuerySystem` - Query in linguaggio naturale sui dati di gioco
- `LoreGenerator` - Generazione contenuti narrativi

**Dipendenze:** Lex Core

**Output:** Contesto leggibile da AI (Markdown, JSON, Minimal)

**Nuovo in v0.4.0:**
- Tre formati output per contesto AI
- Sistema query per ricerca in linguaggio naturale
- Formato minimal compatto per context window LLM

---

## 4. Lex Engine (Privato)

**Scopo:** Layer integrazione engine

**Posizione:** `C:\Imperium 2.0\lex_engine\`

**Componenti:**
- `Runtime` - Interfaccia FFI
- `EngineBridge` - Chiamate all'engine
- `VerboseLogger` - Errori strutturati
- `SchemaValidator` - Validazione specifica engine

**Dipendenze:** Lex Core + Imperium Engine

**Output:** Comandi engine / Input validato

---

## Struttura File

```
C:\Lex\
├── src/
│   ├── lexer/          # Lex Base
│   ├── parser/         # Lex Base
│   ├── ast/            # Lex Base
│   ├── semantic/       # Lex Core
│   │   ├── validator.cpp
│   │   ├── type_checker.cpp
│   │   └── schema.cpp
│   ├── codegen/        # Backend output
│   │   ├── lua_backend.cpp
│   │   ├── json_backend.cpp
│   │   ├── godot_backend.cpp
│   │   └── unity_backend.cpp
│   └── lex.cpp          # API principale
│
└── docs/
    └── spec/
        ├── language.md
        ├── validation.md
        └── outputs.md
```

---

## Flusso Dati

```
file .lex
    │
    ▼ Lexer ──→ Token
    │
    ▼ Parser ──→ AST
    │
    ▼ Validator ──→ AST Validato
    │
    ▼ TypeChecker ──→ AST Tipizzato
    │
    ▼ Backend ──→ Output (Lua/JSON/Godot/Unity)
```

---

## Punti di Estensione

### Aggiungere un Nuovo Backend

1. Crea `src/codegen/xxx_backend.cpp`
2. Eredita dall'interfaccia `Backend`
3. Registra in `BackendFactory`
4. Aggiungi alle opzioni CLI

### Aggiungere una Nuova Keyword

1. Aggiungi token in `lexer/keywords.h`
2. Aggiungi regola grammaticale in `parser/grammar.cpp`
3. Aggiungi validazione in `semantic/validator.cpp`
4. Aggiorna `docs/spec/language.md`
