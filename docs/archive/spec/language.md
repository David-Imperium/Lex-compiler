# Lex Language Specification

**Version:** 0.3.2

---

## 1. Core Concepts

Lex is **declarative** - you describe WHAT, not HOW.

```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

The compiler generates the implementation.

---

## 2. Types

### 2.1 Primitives

| Type | Example | Description |
|------|---------|-------------|
| `int` | `50`, `-17` | 64-bit integer |
| `float` | `1.5`, `3.14` | 64-bit float |
| `string` | `"Farm"` | UTF-8 string |
| `bool` | `true`, `false` | Boolean |
| `color` | `#FF0000` | Hex color |
| `null` | `null` | Null value |

### 2.2 Composite

| Type | Example | Description |
|------|---------|-------------|
| `ResourceMap` | `{ Gold: 50, Wood: 20 }` | Resource → amount |
| `ReferenceList` | `[SteamEngine, Metallurgy]` | Cross-references |
| `Tuple` | `(10, 0, 5)` | Position, rotation |

---

## 3. Keywords

### 3.1 Definitions

| Keyword | Purpose | Required Properties |
|---------|---------|---------------------|
| `era` | Historical era | `name` |
| `structure` | Building | `era` |
| `unit` | Military/civilian unit | `era`, `type` |
| `technology` | Researchable tech | `research_cost` |
| `resource` | Game resource | `name` |
| `choice` | Narrative choice | `text`, `options` |
| `ending` | Game ending | `type` |
| `event` | Game event | `trigger` |

### 3.2 Properties

| Keyword | Type | Used In |
|---------|------|---------|
| `name` | string | All |
| `description` | string | All |
| `era` | reference | structure, unit, technology |
| `cost` | ResourceMap | structure, unit |
| `production` | ResourceMap | structure |
| `maintenance` | ResourceMap | structure |
| `requires` | block | structure, unit, technology |
| `unlocks` | block | technology |
| `prerequisites` | ReferenceList | technology |
| `stats` | block | unit |
| `available_if` | block | structure, unit, technology |
| `trigger` | block | event, choice, ending |

### 3.3 Conditionals

| Keyword | Purpose |
|---------|---------|
| `when` | Temporal condition |
| `if` | Generic condition |
| `available_if` | Availability condition |
| `active_if` | Activation condition |
| `condition` | Boolean expression |

### 3.4 Operators

| Operator | Meaning | Example |
|----------|---------|---------|
| `==` | Equal | `era == Steampunk` |
| `!=` | Not equal | `status != AtWar` |
| `>` `<` `>=` `<=` | Comparison | `population > 100` |
| `and` `or` `not` | Logical | `a and b` |
| `+` `-` `*` `/` | Arithmetic | `cost * 1.5` |

---

## 4. Definitions

### 4.1 Era

```lex
era <Identifier> {
    name: <string>
    description: <string>          // optional
    period: <string>               // optional, e.g. "1850-1900"
    atmosphere: <string>           // optional
    music: <string>                // optional
    dominant_color: <color>        // optional
    transition: { ... }            // optional
}
```

**Required:** `name`

**Example:**
```lex
era Steampunk {
    name: "Steampunk Era"
    period: "1850-1900"
    atmosphere: "industrial, foggy"
    dominant_color: #8B6914
}
```

### 4.2 Structure

```lex
structure <Identifier> {
    era: <reference>               // REQUIRED
    name: <string>                 // optional
    description: <string>          // optional
    
    cost: { <ResourceMap> }        // optional
    production: { <ResourceMap> }  // optional
    maintenance: { <ResourceMap> } // optional
    
    requires: {
        technologies: [<ReferenceList>]
        structures: [<ReferenceList>]
        resources: { <ResourceMap> }
    }
    
    unlocks: {
        technologies: [<ReferenceList>]
        structures: [<ReferenceList>]
        units: [<ReferenceList>]
    }
    
    available_if: {
        condition: <expression>
        message: <string>
    }
}
```

**Required:** `era`

**Example:**
```lex
structure SteamFactory {
    era: Steampunk
    name: "Steam Factory"
    
    cost: { Coal: 8, Steel: 5, Gold: 50 }
    production: { Energy: 15, Industry: 10 }
    maintenance: { Coal: 2, Gold: 5 }
    
    requires: {
        technologies: [SteamEngine]
        structures: [SteelFoundry]
    }
}
```

### 4.3 Unit

```lex
unit <Identifier> {
    era: <reference>               // REQUIRED
    type: <identifier>             // REQUIRED: military, civilian, hero
    
    name: <string>
    description: <string>
    
    stats: {
        attack: <int>
        defense: <int>
        movement: <int>
        range: <int>
    }
    
    cost: { <ResourceMap> }
    maintenance: { <ResourceMap> }
    
    abilities: [<ReferenceList>]
    requires: { ... }
}
```

**Required:** `era`, `type`

**Example:**
```lex
unit SteamTank {
    era: Steampunk
    type: military
    
    stats: {
        attack: 25
        defense: 20
        movement: 3
    }
    
    cost: { Steel: 15, Coal: 8, Gold: 150 }
    abilities: [Breakthrough, Armored]
}
```

### 4.4 Technology

```lex
technology <Identifier> {
    research_cost: <int>           // REQUIRED
    research_time: <int>           // optional (turns)
    
    name: <string>
    description: <string>
    quote: <string>
    
    prerequisites: [<ReferenceList>]
    
    unlocks: {
        technologies: [<ReferenceList>]
        structures: [<ReferenceList>]
        units: [<ReferenceList>]
    }
}
```

**Required:** `research_cost`

**Example:**
```lex
technology SteamEngine {
    name: "Steam Engine"
    research_cost: 450
    research_time: 5
    
    prerequisites: [Metallurgy, BasicThermodynamics]
    
    unlocks: {
        structures: [SteamFactory, MechanicalWorkshop]
        units: [ArmoredTrain]
    }
}
```

### 4.5 Resource

```lex
resource <Identifier> {
    name: <string>                 // REQUIRED
    category: <identifier>         // optional: basic, luxury, strategic
    description: <string>
}
```

**Required:** `name`

**Example:**
```lex
resource Coal {
    name: "Coal"
    category: strategic
    description: "Essential for steam power"
}
```

### 4.6 Entity (Engine Integration)

For scene/entity definitions in Imperium Engine:

```lex
entity <Identifier> {
    pos: (<float>, <float>, <float>)    // REQUIRED: position x, y, z
    faction: <string>                    // REQUIRED: "Player", "Enemy", "Neutral"
    
    rot: (<float>, <float>, <float>, <float>)  // optional: quaternion
    scale: (<float>, <float>, <float>)         // optional
    health: <int>                              // optional, default: 100
    model: <string>                            // optional: path to .glb
}
```

**Required:** `pos`, `faction`

**Example:**
```lex
entity Warrior {
    pos: (10.0, 0.0, 5.0)
    faction: "Player"
    health: 100
    model: "assets/models/warrior.glb"
}
```

---

## 5. Expressions

### 5.1 Boolean

```lex
condition: population > 100 and happiness < 40
condition: era == Steampunk or era == Cyberpunk
condition: not has_war_with(Neighbor)
condition: civilization.total_industry >= 100
```

### 5.2 Arithmetic

```lex
production: { Energy: 15 + 5 }        // 20
cost: { Gold: base_cost * 1.5 }       // multiplier
effect: { Happiness: +25 }            // modifier
```

---

## 6. Comments

```lex
// Single line comment

/*
   Multi-line comment
*/
```

---

## 7. Full Example

```lex
// Define era first
era Steampunk {
    name: "Steampunk Era"
    period: "1850-1900"
    dominant_color: #8B6914
}

// Define resources
resource Coal {
    name: "Coal"
    category: strategic
}

resource Steel {
    name: "Steel"
    category: strategic
}

// Define technology
technology SteamEngine {
    name: "Steam Engine"
    research_cost: 450
    
    unlocks: {
        structures: [SteamFactory]
    }
}

// Define structure
structure SteamFactory {
    era: Steampunk
    name: "Steam Factory"
    
    cost: { Coal: 8, Steel: 5, Gold: 50 }
    production: { Energy: 15, Industry: 10 }
    maintenance: { Coal: 2, Gold: 5 }
    
    requires: {
        technologies: [SteamEngine]
    }
    
    available_if: {
        condition: civilization.total_industry >= 100
        message: "Your industry is developed enough"
    }
}

// Define unit
unit SteamTank {
    era: Steampunk
    type: military
    
    stats: {
        attack: 25
        defense: 20
        movement: 3
    }
    
    cost: { Steel: 15, Coal: 8, Gold: 150 }
}
```
