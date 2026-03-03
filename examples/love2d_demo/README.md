# Lex Love2D Demo

Un demo funzionante che mostra come usare i dati generati da Lex in Love2D.

## Come eseguire

```bash
cd examples/love2d_demo
love .
```

## Struttura

```
love2d_demo/
├── main.lua        # Logica gioco
├── conf.lua        # Configurazione Love2D
├── game_data.lua   # Dati generati da Lex (con helper functions)
└── README.md       # Questo file
```

## Come rigenerare i dati

Dalla root di Lex:

```bash
build\Debug\lexc.exe examples/imperium_minimal.lex -t lua -o examples/love2d_demo/ -n game_data
```

Poi edita il file per aggiungere `return { ... }` alla fine.

## Features demo

- **Visualizzazione strutture** con stato (disponibile/bloccata/costruita)
- **Sistema di costruzione** (premi 1-4 per costruire)
- **Controllo risorse** (costi, pagamenti)
- **Condizioni dinamiche** (`available_if` blocca strutture)
- **Produzione risorse** (le strutture generano risorse)

## Integrazione nel tuo gioco

1. Genera i dati: `lexc tuo_gioco.lex -t lua -o data/`
2. Aggiungi `return { ... }` alla fine del file
3. Nel tuo main.lua: `local data = require("data/game_data")`
4. Inizializza `game_state` prima di usare le condizioni
5. Chiama `entity.conditions.available_if(game_state)` per verificare

## Helper functions incluse

- `has_technology(tech_id)` — Controlla se una tech è sbloccata
- `has_structure(structure_id)` — Controlla se una struttura è costruita
- `has_resources(resource_map)` — Controlla se ci sono abbastanza risorse
