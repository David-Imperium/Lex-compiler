# Lex Compiler

[![CI](https://github.com/David-Imperium/Lex-compiler/actions/workflows/ci.yml/badge.svg)](https://github.com/David-Imperium/Lex-compiler/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-0.3.2-blue.svg)](https://github.com/David-Imperium/Lex-compiler)

Un transpiler dichiarativo e multi-target per contenuti di gioco.

**Principio:** *Descrivi il cosa, non il come.*

## Guida Rapida

```bash
# Clona e compila
git clone https://github.com/David-Imperium/Lex-compiler.git
cd lex
cmake -B build -S .
cmake --build build --config Release

# Provalo!
.\build\Release\lexc.exe examples\imperium_minimal.lex -t lua,json
```

## Utilizzo

```bash
# Utilizzo base (usa lo schema Imperium predefinito)
lexc input.lex -t lua,json

# Schema personalizzato per il tuo gioco
lexc input.lex --types item,character,location -t json

# Output multipli
lexc input.lex -o output/ -t lua,json
```

Opzioni:
- `-o, --output <dir>` — Directory di output
- `-t, --target <fmt>` — Formato/i di output: lua, json
- `--types <list>` — Tipi di definizione personalizzati (separati da virgola)
- `--no-validate` — Salta la validazione semantica
- `-h, --help` — Mostra l'aiuto

## Schemi Personalizzati

Lex è **agnostico** — definisci i tuoi tipi:

```lex
character Hero {
    name: "Arthur"
    level: 5
    class: "Warrior"
}

item Sword {
    name: "Excalibur"
    damage: 50
    rarity: "legendary"
}
```

Compila con:
```bash
lexc game.lex --types character,item -t json
```

Output:
```json
{
  "characters": {
    "Hero": {
      "id": "Hero",
      "type": "character",
      "name": "Arthur",
      "level": 5,
      "class": "Warrior"
    }
  },
  "items": {
    "Sword": {
      "id": "Sword",
      "type": "item",
      "name": "Excalibur",
      "damage": 50,
      "rarity": "legendary"
    }
  }
}
```

## Esempio

Schema predefinito (stile Imperium):

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
    
    available_if has_technology(SteamEngine) {
    }
}
```

## Caratteristiche

- **Sintassi dichiarativa** — Definizioni pulite e leggibili
- **Multi-target** — Backend Lua, JSON, Godot (GDScript), Unity (C#), e Love2D
- **Schemi personalizzati** — Definisci i tuoi tipi tramite `--types`
- **Type-safe** — Tipizzazione forte con inferenza
- **Validazione semantica** — Rileva errori prima del runtime
- **Parser di espressioni** — Supporto completo per precedenza operatori
- **Parole chiave inglesi** — Accessibile alla comunità internazionale

## Roadmap

| Versione | Stato | Funzionalità |
|----------|-------|--------------|
| v0.3.1 | ✅ Corrente | Core agnostico, flag `--types` |
| v0.4.0 | 🔜 Prossima | Release binarie, tutorial, estensione VS Code |
| v0.5.0 | 📋 Pianificata | Backend TypeScript |

Vedi [ROADMAP.md](docs/ROADMAP.md) per dettagli.

## Testing

```bash
# Unit test
.\build\tests\Debug\lexer_tests.exe

# Validazione output Lua
python tests/lua_validator.py output/game.lua
```

## Licenza

MIT

## Autore

David (Imperium Game Engine)

---

## 📚 Documentazione

- **[English README](README.md)**
- **[Tutorial in Italiano](docs/TUTORIAL_IT.md)**
