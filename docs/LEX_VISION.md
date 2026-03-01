# Lex — Visione del Progetto

**Versione:** 0.1.0  
**Data:** 2026-03-01  
**Stato:** 🟡 In Definizione

---

## Cos'è Lex

Lex è un linguaggio dichiarativo multi-target progettato per semplificare il modding e l'estensione di giochi complessi.

**Principio fondamentale:**  
*"Descrivi il mondo, non come costruirlo."*

---

## Perché Lex Esiste

### Il Problema

Nei giochi moderni con architetture complesse (C++ + Lua + React + AI), i modder devono:
- Imparare 3-4 linguaggi diversi
- Modificare file in 3-4 posti diversi per una singola feature
- Capire come i sistemi interagiscono tra loro
- Gestire manualmente la sincronizzazione tra layer

### La Soluzione

Lex è un **"collante linguistico"** che permette di definire contenuti di gioco in un unico posto, generando automaticamente il codice per tutti i layer necessari.

---

## Target Utenti

### 1. Modder Community
- Vogliono aggiungere contenuti (strutture, unità, tecnologie)
- Non vogliono imparare 4 linguaggi
- Vogliono vedere risultati velocemente
- Non vogliono rompere i sistemi esistenti

### 2. Designer
- Definiscono regole di gioco, finali, eventi narrativi
- Vogliono un linguaggio leggibile come prosa
- Vogliono sperimentare senza toccare codice core

### 3. Developer
- Estendono l'architettura di base
- Vogliono integrazione pulita con i sistemi esistenti
- Vogliono type safety e validazione

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

## Metriche di Successo

**Fase 1 - MVP (1-2 settimane):**
- [ ] Transpiler Lex → Lua funzionante
- [ ] Un mod complesso convertito da Lua a Lex
- [ ] Modder riesce a usare Lex senza documentazione estesa

**Fase 2 - Integrazione (1 mese):**
- [ ] Generatori per JSON, React, Lore
- [ ] Hot-reload durante sviluppo
- [ ] 3+ mods creati con Lex

**Fase 3 - Community (3+ mesi):**
- [ ] Open source su GitHub
- [ ] Documentazione completa
- [ ] VS Code extension
- [ ] Community attiva

---

## Cosa Lex NON È

- **Non è un replacement per C++** → Rendering core resta in C++
- **Non è un linguaggio generico** → Specifico per game content
- **Non è magic** → Genera codice, non sostituisce pensiero
- **Non è perfetto al primo tentativo** → Itereremo

---

## Prossimi Passi

1. Definire specifiche linguistiche (`LEX_SPECIFICATION.md`)
2. Definire architettura compilatore (`LEX_ARCHITECTURE.md`)
3. Implementare MVP transpiler
4. Testare con mod Imperium reale
5. Iterare basandosi su feedback

---

*"Il miglior linguaggio è quello che ti permette di dire quello che vuoi, non quello che il compilatore vuole."*
