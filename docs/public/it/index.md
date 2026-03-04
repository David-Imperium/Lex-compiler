# Documentazione Lex

**Versione:** 0.3.2
**Stato:** Sviluppo Attivo

---

## Cos'è Lex?

Lex è un **transpiler dichiarativo e multi-target** per contenuti di gioco. Scrivi una volta, compila in più backend.

```
game.lex → Lua, JSON, Godot, Unity, Contesto AI
```

---

## Link Rapidi

| Documento | Scopo |
|-----------|-------|
| [Specifica Linguaggio](./spec/language.md) | Sintassi, tipi, keyword |
| [Regole Validazione](./spec/validation.md) | Cosa valida lex_engine |
| [Backend Output](./spec/outputs.md) | Lua, JSON, Godot, Unity |
| [Architettura](./architecture/layers.md) | Come è strutturato Lex |
| [Tutorial](./guides/tutorial.md) | Come iniziare |

---

## Panoramica Architettura

```
┌─────────────────────────────────────────────┐
│                 Lex Neural                  │  Contesto AI, Lore
├─────────────────────────────────────────────┤
│                  Lex Core                   │  Validazione, riferimenti
├─────────────────────────────────────────────┤
│                  Lex Base                   │  Lexer, parser, AST
└─────────────────────────────────────────────┘
```

---

## Esempio in 30 Secondi

```lex
era Ancient {
    name: "Ancient Era"
}

structure Farm {
    era: Ancient
    name: "Farm"
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

Compila in:

```lua
Buildings.Farm = {
    era = "Ancient",
    cost = { Gold = 50 },
    production = { Food = 5 }
}
```

---

## Per Agenti AI

- Usa **solo keyword inglesi** (`structure`, non `struttura`)
- Leggi [spec/validation.md](./spec/validation.md) per i codici errore
- Leggi [integrations/imperium-engine.md](./integrations/imperium-engine.md) per integrazione Engine
- Tutte le definizioni sono dichiarative — descrivi il COSA, non il COME
