# Lex Keyword Reference — Bilingue / Bilingual

**Versione:** 0.1.0  
**Ultimo aggiornamento:** 2026-03-01

---

## Overview / Panoramica

Lex supporta keyword in **due lingue**: italiano e inglese.

**Per modder umani:** Potete usare la lingua che preferite.  
**Per AI agents:** Dovete usare SOLO le keyword inglesi.

---

## Legenda / Legend

| Simbolo | Significato |
|---------|-------------|
| 🇮🇹 | Keyword italiana (per umani) |
| 🇬🇧 | Keyword inglese (per tutti, OBBLIGATORIA per AI) |
| ⚠️ | Attenzione: stesso spelling in entrambe le lingue |
| 🤖 | AI agents DEVONO usare questa versione |

---

## Core Constructs / Costrutti Base

### Strutture / Structures

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `struttura` | `structure` | STRUCTURE | Definisce un edificio |
| `unità` | `unit` | UNIT | Definisce un'unità |
| `tecnologia` | `technology` | TECHNOLOGY | Definisce una tecnologia |
| `risorsa` | `resource` | RESOURCE | Definisce una risorsa |
| `era` ⚠️ | `era` ⚠️ | ERA | Definisce un'era storica |

### Narrazione / Narrative

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `scelta` | `choice` | CHOICE | Scelta narrativa |
| `finale` | `ending` | ENDING | Finale del gioco |
| `evento` | `event` | EVENT | Evento generico |
| `segreto` | `secret` | SECRET | Contenuto segreto |

### Companion AI

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `archivista` | `archivist` | ARCHIVIST | Messaggio companion |
| `lore` ⚠️ | `lore` ⚠️ | LORE | Contesto narrativo |

---

## Properties / Proprietà

### Identificazione

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `nome` | `name` | NAME | Nome visualizzato |
| `id` ⚠️ | `id` ⚠️ | ID | Identificatore unico |
| `descrizione` | `description` | DESCRIPTION | Descrizione testuale |
| `citazione` | `quote` | QUOTE | Citazione storica |

### Economia

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `costo` | `cost` | COST | Costo costruzione |
| `produzione` | `production` | PRODUCTION | Produzione per turno |
| `mantenimento` | `maintenance` | MAINTENANCE | Costo mantenimento |
| `consumo` | `consumption` | CONSUMPTION | Consumo risorse |

### Requisiti e Sblocco

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `requisiti` | `requires` | REQUIRES | Prerequisiti necessari |
| `sblocca` | `unlocks` | UNLOCKS | Cosa sblocca |
| `blocca` | `locks` | LOCKS | Cosa blocca |

### Stato

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `era` ⚠️ | `era` ⚠️ | ERA | Era di appartenenza |
| `tipo` | `type` | TYPE | Tipo di entità |
| `categoria` | `category` | CATEGORY | Categoria |
| `livello` | `level` | LEVEL | Livello/tier |

---

## Conditions / Condizioni

### Costrutti Condizionali

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `quando` | `when` | WHEN | Condizione temporale |
| `se` | `if` | IF | Condizione generica |
| `disponibile_se` | `available_if` | AVAILABLE_IF | Condizione disponibilità |
| `segreto_se` | `secret_if` | SECRET_IF | Condizione segreto |
| `attiva_se` | `active_if` | ACTIVE_IF | Condizione attivazione |

### Trigger

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `trigger` ⚠️ | `trigger` ⚠️ | TRIGGER | Definisce trigger |
| `condizione` | `condition` | CONDITION | Espressione booleana |
| `probabilità` | `probability` | PROBABILITY | Probabilità evento |

---

## Logic Operators / Operatori Logici

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione | Esempio |
|-------------|---------------|-------|-------------|---------|
| `e` | `and` | AND | AND logico | `a and b` |
| `o` | `or` | OR | OR logico | `a or b` |
| `non` | `not` | NOT | NOT logico | `not a` |
| `vero` | `true` | TRUE | Booleano true | `enabled: true` |
| `falso` | `false` | FALSE | Booleano false | `hidden: false` |

---

## Actions / Azioni

### Attivazione

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `attiva` | `activate` | ACTIVATE | Attiva evento |
| `disattiva` | `deactivate` | DEACTIVATE | Disattiva |
| `abilita` | `enable` | ENABLE | Abilita feature |
| `disabilita` | `disable` | DISABLE | Disabilita |

### Flags

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `flag` ⚠️ | `flag` ⚠️ | FLAG | Imposta flag |
| `segna` | `mark` | MARK | Marca stato |

### Modifiche

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `effetto` | `effect` | EFFECT | Definisce effetto |
| `bonus` ⚠️ | `bonus` ⚠️ | BONUS | Bonus applicato |
| `malus` ⚠️ | `malus` ⚠️ | MALUS | Malus applicato |

---

## UI Properties / Proprietà UI

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `icona` | `icon` | ICON | Percorso icona |
| `colore` | `color` | COLOR | Colore esadecimale |
| `tooltip` ⚠️ | `tooltip` ⚠️ | TOOLTIP | Testo tooltip |
| `pannello` | `panel` | PANEL | Config pannello |
| `componenti` | `components` | COMPONENTS | Componenti UI |

---

## Lore Properties / Proprietà Lore

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `citazione` | `quote` | QUOTE | Citazione storica |
| `contesto` | `context` | CONTEXT | Contesto AI |
| `riferimenti` | `references` | REFERENCES | Riferimenti storici |
| `atmosfera` | `atmosphere` | ATMOSPHERE | Atmosfera era |

---

## Statistics / Statistiche

### Unità

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `attacco` | `attack` | ATTACK | Valore attacco |
| `difesa` | `defense` | DEFENSE | Valore difesa |
| `movimento` | `movement` | MOVEMENT | Punti movimento |
| `portata` | `range` | RANGE | Portata attacco |

### Popolazione

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `popolazione` | `population` | POPULATION | Popolazione totale |
| `salute` | `health` | HEALTH | Livello salute |
| `felicità` | `happiness` | HAPPINESS | Livello felicità |
| `stabilità` | `stability` | STABILITY | Stabilità politica |

---

## Technology Tree / Albero Tecnologico

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `costo_ricerca` | `research_cost` | RESEARCH_COST | Costo ricerca |
| `prerequisiti` | `prerequisites` | PREREQUISITES | Prerequisiti tech |
| `tempo_ricerca` | `research_time` | RESEARCH_TIME | Turni ricerca |

---

## Era Properties / Proprietà Era

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `periodo` | `period` | PERIOD | Periodo storico |
| `musica` | `music` | MUSIC | Colonna sonora |
| `colore_dominante` | `dominant_color` | DOMINANT_COLOR | Colore principale |
| `transizione` | `transition` | TRANSITION | Config transizione |

---

## Comparison Operators / Operatori di Confronto

**Questi sono IDENTICI in entrambe le lingue:**

| Operatore | Significato | Esempio |
|-----------|-------------|---------|
| `>` | Maggiore di | `population > 100` |
| `<` | Minore di | `gold < 50` |
| `>=` | Maggiore o uguale | `happiness >= 70` |
| `<=` | Minore o uguale | `stability <= 30` |
| `==` | Uguale | `era == Steampunk` |
| `!=` | Diverso | `status != AtWar` |

---

## Arithmetic Operators / Operatori Aritmetici

**Identici in entrambe le lingue:**

| Operatore | Significato | Esempio |
|-----------|-------------|---------|
| `+` | Addizione | `gold + 100` |
| `-` | Sottrazione | `health - 10` |
| `*` | Moltiplicazione | `production * 1.5` |
| `/` | Divisione | `cost / 2` |
| `%` | Modulo | `turn % 10` |

---

## Assignment Operators / Operatori Assegnamento

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione | Esempio |
|-------------|---------------|-------|-------------|---------|
| `+=` | `+=` | PLUS_ASSIGN | Incrementa | `gold += 50` |
| `-=` | `-=` | MINUS_ASSIGN | Decrementa | `health -= 5` |
| `=` | `=` | ASSIGN | Assegna | `enabled = true` |
| `->` | `->` | ARROW | Transizione | `transition -> era.Cyber` |

---

## Special Keywords / Keyword Speciali

### Valori Letterali

| 🇮🇹 Italiano | 🇬🇧 Inglese 🤖 | Token | Descrizione |
|-------------|---------------|-------|-------------|
| `vero` | `true` | TRUE | Booleano vero |
| `falso` | `false` | FALSE | Booleano falso |
| `nullo` | `null` | NULL | Valore nullo |

### Quantità

| Sintassi | Significato | Esempio |
|----------|-------------|---------|
| `Risorsa x N` | Quantità risorsa | `Gold x 50` |
| `{ Risorsa: N }` | Mappa risorse | `{ Gold: 50, Wood: 20 }` |

---

## Complete Example / Esempio Completo

### Versione Italiana (Per Modder Umani)

```lex
struttura FabbricaVapore {
    era: Steampunk
    nome: "Fabbrica a Vapore"
    
    costo: {
        Carbone: 8,
        Acciaio: 5,
        Oro: 50
    }
    
    produzione: {
        Energia: 15,
        Industria: 10
    }
    
    mantenimento: {
        Carbone: 2,
        Oro: 5
    }
    
    requisiti: {
        tecnologie: [MotoreVapore],
        strutture: [FonderiaAcciaio]
    }
    
    disponibile_se {
        condizione: civiltà.industria_totale >= 100
        messaggio: "La tua industria è sviluppata"
    }
    
    descrizione: "Converte carbone in energia"
}
```

### Versione Inglese (Per AI Agents e Community Internazionale)

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
    
    available_if: {
        condition: civilization.total_industry >= 100
        message: "Your industry is developed enough"
    }
    
    description: "Converts coal into energy"
}
```

---

## Nota Importante per AI Agents

**🤖 AI Agents:**

Quando generate codice Lex, usate **ESCLUSIVAMENTE** le keyword della colonna "🇬🇧 Inglese".

Le keyword italiane esistono per i modder umani, ma causano confusione negli AI agents.

**Esempio:**
```lex
// ✅ CORRETTO per AI
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}

// ❌ ERRATO per AI (ma OK per modder italiano)
struttura Fattoria {
    era: Antica
    costo: { Oro: 50 }
    produzione: { Cibo: 5 }
}
```

Per dettagli completi, vedere `LEX_AI_INTEGRATION.md`.

---

## Note per Sviluppatori

### Implementazione Lexer

```cpp
// Entrambe le versioni mappano allo stesso token
std::map<std::string, TokenType> KEYWORDS = {
    {"struttura", TokenType::STRUCTURE},
    {"structure", TokenType::STRUCTURE},
    
    {"unità", TokenType::UNIT},
    {"unit", TokenType::UNIT},
    
    // ... (vedi keywords.h per lista completa)
};
```

### Linter Rule

Per prevenire file "mixed" (italiano + inglese nello stesso file):

```cpp
// Consigliato: file dovrebbe essere consistent
LintLevel checkLocaleConsistency(const AST& ast);
```

---

## Aggiornamenti Futuri

Questo documento sarà aggiornato quando:
- Nuove keyword vengono aggiunte
- Semantiche cambiano
- Best practices emergono

**Versione attuale:** 0.1.0 (Pre-alpha)

---

*Ultimo aggiornamento: 2026-03-01*
*Prossimo update: Quando implementazione lexer inizia*
