# Specifica del Linguaggio Lex

**Versione:** 0.3.2

---

## 1. Concetti Fondamentali

Lex è **dichiarativo** — descrivi il COSA, non il COME.

```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

Il compilatore genera l'implementazione.

---

## 2. Tipi

### 2.1 Primitivi

| Tipo | Esempio | Descrizione |
|------|---------|-------------|
| `int` | `50`, `-17` | Intero 64-bit |
| `float` | `1.5`, `3.14` | Float 64-bit |
| `string` | `"Farm"` | Stringa UTF-8 |
| `bool` | `true`, `false` | Booleano |
| `color` | `#FF0000` | Colore esadecimale |
| `null` | `null` | Valore nullo |

### 2.2 Compositi

| Tipo | Esempio | Descrizione |
|------|---------|-------------|
| `ResourceMap` | `{ Gold: 50, Wood: 20 }` | Risorsa → quantità |
| `ReferenceList` | `[SteamEngine, Metallurgy]` | Riferimenti incrociati |
| `Tuple` | `(10, 0, 5)` | Posizione, rotazione |

---

## 3. Keyword

### 3.1 Definizioni

| Keyword | Scopo | Proprietà Obbligatorie |
|---------|-------|------------------------|
| `era` | Era storica | `name` |
| `structure` | Edificio | `era` |
| `unit` | Unità militare/civile | `era`, `type` |
| `technology` | Tecnologia ricercabile | `research_cost` |
| `resource` | Risorsa di gioco | `name` |
| `choice` | Scelta narrativa | `text`, `options` |
| `ending` | Finale di gioco | `type` |
| `event` | Evento di gioco | `trigger` |

### 3.2 Proprietà

| Keyword | Tipo | Usato In |
|---------|------|----------|
| `name` | string | Tutti |
| `description` | string | Tutti |
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

### 3.3 Condizionali

| Keyword | Scopo |
|---------|-------|
| `when` | Condizione temporale |
| `if` | Condizione generica |
| `available_if` | Condizione di disponibilità |
| `active_if` | Condizione di attivazione |
| `condition` | Espressione booleana |

### 3.4 Operatori

| Operatore | Significato | Esempio |
|-----------|-------------|---------|
| `==` | Uguale | `era == Steampunk` |
| `!=` | Diverso | `status != AtWar` |
| `>` `<` `>=` `<=` | Confronto | `population > 100` |
| `and` `or` `not` | Logici | `a and b` |
| `+` `-` `*` `/` | Aritmetici | `cost * 1.5` |

---

## 4. Definizioni

### 4.1 Era

```lex
era <Identificatore> {
    name: <string>
    description: <string>          // opzionale
    period: <string>               // opzionale, es. "1850-1900"
    atmosphere: <string>           // opzionale
    music: <string>                // opzionale
    dominant_color: <color>        // opzionale
    transition: { ... }            // opzionale
}
```

**Obbligatorio:** `name`

**Esempio:**
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
structure <Identificatore> {
    era: <riferimento>             // OBBLIGATORIO
    name: <string>                 // opzionale
    description: <string>          // opzionale
    
    cost: { <ResourceMap> }        // opzionale
    production: { <ResourceMap> }  // opzionale
    maintenance: { <ResourceMap> } // opzionale
    
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
        condition: <espressione>
        message: <string>
    }
}
```

**Obbligatorio:** `era`

**Esempio:**
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
unit <Identificatore> {
    era: <riferimento>             // OBBLIGATORIO
    type: <identificatore>         // OBBLIGATORIO: military, civilian, hero
    
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

**Obbligatorio:** `era`, `type`

**Esempio:**
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
technology <Identificatore> {
    research_cost: <int>           // OBBLIGATORIO
    research_time: <int>           // opzionale (turni)
    
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

**Obbligatorio:** `research_cost`

**Esempio:**
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
resource <Identificatore> {
    name: <string>                 // OBBLIGATORIO
    category: <identificatore>     // opzionale: basic, luxury, strategic
    description: <string>
}
```

**Obbligatorio:** `name`

**Esempio:**
```lex
resource Coal {
    name: "Coal"
    category: strategic
    description: "Essential for steam power"
}
```

### 4.6 Entity (Integrazione Engine)

Per definizioni scene/entità in Imperium Engine:

```lex
entity <Identificatore> {
    pos: (<float>, <float>, <float>)    // OBBLIGATORIO: posizione x, y, z
    faction: <string>                    // OBBLIGATORIO: "Player", "Enemy", "Neutral"
    
    rot: (<float>, <float>, <float>, <float>)  // opzionale: quaternione
    scale: (<float>, <float>, <float>)         // opzionale
    health: <int>                              // opzionale, default: 100
    model: <string>                            // opzionale: percorso .glb
}
```

**Obbligatorio:** `pos`, `faction`

**Esempio:**
```lex
entity Warrior {
    pos: (10.0, 0.0, 5.0)
    faction: "Player"
    health: 100
    model: "assets/models/warrior.glb"
}
```

---

## 5. Espressioni

### 5.1 Booleane

```lex
condition: population > 100 and happiness < 40
condition: era == Steampunk or era == Cyberpunk
condition: not has_war_with(Neighbor)
condition: civilization.total_industry >= 100
```

### 5.2 Aritmetiche

```lex
production: { Energy: 15 + 5 }        // 20
cost: { Gold: base_cost * 1.5 }       // moltiplicatore
effect: { Happiness: +25 }            // modificatore
```

---

## 6. Commenti

```lex
// Commento su una riga

/*
   Commento
   su più righe
*/
```

---

## 7. Esempio Completo

```lex
// Definisci prima l'era
era Steampunk {
    name: "Steampunk Era"
    period: "1850-1900"
    dominant_color: #8B6914
}

// Definisci le risorse
resource Coal {
    name: "Coal"
    category: strategic
}

resource Steel {
    name: "Steel"
    category: strategic
}

// Definisci la tecnologia
technology SteamEngine {
    name: "Steam Engine"
    research_cost: 450
    
    unlocks: {
        structures: [SteamFactory]
    }
}

// Definisci la struttura
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

// Definisci l'unità
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
