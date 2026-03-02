# Lex — Visione del Progetto

**Versione:** 0.3.0
**Data:** 2026-03-02
**Stato:** 🟢 Definito

---

## Cos'è Lex

Lex è il linguaggio unificato dell'ecosistema Imperium. Non è un semplice transpiler né un linguaggio general purpose — è il punto di contatto tra tutti i sistemi del progetto: engine C++, editor Rust, gameplay Lua, UI React, agenti AI Python, e GPU compute.

**Principio fondamentale:**
> Lex non sostituisce i sistemi esistenti. Li astrae, li compone, e li protegge.

---

## L'Ecosistema Imperium

Lex si integra con uno stack già esistente e maturo:

```
┌─────────────────────────────────────────────────────┐
│                    IMPERIUM                         │
│                                                     │
│  Engine Core    →  C++                              │
│  Editor         →  Rust                             │
│  Gameplay       →  Lua (via sol2)                   │
│  UI             →  React/TypeScript                 │
│  AI/LLM         →  Python                           │
│  Rendering      →  Vulkan nativo + RenderGraph      │
│  Neural         →  da costruire (DLSS/XeSS/FSR)     │
└─────────────────────────────────────────────────────┘
```

Lex non sostituisce nessuno di questi — genera codice per ognuno.

---

## Perché Lex Esiste

### Senza Lex
Per aggiungere una struttura al gioco un modder deve:
1. Creare il file JSON per i dati
2. Creare lo script Lua per la logica
3. Creare il componente React per la UI
4. Sperare che tutto sia sincronizzato

Per te sviluppatore, configurare un nuovo sistema richiede toccare C++, Rust, Lua, e Python in punti diversi.

### Con Lex
Un file `.lex` descrive l'intenzione. Il compilatore genera tutto il resto automaticamente per ogni target. I modder non toccano mai il core. Tu hai un linguaggio unificato per configurare sistemi complessi.

---

## Target Utenti

### 1. Modder Community (Lex Base)
- Vogliono aggiungere contenuti (strutture, unità, tecnologie)
- Non vogliono imparare 4 linguaggi
- Non possono rompere i sistemi fondamentali
- Target: Lua, JSON, React

### 2. Sviluppatori Core (Lex Core)
- Estendono l'architettura di base
- Hanno accesso a C++, Rust, API engine
- Definiscono sistemi di gioco complessi
- Target: C++, Rust, Lua avanzato

### 3. Graphics Developers (Lex Shader)
- Definiscono materiali e shader
- Interfaccia dichiarativa al RenderGraph
- Target: GLSL/SPIR-V, chiamate C++

### 4. AI Agents (Aether)
- Generato automaticamente dal compilatore
- Contesto strutturato per Archivista e God AI
- Target: Python bridge

---

## Filosofia di Design

### 1. Semplicità Prima di Tutto
- Una feature si può scrivere in modo più semplice? Quella è la sintassi corretta
- Zero boilerplate non necessario
- Defaults sensati per tutto

### 2. Dichiarativo, Non Imperativo
- Descrivi **cosa esiste**, non come deve essere eseguito
- Il compilatore genera il codice imperativo
- Il modder non deve gestire stati, loop, ottimizzazioni

### 3. Multi-Target Naturale
- Un file `.lex` genera codice per tutti i layer
- Lua per gameplay logic
- React/TypeScript per UI
- JSON per data definitions
- C++ bindings per integrazione engine
- Lore context per AI companion

### 4. Leggibilità Umana
- Keyword in inglese per compatibilità internazionale
- Commenti e descrizioni in italiano per accessibilità locale
- Contesto implicito (non serve ripetere informazioni)
- Esempi autoesplicativii

### 5. Errori che Spiegano, Non Accusino
- Ogni errore dice cosa correggere
- Suggerimenti concreti
- Posizione precisa nel codice

---

## Ambizione vs Fattibilità

### Ambizione
- Linguaggio generico, riutilizzabile in altri progetti
- Community-driven development
- Tooling completo (LSP, debugger, IDE support)

### Fattibilità (MVP)
- Focus iniziale: integrazione Imperium
- Transpiler Lex → Lua (backend principale)
- Generatori secondari per JSON, React, Lore
- Testing con modder reali prima di generalizzare

---

## Principi di Successo

### 1. Non Sostituire l'Esistente
- Lex **aggiunge** capacità, non rimpiazza sistemi funzionanti
- Coesiste con Lua esistente (side-by-side)
- Gradual migration path

### 2. Non Bloccare il Futuro
- Design estensibile (nuovi backend, nuovi target)
- Plugin system per community
- Non accoppiato a Imperium-specific features

### 3. Testing Pratico
- Prima funziona per Imperium
- Poi generalizza
- Community feedback loop

---

## Metriche di Avanzamento

| Componente | Stato Attuale | Target Fase 1 |
|------------|---------------|---------------|
| Lexer | ✅ Completo | ✅ |
| Parser base | ✅ Funzionante | ✅ |
| Expression parser | ❌ Mancante | ✅ |
| AST tipizzato | 🟡 Generico | ✅ |
| Type checker | ❌ Mancante | ✅ |
| Lua backend | 🟡 Parziale | ✅ |
| JSON backend | ✅ Funzionante | ✅ |
| CLI | ✅ Completo | ✅ |
| Test | 🟡 17 test | 40+ test |
| Lex Core | ❌ Non iniziato | — |
| Lex Shader | ❌ Non iniziato | — |
| Lex Neural | ❌ Non iniziato | — |
| Aether | ❌ Non iniziato | — |

**Fase 1 - Core Solido:**
- [ ] Expression parser con precedenza operatori
- [ ] AST tipizzato per tipi base
- [ ] Type checker base
- [ ] Lua backend completo con condizioni reali
- [ ] Test end-to-end su mod reale

**Fase 2 - Lex Core:**
- [ ] Backend C++ per bindings engine
- [ ] Backend Rust per interfacce editor
- [ ] Sistema tipi avanzato

**Fase 3 - Lex Shader:**
- [ ] Backend RenderGraph
- [ ] Backend GLSL

**Fase 4 - Lex Neural:**
- [ ] Design API C++ Neural
- [ ] Backend DLSS/XeSS/FSR

**Fase 5 - Aether:**
- [ ] Generazione contesto automatica
- [ ] Bridge Python per agenti

---

## Principi Guida

**Lex non sostituisce, astrae.**
Ogni sistema esistente (RenderGraph, sol2, Lua) resta invariato. Lex genera interfacce per loro.

**Costruire dal basso.**
Core solido prima di feature avanzate. Un expression parser funzionante vale più di dieci layer non implementati.

**Separazione netta tra layer.**
Un modder non deve mai vedere sintassi Core. Un sviluppatore non deve mai scrivere Aether a mano.

**Test su casi reali.**
L'unico test che conta è un mod o un sistema reale di Imperium che funziona end-to-end.

---

## Cosa Lex NON È

- **Non è un replacement per C++** → Rendering core resta in C++
- **Non è un linguaggio generico** → Specifico per game content
- **Non è magic** → Genera codice, non sostituisce pensiero
- **Non è perfetto al primo tentativo** → Itereremo

---

*"Il miglior linguaggio è quello che ti permette di dire quello che vuoi, non quello che il compilatore vuole."*
