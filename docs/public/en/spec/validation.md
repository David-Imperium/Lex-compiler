# Lex Validation Rules

**Version:** 0.3.2

This document defines ALL validation rules that lex_engine must enforce.

---

## 1. Required Properties

### 1.1 Structure

| Property | Required | Type | Validation |
|----------|----------|------|------------|
| `era` | YES | reference | Must reference defined era |
| `name` | NO | string | - |
| `cost` | NO | ResourceMap | Keys must be defined resources |
| `production` | NO | ResourceMap | Keys must be defined resources |
| `maintenance` | NO | ResourceMap | Keys must be defined resources |

### 1.2 Unit

| Property | Required | Type | Validation |
|----------|----------|------|------------|
| `era` | YES | reference | Must reference defined era |
| `type` | YES | enum | Must be: `military`, `civilian`, `hero` |
| `stats` | NO | block | If present, requires `attack`, `defense`, `movement` |

### 1.3 Technology

| Property | Required | Type | Validation |
|----------|----------|------|------------|
| `research_cost` | YES | int | Must be > 0 |
| `prerequisites` | NO | ReferenceList | Must reference defined technologies |
| `unlocks` | NO | block | References must exist |

### 1.4 Entity (Engine)

| Property | Required | Type | Validation |
|----------|----------|------|------------|
| `pos` | YES | tuple(float, float, float) | 3 floats required |
| `faction` | YES | enum | Must be: `Player`, `Enemy`, `Neutral` |
| `health` | NO | int | Must be > 0, default: 100 |
| `model` | NO | string | Should be valid path (warning if not found) |

### 1.5 Era

| Property | Required | Type | Validation |
|----------|----------|------|------------|
| `name` | YES | string | Non-empty |

### 1.6 Resource

| Property | Required | Type | Validation |
|----------|----------|------|------------|
| `name` | YES | string | Non-empty |
| `category` | NO | enum | If present: `basic`, `luxury`, `strategic` |

---

## 2. Cross-Reference Rules

### 2.1 Era References

**Rule:** `era: <Identifier>` must reference a defined era.

```lex
// ✅ Valid
era Ancient { name: "Ancient" }
structure Farm { era: Ancient }

// ❌ Invalid: Medieval not defined
structure Castle { era: Medieval }
```

**Error Code:** `LEX_IMP_001`

### 2.2 Technology References

**Rule:** `prerequisites: [TechA, TechB]` must reference defined technologies.

```lex
// ✅ Valid
technology SteamEngine { research_cost: 100 }
technology AdvancedSteam {
    prerequisites: [SteamEngine]  // OK
}

// ❌ Invalid: UnknownTech not defined
technology Foo {
    prerequisites: [UnknownTech]
}
```

**Error Code:** `LEX_IMP_003`

### 2.3 Structure References

**Rule:** `requires: { structures: [BuildingA] }` must reference defined structures.

**Error Code:** `LEX_IMP_004`

### 2.4 Unit References

**Rule:** `unlocks: { units: [UnitA] }` must reference defined units.

**Error Code:** `LEX_IMP_005`

### 2.5 Resource References

**Rule:** ResourceMap keys must reference defined resources.

```lex
// ✅ Valid
resource Gold { name: "Gold" }
structure Farm { cost: { Gold: 50 } }

// ❌ Invalid: Wood not defined
structure Mine { cost: { Wood: 20 } }
```

**Error Code:** `LEX_IMP_002`

---

## 3. Type Validation

### 3.1 Integer

**Rule:** Must be numeric, no decimals.

```lex
cost: 50          // ✅ Valid
cost: 50.5        // ⚠️ Warning: float used as int
cost: "fifty"     // ❌ Invalid: type mismatch
```

### 3.2 Float

**Rule:** Must be numeric.

```lex
ratio: 1.5        // ✅ Valid
ratio: 50         // ✅ Valid (int promoted to float)
ratio: "high"     // ❌ Invalid: type mismatch
```

### 3.3 String

**Rule:** Must be quoted.

```lex
name: "Farm"      // ✅ Valid
name: Farm        // ⚠️ Warning: looks like reference, not string
```

### 3.4 Enum

**Rule:** Must be one of allowed values.

```lex
// For unit.type
type: military    // ✅ Valid
type: civilian    // ✅ Valid
type: hero        // ✅ Valid
type: tank        // ❌ Invalid: not in enum

// For entity.faction
faction: "Player"   // ✅ Valid
faction: "Enemy"    // ✅ Valid
faction: "Neutral"  // ✅ Valid
faction: "Goblin"   // ❌ Invalid: not in enum
```

### 3.5 Tuple

**Rule:** Must have correct number of elements.

```lex
// Position: 3 floats
pos: (10.0, 0.0, 5.0)       // ✅ Valid
pos: (10.0, 0.0)            // ❌ Invalid: expected 3, got 2
pos: (10, 0, 5)             // ✅ Valid (int promoted)

// Rotation (quaternion): 4 floats
rot: (0.0, 0.0, 0.0, 1.0)   // ✅ Valid
rot: (0.0, 0.0)             // ❌ Invalid: expected 4, got 2
```

---

## 4. Semantic Rules

### 4.1 Duplicate Definitions

**Rule:** No two definitions can have the same name and type.

```lex
// ❌ Invalid: duplicate
structure Farm { era: Ancient }
structure Farm { era: Medieval }  // ERROR
```

### 4.2 Circular Dependencies

**Rule:** No circular references in prerequisites.

```lex
// ❌ Invalid: circular
technology A { prerequisites: [B] }
technology B { prerequisites: [A] }
```

### 4.3 Era Membership

**Rule:** Structures, units must belong to an era.

```lex
// ❌ Invalid: missing era
structure Farm {
    name: "Farm"
    // ERROR: era required
}
```

### 4.4 Conditional Blocks

**Rule:** `available_if`, `active_if` must have `condition`.

```lex
// ✅ Valid
available_if: {
    condition: population > 100
}

// ❌ Invalid: missing condition
available_if: {
    message: "Not available"
}
```

---

## 5. Engine-Specific Validation

### 5.1 Asset Paths

**Rule:** `model: "path/to/file.glb"` should reference existing file.

```lex
entity Warrior {
    model: "assets/warrior.glb"   // ✅ If file exists
    model: "missing.glb"          // ⚠️ Warning: file not found
}
```

### 5.2 Value Ranges

| Property | Min | Max | Default |
|----------|-----|-----|---------|
| `health` | 1 | 10000 | 100 |
| `research_cost` | 1 | 100000 | - |
| `movement` | 1 | 10 | - |

```lex
stats: {
    health: 5000      // ✅ Valid
    health: 50000     // ⚠️ Warning: exceeds max
    health: 0         // ❌ Invalid: below min
}
```

---

## 6. Error Codes

| Code | Category | Message |
|------|----------|---------|
| `LEX001` | Lexer | Unterminated string |
| `LEX002` | Lexer | Invalid hex color |
| `LEX003` | Lexer | Invalid number format |
| `SYN001` | Syntax | Missing closing brace |
| `SYN002` | Syntax | Unexpected token |
| `SYN003` | Syntax | Missing colon |
| `SEM001` | Semantic | Undefined reference |
| `SEM002` | Semantic | Duplicate definition |
| `SEM003` | Semantic | Missing required property |
| `SEM004` | Semantic | Type mismatch |
| `SEM005` | Semantic | Circular dependency |
| `LEX_IMP_001` | Validation | Unknown era reference |
| `LEX_IMP_002` | Validation | Unknown resource reference |
| `LEX_IMP_003` | Validation | Unknown technology reference |
| `LEX_IMP_004` | Validation | Unknown structure reference |
| `LEX_IMP_005` | Validation | Unknown unit reference |
| `LEX_IMP_006` | I/O | File not found |
| `LEX_IMP_007` | Scene | Scene operation failed |
| `LEX_IMP_008` | FFI | Engine not initialized |
| `LEX_IMP_009` | Type | Invalid enum value |
| `LEX_IMP_010` | Schema | No eras defined (warning) |
| `LEX_IMP_011` | Schema | No resources defined (warning) |

---

## 7. Error Format

All errors are returned as structured JSON:

```json
{
  "severity": "error",
  "code": "LEX_IMP_001",
  "category": "validation",
  "message": "Unknown era reference: 'Medieval'",
  "file": "structures.lex",
  "line": 15,
  "column": 10,
  "suggestion": "Define era 'Medieval' or check spelling",
  "ai_hint": "Era must be defined before referencing in structure/unit/technology"
}
```

**Severity levels:**
- `error` - Compilation fails
- `warning` - Compiled but may cause issues
- `info` - Informational only
