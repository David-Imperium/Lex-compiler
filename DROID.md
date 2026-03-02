# Lex Compiler — Istruzioni Agente

Rispondi SEMPRE in italiano. Codice e commit in inglese.

## REGOLE (non negoziabili)

1. **Leggi PRIMA** — MAI modificare senza leggere. MAI assumere il contenuto.
2. **Verifica** — Dopo ogni modifica, controlla che compili e i test passino.
3. **Un problema alla volta** — Obiettivi piccoli e verificabili.
4. **Stop al doppio fallimento** — 2 tentativi falliti → fermati, spiega, cambia approccio.
5. **Se il contesto diventa lungo** → usa `/compact` proattivamente.

## MEMORIA (RAG) — USA SEMPRE

Hai accesso al MCP server `david-rag`. Usalo ATTIVAMENTE:

- **`rag_search("query")`** — Cerca PRIMA di iniziare qualsiasi task.
- **`rag_add("testo")`** — Salva quando: completi un task, trovi un bug, prendi una decisione.
- **`rag_reindex()`** — Dopo aver modificato file .md importanti.

## STATO PROGETTO — .planning/STATE.md

- **INIZIO SESSIONE:** Leggi `.planning/STATE.md`.
- **DOPO OGNI TASK:** Aggiorna STATE.md (Active/Next/Recent).
- **OGNI 10 AZIONI:** Aggiorna anche senza task completati.
- **FINE SESSIONE:** Aggiorna STATE.md. OBBLIGATORIO.

## STRUTTURA

- `src/lexer/` — Analisi lessicale (token, keywords)
- `src/parser/` — Parsing sintattico (recursive descent)
- `src/ast/` — Nodi AST e visitors
- `src/codegen/` — Backend (Lua, JSON, React planned)
- `tests/` — Unit test
- `docs/` — RFCs e specifiche

## BUILD

```
cmake -B build -S .
cmake --build build
ctest --test-dir build
```

## C++

- Namespace `lex {}`, `#pragma once`
- `PascalCase` classi, `snake_case` metodi, `snake_case_` member vars
- `unique_ptr`, `const` correctness, `enum class`
- Commit inglese, mai push senza permesso
