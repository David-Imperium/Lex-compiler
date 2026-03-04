# Validation Rules

lex_engine validates all inputs before passing to Imperium Engine.

## Error Codes

| Code | Category | Description |
|------|----------|-------------|
| LEX_IMP_001 | Resource | Missing resource file |
| LEX_IMP_002 | Resource | Invalid resource format |
| LEX_IMP_003 | Type | Type mismatch |
| LEX_IMP_004 | Type | Invalid cast |
| LEX_IMP_005 | Reference | Undefined variable |
| LEX_IMP_006 | Reference | Undefined function |
| LEX_IMP_007 | Syntax | Parse error |
| LEX_IMP_008 | Syntax | Invalid keyword usage |
| LEX_IMP_009 | Game | Invalid era reference |
| LEX_IMP_010 | Game | Invalid tech reference |
| LEX_IMP_011 | Game | Missing game schema |

## Validation Pipeline

```
Source Code (.lex)
       ↓
   Lexer
       ↓
   Parser
       ↓
   AST
       ↓
Schema Validator ←── Game Schema (eras, units, techs...)
       ↓
   Code Generator
       ↓
   Output (Lua/JSON)
```

## Schema Validation

lex_engine checks against game schema:

```json
{
  "eras": ["stone_age", "bronze_age", "iron_age"],
  "resources": ["wood", "stone", "gold", "food"],
  "units": ["worker", "warrior", "archer"],
  "techs": ["mining", "smithing", "agriculture"],
  "structures": ["house", "barracks", "farm"]
}
```

### Validated References

- `spawn("worker", ...)` → checks if "worker" exists in units
- `require("mining")` → checks if "mining" exists in techs
- `build("farm", ...)` → checks if "farm" exists in structures

## Verbose Logging

All errors output structured JSON for AI agents:

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
