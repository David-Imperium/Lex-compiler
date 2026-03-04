# Regole di Validazione

lex_engine valida tutti gli input prima di passarli a Imperium Engine.

## Codici di Errore

| Codice | Categoria | Descrizione |
|--------|-----------|-------------|
| LEX_IMP_001 | Risorsa | File risorsa mancante |
| LEX_IMP_002 | Risorsa | Formato risorsa non valido |
| LEX_IMP_003 | Tipo | Tipo non corrispondente |
| LEX_IMP_004 | Tipo | Cast non valido |
| LEX_IMP_005 | Riferimento | Variabile non definita |
| LEX_IMP_006 | Riferimento | Funzione non definita |
| LEX_IMP_007 | Sintassi | Errore di parsing |
| LEX_IMP_008 | Sintassi | Uso keyword non valido |
| LEX_IMP_009 | Gioco | Riferimento era non valido |
| LEX_IMP_010 | Gioco | Riferimento tech non valido |
| LEX_IMP_011 | Gioco | Schema gioco mancante |

## Pipeline di Validazione

```
Codice Sorgente (.lex)
       ↓
   Lexer
       ↓
   Parser
       ↓
   AST
       ↓
Validatore Schema ←── Schema Gioco (ere, unità, tech...)
       ↓
   Generatore Codice
       ↓
   Output (Lua/JSON)
```

## Validazione Schema

lex_engine controlla contro lo schema di gioco:

```json
{
  "eras": ["stone_age", "bronze_age", "iron_age"],
  "resources": ["wood", "stone", "gold", "food"],
  "units": ["worker", "warrior", "archer"],
  "techs": ["mining", "smithing", "agriculture"],
  "structures": ["house", "barracks", "farm"]
}
```

### Riferimenti Validati

- `spawn("worker", ...)` → controlla se "worker" esiste in units
- `require("mining")` → controlla se "mining" esiste in techs
- `build("farm", ...)` → controlla se "farm" esiste in structures

## Log Verbose

Tutti gli errori restituiscono JSON strutturato per agenti AI:

```json
{
  "code": "LEX_IMP_009",
  "severity": "error",
  "message": "Invalid era reference: 'future_age'",
  "file": "scripts/main.lex",
  "line": 42,
  "column": 15,
  "suggestion": "Available eras: stone_age, bronze_age, iron_age"
}
```
