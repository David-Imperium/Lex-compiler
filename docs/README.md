# Lex Documentation

**Versione:** 0.1.0-alpha  
**Ultimo aggiornamento:** 2026-03-01

---

## 📚 Documentazione Completa

### 🎯 Concettuali

| File | Descrizione |
|------|-------------|
| [LEX_VISION.md](LEX_VISION.md) | Visione del progetto, filosofia, obiettivi |
| [LEX_CHANGELOG.md](LEX_CHANGELOG.md) | Tracciamento cambiamenti, roadmap, metriche |

### 📖 Tecnici (Da Creare)

| File | Descrizione | Stato |
|------|-------------|-------|
| `LEX_SPECIFICATION.md` | Specifiche linguistiche complete | 🟡 Da scrivere |
| `LEX_ARCHITECTURE.md` | Architettura compilatore | 🟡 Da scrivere |
| `LEX_INTEGRATION.md` | Integrazione con Imperium | 🟡 Da scrivere |

### 📁 Esempi Pratici

| File | Descrizione | Complessità |
|------|-------------|-------------|
| [examples/basic_structure.lex](examples/basic_structure.lex) | Struttura semplice | 🟢 Base |
| [examples/advanced_structure.lex](examples/advanced_structure.lex) | Struttura con condizioni, eventi, UI | 🟡 Media |
| [examples/era_definition.lex](examples/era_definition.lex) | Definizione era completa | 🔴 Avanzata |

---

## 🚀 Quick Start

### Cos'è Lex?

Lex è un linguaggio dichiarativo multi-target per il modding di giochi complessi.

**Principio:** *"Descrivi il mondo, non come costruirlo."*

### Perché Lex?

**Senza Lex (Modding Tradizionale):**
```
1. Modder vuole aggiungere una struttura
2. Crea data/building.json (dati)
3. Crea scripts/building.lua (logica)
4. Crea ui/BuildingPanel.jsx (UI)
5. Crea ai/lore/building.txt (AI context)
6. Spera che tutto sia sincronizzato
```

**Con Lex:**
```
1. Modder crea building.lex
2. Compilatore genera tutti i file automaticamente
3. Modder modifica solo dove serve
```

### Esempio Minimo

```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50, Wood: 20 }
    production: { Food: 5 }
}
```

Questo genera automaticamente:
- `data/buildings/fattoria.json`
- `scripts/lua/buildings/fattoria.lua`
- `ui/buildings/FattoriaPanel.tsx`
- `ai/lore/buildings/fattoria.txt`

---

## 🎯 Target Utenti

### 1. Modder
- Aggiungono contenuti senza imparare 4 linguaggi
- Vedono risultati immediati
- Non rompono sistemi core

### 2. Designer
- Definiscono regole in linguaggio leggibile
- Sperimentano rapidamente
- Focus su creatività, non su implementazione

### 3. Developer
- Integrano nuovi sistemi facilmente
- Estendono il compilatore
- Mantengono type safety

---

## 🏗️ Architettura

```
File .lex (Sorgente)
    ↓
Lexer (Tokenization)
    ↓
Parser (AST Generation)
    ↓
Multi-Target Code Generator
    ├─→ Lua Backend → Gameplay Logic
    ├─→ JSON Backend → Data Definitions
    ├─→ React Backend → UI Components
    ├─→ Lore Backend → AI Context
    └─→ C++ Backend → Engine Bindings
```

---

## 📅 Roadmap

### [0.1.0] - Target: 2026-03-08 (1 settimana)

**Obiettivo:** MVP Transpiler Lex → Lua

**Features:**
- ✅ Documentazione base
- 🟡 Lexer + Parser
- 🟡 Lua code generator
- 🟡 CLI tool (`lexc`)
- 🟡 Test con mod Imperium

### [0.2.0] - Target: 2026-03-22 (3 settimane)

**Obiettivo:** Multi-target code generation

**Features:**
- [ ] JSON backend
- [ ] React backend
- [ ] Lore backend
- [ ] Hot-reload

### [0.3.0] - Target: 2026-04-01 (1 mese)

**Obiettivo:** Tooling e Developer Experience

**Features:**
- [ ] VS Code extension
- [ ] Language Server Protocol
- [ ] Error messages user-friendly
- [ ] Debug mode

### [1.0.0] - Target: TBD

**Obiettivo:** Release pubblica

**Features:**
- [ ] Documentazione completa
- [ ] Tutorial interattivo
- [ ] Example projects
- [ ] Community support

---

## 🔗 Risorse

### File Principali
- `LEX_VISION.md` - Visione e filosofia
- `LEX_CHANGELOG.md` - Tracciamento cambiamenti
- `progress.md` - Progresso sviluppo

### Esempi
- `examples/basic_structure.lex` - Partire da qui
- `examples/advanced_structure.lex` - Feature complete
- `examples/era_definition.lex` - Definizioni complesse

### Esterno
- [Imperium 2.0](file:///C:/Imperium%202.0/) - Progetto principale
- [Lua Engine 2.0](file:///C:/Imperium%202.0/docs/technical/2.0/core/LUA_ENGINE_2.0.md) - Documentazione Lua

---

## 🤝 Contribuire

**Stato:** 🟡 Pre-alpha (sviluppo interno)

Al momento Lex è in fase di sviluppo iniziale. Contributions sono benvenute dopo il rilascio della v0.1.0.

### Codice di Condotta
- Semplicità > potenza
- Documenta tutto
- Testa con utenti reali
- Non rompere il passato

---

## 📊 Metriche

| Metrica | Valore Attuale | Target v0.1.0 |
|---------|----------------|---------------|
| Righe Documentazione | ~400 | ~1000 |
| Righe Codice | 0 | ~5000 |
| Test Coverage | 0% | 80% |
| Backend Supportati | 0 | 1 (Lua) |
| Esempi | 3 | 10+ |

---

## ⚠️ Stato del Progetto

**ATTENZIONE:** Lex è in fase pre-alpha. Tutto può cambiare.

- ❌ Non usare in produzione
- ❌ Non promettere feature che non esistono
- ✅ Sperimenta e fornisci feedback
- ✅ Contribuisci con idee ed esempi

---

## 📝 Licenza

(TBD - Probabilmente MIT)

---

*"Il miglior linguaggio è quello che ti permette di dire quello che vuoi, non quello che il compilatore vuole."*

---

**Ultimo aggiornamento:** 2026-03-01  
**Prossimo step:** Scrivere `LEX_SPECIFICATION.md`
