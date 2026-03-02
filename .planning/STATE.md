# State
Updated: 2026-03-01

## Active
- Completamento v0.1.0 (CLI + Validator)

## Next (Priority Order)
1. **CLI Implementation** - main.cpp deve leggere file, invocare pipeline, scrivere output
2. **Validator Implementation** - implementare validator.cpp (header definito)
3. **Integration Test** - test end-to-end con file .lex reali in docs/examples/
4. **v0.2.0 Backends** - JSON, React, Lore backends
5. **Error Reporting** - formattazione con line/column

## Recent (Completed)
- [x] Lexer completo con test (13+ test cases)
- [x] Parser completo con test (17+ test cases totali)
  - Definition types: era, structure, unit, technology, resource, choice, ending, event, secret
  - Property parsing con Expression, ResourceMap, ReferenceList
  - Condition parsing (when, if, available_if, etc.)
  - Error recovery
- [x] AST completo con 10 definition types + conditions + properties
- [x] Lua backend funzionante
- [x] Catch2 test framework configurato

## Component Status

| Component | Status | Files |
|-----------|--------|-------|
| Lexer | Completo | token.h, keywords.h, lexer.h/cpp |
| Parser | Completo | parser.h/cpp, validator.h |
| AST | Completo | ast.h/cpp |
| Validator | Header only | validator.h (manca .cpp) |
| CodeGen Lua | Completo | backend.h, lua_backend.h/cpp |
| CodeGen JSON | Non iniziato | - |
| CodeGen React | Non iniziato | - |
| CodeGen Lore | Non iniziato | - |
| CLI | Stub | main.cpp (solo hello world) |
| Tests | Attivo | test_main.cpp (547 righe) |

## Metrics

| Metrica | Valore |
|---------|--------|
| Righe codice src/ | ~12K |
| Test totali | 17+ |
| Backends supportati | 1 (Lua) |
| Definition types | 10 |
| Keywords | 50+ |

## Documentation Reminder
**Dopo ogni fase grande (milestone), aggiornare:**
- CHANGELOG.md - feature completate/rimosse
- STATE.md - questo file
- LEX_ARCHITECTURE.md - se cambiano componenti
- LEX_SPECIFICATION.md - se cambiano keywords/sintassi
