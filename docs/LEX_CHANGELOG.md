# Lex Changelog

Tutte le modifiche notevoli a questo progetto saranno documentate in questo file.

Il formato è basato su [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

---

## [Unreleased]

### Added
- Visione del progetto (`LEX_VISION.md`)
- Struttura documentazione iniziale
- Piano di sviluppo MVP

### Changed
- Nessun cambiamento ancora

### Deprecated
- Niente ancora

### Removed
- Niente ancora

### Fixed
- Niente ancora

---

## [0.1.0] - 2026-03-01 (COMPLETATO)

**Obiettivo:** Transpiler Lex → Lua funzionante

### Added
- **Lexer completo** (`src/lexer/`)
  - Token types: literals, identifiers, keywords, operators, punctuation
  - 50+ keywords supportate (core, conditional, property, logical)
  - String escape sequences, hex colors, multiline strings
  - Error accumulation con line/column tracking
- **Parser completo** (`src/parser/`)
  - Recursive descent parser
  - 10 definition types: era, structure, unit, technology, resource, choice, ending, event, secret
  - Property parsing: Expression, ResourceMap, ReferenceList
  - Condition parsing: when, if, available_if, secret_if, active_if, bonus_if
  - Error recovery per continuare dopo errori
- **AST completo** (`src/ast/`)
  - Base classes: ASTNode, ASTVisitor
  - Expressions con type-safe variant
  - Collections: ResourceMap, ReferenceList
  - Properties e Conditions
  - 10 concrete Definition types
- **Lua Backend** (`src/codegen/`)
  - BackendRegistry per extensibility
  - Lua code generation da AST
  - Resource map e reference list formatting
- **Test Suite** (`tests/`)
  - Catch2 framework configurato
  - 17+ test cases (Lexer + Parser)
  - Coverage: keywords, identifiers, literals, definitions, properties, conditions
- **Documentazione**
  - Specifiche linguistiche (`LEX_SPECIFICATION.md`)
  - Architettura compilatore (`LEX_ARCHITECTURE.md`)
  - Keywords reference (`LEX_KEYWORDS.md`)
  - Esempi in `docs/examples/`

### In Progress (per completare v0.1.0)
- [ ] CLI tool (`lexc`) - main.cpp stub esiste
- [ ] Semantic Validator - header definito, manca implementazione

---

## Roadmap

### [0.1.1] - Target: 2026-03-08

**Obiettivo:** Completare CLI e Validator

**Features:**
- [x] Lexer + Parser + AST (completato in 0.1.0)
- [x] Lua code generator (completato in 0.1.0)
- [ ] CLI tool (`lexc`) con argomenti
- [ ] Semantic Validator (cross-reference, required props)
- [ ] Integration test con file .lex reali

### [0.2.0] - Target: 2026-03-22 (3 settimane)

**Obiettivo:** Multi-target code generation

**Features:**
- [ ] JSON backend (data definitions)
- [ ] React/TypeScript backend (UI components)
- [ ] Lore backend (AI context)
- [ ] Hot-reload durante sviluppo

**Integrazione:**
- [ ] Plugin ModManager Imperium
- [ ] Build system integration

### [0.3.0] - Target: 2026-04-01 (1 mese)

**Obiettivo:** Tooling e Developer Experience

**Features:**
- [ ] VS Code extension (syntax highlighting, autocomplete)
- [ ] LSP (Language Server Protocol)
- [ ] Error messages user-friendly
- [ ] Debug mode (source mapping)

**Testing:**
- [ ] Integration tests con mods esistenti
- [ ] Performance benchmarks
- [ ] Modder feedback

### [1.0.0] - Target: TBD

**Obiettivo:** Release pubblica

**Features:**
- [ ] Documentazione completa
- [ ] Tutorial interattivo
- [ ] Example projects
- [ ] Community support

---

## Version Numbering

- **0.x.x** - Development releases (breaking changes expected)
- **1.x.x** - Stable releases (semantic versioning)
- **Major** - Breaking language changes
- **Minor** - New features, backward compatible
- **Patch** - Bug fixes, backward compatible

---

## Note per Sviluppo

### Documentazione Post-Milestone

> **IMPORTANTE:** Dopo ogni milestone/fase grande, aggiornare SEMPRE:
> - `CHANGELOG.md` - questo file (feature, fixes, breaking changes)
> - `.planning/STATE.md` - stato attuale e prossimi task
> - `LEX_ARCHITECTURE.md` - se cambiano componenti architetturali
> - `LEX_SPECIFICATION.md` - se cambiano keywords o sintassi
> - `ROADMAP.md` - progress e aggiornamenti roadmap

### Quando aggiornare questo file

**Added:**
- Nuove features
- Nuovi file di documentazione
- Nuovi esempi

**Changed:**
- Modifiche a sintassi esistente
- Modifiche a behavior
- Refactoring

**Fixed:**
- Bug fix
- Error messages migliorati
- Documentation fixes

**Removed:**
- Features deprecate rimosse
- Codice legacy eliminato

### Convenzione Commit

```
type(scope): description

- type: feat|fix|docs|style|refactor|test|chore
- scope: lexer|parser|codegen|docs|cli
- description: brief description

Examples:
- feat(lexer): add string interpolation
- fix(codegen): correct Lua table generation
- docs(spec): clarify era syntax
- refactor(parser): simplify AST building
```

---

## Metrics Tracking

### Code Metrics (Updated Weekly)

| Settimana | Righe Codice | Test Coverage | Backend Supportati |
|-----------|-------------|---------------|-------------------|
| 1         | 0           | 0%            | 0                 |
| 2         | -           | -             | -                 |
| 3         | -           | -             | -                 |

### Documentation Metrics

| Settimana | File Doc | Righe Doc | Esempi |
|-----------|----------|-----------|--------|
| 1         | 3        | ~100      | 0      |
| 2         | -        | -         | -      |

---

## Decision Log

### 2026-03-01: Lex come Transpiler (non Runtime)

**Decisione:** Lex transpila a codice esistente (Lua, JSON, React, etc.) invece di avere un proprio runtime.

**Motivazione:**
- Nessun nuovo runtime = meno complessità
- Performance native (LuaJIT, V8, etc.)
- Integrazione pulita con sistemi esistenti
- Modder possono vedere e debuggare codice generato

**Alternativa scartata:** Lex runtime side-by-side con Lua
- Troppo overhead (due VM)
- Complessità debugging
- Non necessario per MVP

### 2026-03-01: MVP Focus su Lua Backend

**Decisione:** Prima implementazione genera solo Lua.

**Motivazione:**
- Lua è il layer principale di Imperium
- 1 settimana per avere qualcosa di funzionante
- Testabile immediatamente con mod esistenti
- Altri backend vengono dopo

---

## Rischi e Mitigazioni

| Rischio | Probabilità | Impatto | Mitigazione | Stato |
|---------|-------------|---------|-------------|-------|
| Sintassi troppo complessa | Alta | Alto | Iterare con modder reali | 🟡 |
| Performance compilazione | Media | Medio | Incremental compilation | ⏳ |
| Debugging difficile | Alta | Alto | Source mapping, debug mode | ⏳ |
| Feature creep | Media | Alto | MVP rigoroso, "no" alle feature extra | 🟡 |
| Abbandono progetto | Media | Alto | Releases frequenti, community early | 🟡 |

---

## Community & Support

**Stato attuale:** 🟡 Pre-alpha (sviluppo interno)

**Piano:**
1. MVP funzionante con Imperium
2. Testing con modder selezionati
3. Open source alpha release
4. Community feedback integration
5. Beta → Stable release

**Non promettere feature che non abbiamo implementato.**

---

*Ultimo aggiornamento: 2026-03-01*
