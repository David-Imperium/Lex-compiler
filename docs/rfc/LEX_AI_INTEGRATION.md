# Lex AI Integration Guide

**Versione:** 0.1.0  
**Data:** 2026-03-01  
**Target:** AI Agents (Claude, GPT-4, Copilot, etc.)

---

## Overview

Lex usa keyword in **inglese** come unica lingua per il codice. Questo documento for una reference velocalizzata per gli AI agent che generano codice Lex.

### Core Constructs

| Keyword | Token | Descrizione | Esempio |
|---------|-------|-------------|---------|
| `structure` | STRUCTURE | Definisce una struttura/edificio | `structure Farm { }` |
| `unit` | UNIT | Definisce un'unità militare o civile | `unit Tank { }` |
| `technology` | TECHNOLOGY | Definisce una tecnologia ricercabile | `technology SteamEngine { }` |
| `era` | ERA | Definisce un'era storica | `era Steampunk { }` |
| `choice` | CHOICE | Definisce una scelta narrativa | `choice Revolution { }` |
| `ending` | ENDING | Definisce un finale del gioco | `ending SecretEnding { }` |
| `event` | EVENT | Definisce un evento | `event IndustrialAge { }` |
| `resource` | RESOURCE | Definisce una risorsa | `resource Coal { }` |

### Properties

| Keyword | Token | Descrizione | Esempio |
|---------|-------|-------------|---------|
| `name` | NAME | Nome visualizzato | `name: "Steam Factory"` |
| `era` | ERA | Era di appartenenza | `era: Steampunk` |
| `cost` | COST | Costo di costruzione | `cost: { Gold: 50, Wood: 20 }` |
| `production` | PRODUCTION | Produzione per turno | `production: { Food: 5 }` |
| `maintenance` | MAINTENANCE | Costo mantenimento | `maintenance: { Coal: 2 }` |
| `description` | DESCRIPTION | Descrizione testuale | `description: "Produces food"` |
| `quote` | QUOTE | Citazione storica | `quote: "Steam is power"` |

### Conditions

| Keyword | Token | Descrizione | Esempio |
|---------|-------|-------------|---------|
| `when` | WHEN | Condizione temporale | `when civilization.population > 100` |
| `if` | IF | Condizione generica | `if has_technology(SteamEngine)` |
| `available_if` | AVAILABLE_IF | Condizione disponibilità | `available_if { ... }` |
| `secret_if` | SECRET_IF | Condizione segreto | `secret_if { ... }` |
| `trigger` | TRIGGER | Trigger evento | `trigger { condition: ... }` |

### Logic Operators

| Keyword | Token | Descrizione | Esempio |
|---------|-------|-------------|---------|
| `and` | AND | AND logico | `condition1 and condition2` |
| `or` | OR | OR logico | `condition1 or condition2` |
| `not` | NOT | NOT logico | `not has_flag(flag_war)` |
| `true` | TRUE | Valore booleano true | `enabled: true` |
| `false` | FALSE | Valore booleano false | `hidden: false` |

### Actions

| Keyword | Token | Descrizione | Esempio |
|---------|-------|-------------|---------|
| `activate` | ACTIVATE | Attiva evento/segreto | `activate: IndustrialConspiracy` |
| `deactivate` | DEACTIVATE | Disattiva qualcosa | `deactivate: OldSystem` |
| `unlock` | UNLOCK | Sblocca contenuti | `unlock: { technologies: [...] }` |
| `lock` | LOCK | Blocca contenuti | `lock: { units: [...] }` |
| `flag` | FLAG | Imposta flag narrativo | `flag: flag_revolution_happened` |

### Other

| Keyword | Token | Descrizione | Esempio |
|---------|-------|-------------|---------|
| `archivist` | ARCHIVIST | Messaggio companion AI | `archivist: "Something is coming..."` |
| `lore` | LORE | Contesto narrativo | `lore: { ... }` |
| `ui` | UI | Configurazione interfaccia | `ui: { icon: "farm.png" }` |
| `requires` | REQUIRES | Prerequisiti | `requires: { technologies: [...] }` |
| `unlocks` | UNLOCKS | Cosa sblocca | `unlocks: { structures: [...] }` |

---

## Perché Inglese?

### 1. Training Data
I modelli AI sono addestrati principalmente su codice inglese. Le keyword inglesi sono pattern riconosci nativamente.

### 2. Consistency
Tutti gli strumenti AI (GitHub Copilot, GPT-4, Claude, etc.) funzionano meglio con l'inglese. Meno traduzione mentale = meno errori.

### 3. Universal Access
L'inglese funziona per la community internazionale. Pi accessibile per modder di tutto il mondo.

### 4. Tool Integration
LSP, syntax highlighter, validatori: tutti lavorano meglio con keyword inglesi standardizzate.

---

## Perché Inglese Only per AI?

### 1. Training Data
I modelli AI sono addestrati principalmente su codice inglese. Le keyword inglesi sono pattern riconosciuti nativamente.

### 2. Consistency
Tutti gli strumenti AI (GitHub Copilot, GPT-4, Claude, etc.) funzionano meglio con l'inglese. Meno traduzione mentale = meno errori.

### 3. Error Reduction
Eliminare la traduzione riduce gli errori. L'AI non deve ricordare che `struttura` = `structure`.

### 4. Universal Access
L'inglese funziona per la community internazionale. Se un modder tedesco usa Lex, l'inglese è più accessibile dell'italiano.

### 5. Tool Integration
LSP, syntax highlighter, validatori: tutti lavorano meglio con keyword inglesi standardizzate.

---

## Esempi Corretti (Per AI Agents)

### ✅ Esempio 1: Struttura Base

```lex
structure Farm {
    era: Ancient
    name: "Farm"
    
    cost: {
        Gold: 50,
        Wood: 20
    }
    
    production: {
        Food: 5
    }
    
    description: "A farm that produces food for your civilization"
}
```

### ✅ Esempio 2: Struttura con Condizioni

```lex
structure SteamFactory {
    era: Steampunk
    name: "Steam Factory"
    
    cost: {
        Coal: 8,
        Steel: 5,
        Gold: 50
    }
    
    production: {
        Energy: 15,
        Industry: 10
    }
    
    maintenance: {
        Coal: 2,
        Gold: 5
    }
    
    requires: {
        technologies: [SteamEngine],
        structures: [SteelFoundry]
    }
    
    unlocks: {
        technologies: [AdvancedSteam, ZeppelinTransport],
        structures: [MechanicalWorkshop]
    }
    
    available_if: {
        condition: civilization.total_industry >= 100,
        message: "Your industry is developed enough for a steam factory"
    }
    
    secret_if: {
        condition: civilization.has_structure_in_every_city(SteamFactory),
        activate: IndustrialConspiracy,
        archivist: "Someone is gathering too much power in the factories..."
    }
    
    ui: {
        icon: "assets/steampunk/steam_factory.png",
        color: #8B6914
    }
    
    lore: {
        quote: "Steam transforms iron into power",
        context: "Critical structure for the Steampunk era"
    }
}
```

### ✅ Esempio 3: Tecnologia

```lex
technology SteamEngine {
    era: Steampunk
    name: "Steam Engine"
    
    research_cost: 450
    
    requires: {
        technologies: [Metallurgy, BasicThermodynamics]
    }
    
    unlocks: {
        structures: [SteamFactory, MechanicalWorkshop],
        units: [ArmoredTrain],
        bonuses: ["Industrial production +25%"]
    }
    
    quote: "Steam transforms iron into power - dixit Brunel"
}
```

### ✅ Esempio 4: Scelta Narrativa

```lex
choice WorkerRevolution {
    era: Steampunk
    
    trigger: {
        condition: civilization.population > 100 and civilization.happiness < 40,
        probability: 30%
    }
    
    text: "Factory workers are organizing. They demand better conditions and higher wages. How do you respond?"
    
    options: {
        Suppress: {
            text: "Crush the worker movement with force"
            
            effect: {
                Industry: +30,
                Happiness: -20,
                Health: -5
            }
            
            flag: flag_revolution_suppressed
            
            archivist: "You chose the path of iron. Social peace is maintained, but at what cost?"
        }
        
        Accept: {
            text: "Accept the workers' demands"
            
            effect: {
                Happiness: +25,
                Industry: -10,
                Gold: -200
            }
            
            flag: flag_workers_rights
            
            archivist: "You made the right choice. Your citizens will work better, knowing they are heard."
        }
    }
}
```

### ✅ Esempio 5: Era Completa

```lex
era Steampunk {
    name: "Steampunk Era"
    period: "1850-1900"
    description: "The age of steam, mechanical engineering, and industrial revolution"
    
    atmosphere: "industrial, foggy, mechanical"
    music: "steampunk_ost"
    dominant_color: #8B6914
    
    transition: {
        requires: {
            minimum_technologies: 12,
            resources: { Energy: 1000, Industry: 500 }
        },
        
        cinematic: "SteampunkTransitionCinematic",
        
        archivist: "The smell of coal and oil fills the air. A new era has begun—the age of machines."
    }
    
    // Structures defined inline or referenced
    structures: {
        SteamFactory: { /* ... */ },
        MechanicalWorkshop: { /* ... */ },
        CoalRefinery: { /* ... */ }
    }
    
    technologies: {
        SteamEngine: { /* ... */ },
        Electromagnetism: { /* ... */ },
        Aeronautics: { /* ... */ }
    }
}
```

---

## Esempi Corretti

## Identificatori

### Regola

**Tutti gli IDENTIFIER (nomi di strutture, tecnologie, risorse) DEVONO essere in inglese per coerenza.**

**Le KEYWORD sono in inglese (unico).**

### Esempio

```lex
structure SteamFactory {        // ✅ English identifier
    era: Steampunk
    name: "Steam Factory"       // ✅ Display name
    
    cost: { Coal: 8, Steel: 5, Gold: 50 }
}
```

---

## Workflow per AI Agents

### Quando David Chiede in Italiano

**Input:**
```
David: "Crea una struttura per l'era Steampunk che produca energia"
```

**AI Process:**
1. Comprende: needs structure for Steampunk era that produces energy
2. Genera con keyword INGLESI:
   ```lex
   structure SteamGenerator {
       era: Steampunk
       name: "Steam Generator"
       
       cost: {
           Coal: 10,
           Steel: 8,
           Gold: 80
       }
       
       production: {
           Energy: 20
       }
       
       description: "Generates energy from steam power"
   }
   ```
3. Output: SEMPRE keyword inglesi

### Quando David Chiede in Inglese

**Input:**
```
David: "Create a farm structure for the Ancient era"
```

**AI Process:**
1. Generate directly in English:
   ```lex
   structure Farm {
       era: Ancient
       name: "Farm"
       
       cost: {
           Gold: 50,
           Wood: 20
       }
       
       production: {
           Food: 5
       }
   }
   ```

---

## Schemas per AI Validation

### Structure Schema

```json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "type": "object",
  "required": ["era", "cost", "production"],
  "properties": {
    "era": {
      "type": "string",
      "description": "The era this structure belongs to"
    },
    "name": {
      "type": "string",
      "description": "Display name of the structure"
    },
    "cost": {
      "type": "object",
      "description": "Resource costs to build",
      "additionalProperties": {
        "type": "number"
      }
    },
    "production": {
      "type": "object",
      "description": "Resources produced per turn",
      "additionalProperties": {
        "type": "number"
      }
    },
    "maintenance": {
      "type": "object",
      "description": "Ongoing costs per turn",
      "additionalProperties": {
        "type": "number"
      }
    },
    "description": {
      "type": "string",
      "description": "Human-readable description"
    },
    "requires": {
      "type": "object",
      "properties": {
        "technologies": {
          "type": "array",
          "items": { "type": "string" }
        },
        "structures": {
          "type": "array",
          "items": { "type": "string" }
        }
      }
    },
    "unlocks": {
      "type": "object",
      "properties": {
        "technologies": {
          "type": "array",
          "items": { "type": "string" }
        },
        "structures": {
          "type": "array",
          "items": { "type": "string" }
        },
        "units": {
          "type": "array",
          "items": { "type": "string" }
        }
      }
    }
  }
}
```

---

## Checklist per AI Agents

Prima di generare codice Lex, verificare:

- [ ] Tutte le keyword sono in INGLESE
- [ ] Sintassi valida secondo schema
- [ ] Identificatori in inglese per coerenza
- [ ] Costi e produzioni sono mappe `{ Resource: Amount }`
- [ ] Condizioni usano `and`, `or`, `not`
- [ ] Booleani sono `true`/`false`

---

## Risorse Aggiuntive

### Documentazione
- `LEX_SPECIFICATION.md` - Specifiche complete
- `LEX_KEYWORDS.md` - Reference keyword completa
- `examples/` - Esempi

### Schemas
- `schemas/structure.schema.json`
- `schemas/unit.schema.json`
- `schemas/technology.schema.json`
- `schemas/era.schema.json`

---

## Contatti e Supporto

Per problemi o domande sull'integrazione AI:
- Aprire issue su GitHub (quando disponibile)
- Consultare documentazione ufficiale
- Contattare maintainer

---

**Ricorda:** Quando in dubbio, usa SEMPRE le keyword inglesi. È più sicuro e più compatibile.

---

*Ultimo aggiornamento: 2026-03-01*
