# Regole di Validazione Lex

**Versione:** 0.4.0

Questo documento definisce TUTTE le regole di validazione che lex_engine deve applicare.

---

## 1. Proprietà Obbligatorie

### 1.1 Structure

| Proprietà | Obbligatoria | Tipo | Validazione |
|-----------|--------------|------|-------------|
| `era` | SÌ | reference | Deve riferirsi a un'era definita |
| `name` | NO | string | - |
| `cost` | NO | ResourceMap | Le chiavi devono essere risorse definite |
| `production` | NO | ResourceMap | Le chiavi devono essere risorse definite |
| `maintenance` | NO | ResourceMap | Le chiavi devono essere risorse definite |

### 1.2 Unit

| Proprietà | Obbligatoria | Tipo | Validazione |
|-----------|--------------|------|-------------|
| `era` | SÌ | reference | Deve riferirsi a un'era definita |
| `type` | SÌ | enum | Deve essere: `military`, `civilian`, `hero` |
| `stats` | NO | block | Se presente, richiede `attack`, `defense`, `movement` |

### 1.3 Technology

| Proprietà | Obbligatoria | Tipo | Validazione |
|-----------|--------------|------|-------------|
| `research_cost` | SÌ | int | Deve essere > 0 |
| `prerequisites` | NO | ReferenceList | Deve riferirsi a tecnologie definite |
| `unlocks` | NO | block | I riferimenti devono esistere |

### 1.4 Entity (Engine)

| Proprietà | Obbligatoria | Tipo | Validazione |
|-----------|--------------|------|-------------|
| `pos` | SÌ | tuple(float, float, float) | 3 float richiesti |
| `faction` | SÌ | enum | Deve essere: `Player`, `Enemy`, `Neutral` |
| `health` | NO | int | Deve essere > 0, default: 100 |
| `model` | NO | string | Dovrebbe essere un percorso valido (warning se non trovato) |

### 1.5 Era

| Proprietà | Obbligatoria | Tipo | Validazione |
|-----------|--------------|------|-------------|
| `name` | SÌ | string | Non vuoto |

### 1.6 Resource

| Proprietà | Obbligatoria | Tipo | Validazione |
|-----------|--------------|------|-------------|
| `name` | SÌ | string | Non vuoto |
| `category` | NO | enum | Se presente: `basic`, `luxury`, `strategic` |

---

## 2. Regole di Riferimento Incrociato

### 2.1 Riferimenti Era

**Regola:** `era: <Identificatore>` deve riferirsi a un'era definita.

```lex
// ✅ Valido
era Ancient { name: "Ancient" }
structure Farm { era: Ancient }

// ❌ Non valido: Medieval non definita
structure Castle { era: Medieval }
```

**Codice Errore:** `LEX_IMP_001`

### 2.2 Riferimenti Tecnologia

**Regola:** `prerequisites: [TechA, TechB]` deve riferirsi a tecnologie definite.

```lex
// ✅ Valido
technology SteamEngine { research_cost: 100 }
technology AdvancedSteam {
    prerequisites: [SteamEngine]  // OK
}

// ❌ Non valido: UnknownTech non definita
technology Foo {
    prerequisites: [UnknownTech]
}
```

**Codice Errore:** `LEX_IMP_003`

### 2.3 Riferimenti Structure

**Regola:** `requires: { structures: [BuildingA] }` deve riferirsi a strutture definite.

**Codice Errore:** `LEX_IMP_004`

### 2.4 Riferimenti Unit

**Regola:** `unlocks: { units: [UnitA] }` deve riferirsi a unità definite.

**Codice Errore:** `LEX_IMP_005`

### 2.5 Riferimenti Resource

**Regola:** Le chiavi ResourceMap devono riferirsi a risorse definite.

```lex
// ✅ Valido
resource Gold { name: "Gold" }
structure Farm { cost: { Gold: 50 } }

// ❌ Non valido: Wood non definita
structure Mine { cost: { Wood: 20 } }
```

**Codice Errore:** `LEX_IMP_002`

---

## 3. Validazione Tipi

### 3.1 Intero

**Regola:** Deve essere numerico, senza decimali.

```lex
cost: 50          // ✅ Valido
cost: 50.5        // ⚠️ Warning: float usato come int
cost: "fifty"     // ❌ Non valido: tipo non corrispondente
```

### 3.2 Float

**Regola:** Deve essere numerico.

```lex
ratio: 1.5        // ✅ Valido
ratio: 50         // ✅ Valido (int promosso a float)
ratio: "high"     // ❌ Non valido: tipo non corrispondente
```

### 3.3 Stringa

**Regola:** Deve essere tra virgolette.

```lex
name: "Farm"      // ✅ Valido
name: Farm        // ⚠️ Warning: sembra un riferimento, non una stringa
```

### 3.4 Enum

**Regola:** Deve essere uno dei valori consentiti.

```lex
// Per unit.type
type: military    // ✅ Valido
type: civilian    // ✅ Valido
type: hero        // ✅ Valido
type: tank        // ❌ Non valido: non nell'enum

// Per entity.faction
faction: "Player"   // ✅ Valido
faction: "Enemy"    // ✅ Valido
faction: "Neutral"  // ✅ Valido
faction: "Goblin"   // ❌ Non valido: non nell'enum
```

### 3.5 Tupla

**Regola:** Deve avere il numero corretto di elementi.

```lex
// Posizione: 3 float
pos: (10.0, 0.0, 5.0)       // ✅ Valido
pos: (10.0, 0.0)            // ❌ Non valido: attesi 3, trovati 2
pos: (10, 0, 5)             // ✅ Valido (int promosso)

// Rotazione (quaternione): 4 float
rot: (0.0, 0.0, 0.0, 1.0)   // ✅ Valido
rot: (0.0, 0.0)             // ❌ Non valido: attesi 4, trovati 2
```

---

## 4. Regole Semantiche

### 4.1 Definizioni Duplicate

**Regola:** Nessuna due definizioni possono avere lo stesso nome e tipo.

```lex
// ❌ Non valido: duplicato
structure Farm { era: Ancient }
structure Farm { era: Medieval }  // ERRORE
```

### 4.2 Dipendenze Circolari

**Regola:** Nessun riferimento circolare nei prerequisiti.

```lex
// ❌ Non valido: circolare
technology A { prerequisites: [B] }
technology B { prerequisites: [A] }
```

### 4.3 Appartenenza Era

**Regola:** Strutture e unità devono appartenere a un'era.

```lex
// ❌ Non valido: era mancante
structure Farm {
    name: "Farm"
    // ERRORE: era richiesta
}
```

### 4.4 Blocchi Condizionali

**Regola:** `available_if`, `active_if` devono avere `condition`.

```lex
// ✅ Valido
available_if: {
    condition: population > 100
}

// ❌ Non valido: condition mancante
available_if: {
    message: "Not available"
}
```

---

## 5. Validazione Specifica Engine

### 5.1 Percorsi Asset

**Regola:** `model: "path/to/file.glb"` dovrebbe riferirsi a un file esistente.

```lex
entity Warrior {
    model: "assets/warrior.glb"   // ✅ Se il file esiste
    model: "missing.glb"          // ⚠️ Warning: file non trovato
}
```

### 5.2 Range Valori

| Proprietà | Min | Max | Default |
|-----------|-----|-----|---------|
| `health` | 1 | 10000 | 100 |
| `research_cost` | 1 | 100000 | - |
| `movement` | 1 | 10 | - |

```lex
stats: {
    health: 5000      // ✅ Valido
    health: 50000     // ⚠️ Warning: supera il max
    health: 0         // ❌ Non valido: sotto il min
}
```

---

## 6. Codici Errore

| Codice | Categoria | Messaggio |
|--------|-----------|-----------|
| `LEX001` | Lexer | Stringa non terminata |
| `LEX002` | Lexer | Colore hex non valido |
| `LEX003` | Lexer | Formato numero non valido |
| `SYN001` | Sintassi | Parentesi graffa chiusa mancante |
| `SYN002` | Sintassi | Token inaspettato |
| `SYN003` | Sintassi | Due punti mancante |
| `SEM001` | Semantica | Riferimento non definito |
| `SEM002` | Semantica | Definizione duplicata |
| `SEM003` | Semantica | Proprietà obbligatoria mancante |
| `SEM004` | Semantica | Tipo non corrispondente |
| `SEM005` | Semantica | Dipendenza circolare |
| `LEX_IMP_001` | Validazione | Riferimento era sconosciuto |
| `LEX_IMP_002` | Validazione | Riferimento risorsa sconosciuto |
| `LEX_IMP_003` | Validazione | Riferimento tecnologia sconosciuto |
| `LEX_IMP_004` | Validazione | Riferimento struttura sconosciuto |
| `LEX_IMP_005` | Validazione | Riferimento unità sconosciuto |
| `LEX_IMP_006` | I/O | File non trovato |
| `LEX_IMP_007` | Scena | Operazione scena fallita |
| `LEX_IMP_008` | FFI | Engine non inizializzato |
| `LEX_IMP_009` | Tipo | Valore enum non valido |
| `LEX_IMP_010` | Schema | Nessuna era definita (warning) |
| `LEX_IMP_011` | Schema | Nessuna risorsa definita (warning) |

---

## 7. Formato Errore

Tutti gli errori sono restituiti come JSON strutturato:

```json
{
  "severity": "error",
  "code": "LEX_IMP_001",
  "category": "validation",
  "message": "Riferimento era sconosciuto: 'Medieval'",
  "file": "structures.lex",
  "line": 15,
  "column": 10,
  "suggestion": "Definisci l'era 'Medieval' o controlla l'ortografia",
  "ai_hint": "L'era deve essere definita prima di essere referenziata in structure/unit/technology"
}
```

**Livelli di severità:**
- `error` - Compilazione fallita
- `warning` - Compilato ma può causare problemi
- `info` - Solo informativo
