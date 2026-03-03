# Tutorial Lex: Il Tuo Primo Mod

Questo tutorial ti guida nella creazione di una mod semplice per un gioco usando Lex.

## Prerequisiti

- Compilatore Lex installato (o compilato da sorgente)
- Un editor di testo
- Conoscenza base dei contenuti di gioco (oggetti, personaggi, ecc.)

## Passo 1: Installare Lex

### Opzione A: Scaricare il Binario (Consigliato)

Scarica l'ultima release per la tua piattaforma:
- Windows: `lexc-windows-x64.zip`
- Linux: `lexc-linux-x64.tar.gz`
- macOS: `lexc-macos-x64.tar.gz`

Estrai e aggiungi al tuo PATH.

### Opzione B: Compilare da Sorgente

```bash
git clone https://github.com/David-Imperium/Lex-compiler.git
cd Lex-compiler
cmake -B build -S .
cmake --build build --config Release
```

L'eseguibile si trova in `build/Release/lexc` (o `build\Release\lexc.exe` su Windows).

## Passo 2: Creare la Tua Prima Definizione

Crea un file chiamato `mymod.lex`:

```lex
item HealthPotion {
    name: "Health Potion"
    description: "Restores 50 health points"
    rarity: "common"
    value: 25
}

item MagicSword {
    name: "Excalibur"
    description: "A legendary sword of immense power"
    rarity: "legendary"
    damage: 150
    value: 1000
}

character Goblin {
    name: "Goblin Warrior"
    level: 5
    health: 100
    damage: 15
}

character Dragon {
    name: "Ancient Dragon"
    level: 50
    health: 5000
    damage: 200
}
```

## Passo 3: Compilare

```bash
lexc mymod.lex --types item,character -t lua,json
```

Questo indica a Lex:
- `--types item,character` — Stiamo usando i tipi personalizzati "item" e "character"
- `-t lua,json` — Genera sia output Lua che JSON

## Passo 4: Verificare l'Output

### Output JSON (`mymod.json`)

```json
{
  "version": "1.0",
  "generated_by": "Lex Compiler v0.3.1",
  "items": {
    "HealthPotion": {
      "id": "HealthPotion",
      "type": "item",
      "name": "Health Potion",
      "description": "Restores 50 health points",
      "rarity": "common",
      "value": 25
    },
    "MagicSword": {
      "id": "MagicSword",
      "type": "item",
      "name": "Excalibur",
      "description": "A legendary sword of immense power",
      "rarity": "legendary",
      "damage": 150,
      "value": 1000
    }
  },
  "characters": {
    "Goblin": {
      "id": "Goblin",
      "type": "character",
      "name": "Goblin Warrior",
      "level": 5,
      "health": 100,
      "damage": 15
    },
    "Dragon": {
      "id": "Dragon",
      "type": "character",
      "name": "Ancient Dragon",
      "level": 50,
      "health": 5000,
      "damage": 200
    }
  }
}
```

## Passo 5: Usare le Condizioni

Lex supporta blocchi condizionali:

```lex
item FrostSword {
    name: "Frost Brand"
    damage: 75
    
    available_if player_level > 10 {
        description: "A sword that freezes enemies"
    }
    
    bonus_if has_skill(IceMagic) {
        damage_bonus: 25
    }
}
```

## Passo 6: Mappe di Risorse

Per costi, produzione, ecc.:

```lex
structure Blacksmith {
    name: "Village Blacksmith"
    
    cost: {
        Gold: 500,
        Wood: 100,
        Stone: 50
    }
    
    production: {
        Gold: 10
    }
}
```

## Integrazione con il Tuo Gioco

### Integrazione Lua

```lua
-- Carica il file generato
local items = dofile("mymod.lua")

-- Accedi agli oggetti
print(items.HealthPotion.name)  -- "Health Potion"
print(items.MagicSword.damage)  -- 150
```

### Integrazione JSON (esempio Python)

```python
import json

with open("mymod.json") as f:
    data = json.load(f)

print(data["items"]["HealthPotion"]["name"])  # "Health Potion"
```

### Integrazione C++ (usando la libreria)

```cpp
#include <lex/lex.hpp>

// Leggi il tuo file .lex
std::ifstream file("mymod.lex");
std::string source((std::istreambuf_iterator<char>(file)), {});

// Compila
lex::CompileOptions options;
options.types = {"item", "character"};

lex::CompileResult result = lex::compile(source, options);

if (result.success) {
    // Ottieni output JSON
    std::string json = result.outputs["json"];
    
    // Analizza e usa nel tuo gioco...
}
```

## Consigli

1. **Inizia semplice** — Un tipo, poche definizioni
2. **Usa `--verbose`** — Vedi cosa sta facendo il compilatore
3. **Controlla gli errori attentamente** — Lex ti indica i numeri di riga
4. **Organizza per tipo** — Mantieni oggetti, personaggi, ecc. in file separati se necessario

## Prossimi Passi

- Leggi la [Specifica del Linguaggio](LEX_SPECIFICATION.md)
- Vedi altri [Esempi](../examples/)
- Controlla la [Roadmap](ROADMAP.md) per le funzionalità in arrivo

## Ottenere Aiuto

- Apri una issue su GitHub
- Controlla gli esempi esistenti nella directory `examples/`

---

## 📚 Documentazione

- **[English Tutorial](TUTORIAL.md)**
- **[README in Italiano](../README_IT.md)**
