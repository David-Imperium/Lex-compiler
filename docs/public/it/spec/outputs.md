# Backend di Output Lex

**Versione:** 0.3.2

---

## Panoramica

Lex compila i file `.lex` in molteplici formati di output:

| Backend | Output | Caso d'uso |
|---------|--------|------------|
| **Lua** | script `.lua` | Love2D, logica di gioco |
| **JSON** | dati `.json` | Scambio dati, strumenti |
| **Godot** | risorse `.gd` | Godot Engine |
| **Unity** | classi `.cs` | Unity Engine |
| **React** | tipi `.ts` | Web UI |
| **Lore** | contesto `.md` | AI companion |

---

## 1. Backend Lua

### Input
```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

### Output
```lua
-- Auto-generato da: farm.lex
Buildings.Farm = {
    id = "Farm",
    era = "Ancient",
    cost = { Gold = 50 },
    production = { Food = 5 }
}

function Buildings.Farm:onBuild(city)
    city.resources.Food = city.resources.Food + 5
end

function Buildings.Farm:onTurn(city)
    city.resources.Food = city.resources.Food + self.production.Food
end
```

---

## 2. Backend JSON

### Input
```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

### Output
```json
{
  "id": "Farm",
  "era": "Ancient",
  "cost": {
    "Gold": 50
  },
  "production": {
    "Food": 5
  }
}
```

---

## 3. Backend Godot

### Input
```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
    
    ui: {
        icon: "res://assets/farm.png"
    }
}
```

### Output
```gd
# farm.gd
extends Resource
class_name FarmData

@export var id: String = "Farm"
@export var era: String = "Ancient"
@export var cost: Dictionary = {"Gold": 50}
@export var production: Dictionary = {"Food": 5}
@export var icon: Texture2D

func _init():
    id = "Farm"
    era = "Ancient"
    cost = {"Gold": 50}
    production = {"Food": 5}
```

---

## 4. Backend Unity

### Input
```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

### Output
```csharp
// Farm.cs - Auto-generato da Lex
using System.Collections.Generic;

[CreateAssetMenu(fileName = "Farm", menuName = "Lex/Structures/Farm")]
public class Farm : StructureData
{
    public override string Id => "Farm";
    public override string Era => "Ancient";
    
    public Farm()
    {
        Cost = new Dictionary<string, int>
        {
            { "Gold", 50 }
        };
        
        Production = new Dictionary<string, int>
        {
            { "Food", 5 }
        };
    }
}
```

---

## 5. Backend React

### Input
```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

### Output
```typescript
// types/structures.ts
export interface Structure {
  id: string;
  era: string;
  cost: Record<string, number>;
  production: Record<string, number>;
}

export const Farm: Structure = {
  id: "Farm",
  era: "Ancient",
  cost: { Gold: 50 },
  production: { Food: 5 },
};
```

---

## 6. Backend Lore (Contesto AI)

### Input
```lex
structure SteamFactory {
    era: Steampunk
    name: "Steam Factory"
    description: "Converts coal into energy"
    
    lore: {
        quote: "Steam transforms iron into power"
        ai_context: "CRITICAL: Essential for Steampunk era"
    }
}
```

### Output
```markdown
# Steam Factory (Edificio)

## Info Base
- **Era:** Steampunk
- **Descrizione:** Converte carbone in energia

## Costi
- Coal: 8
- Steel: 5
- Gold: 50

## Produzione
- Energy: +15
- Industry: +10

## Contesto AI
**IMPORTANZA:** ALTA
**RACCOMANDAZIONE:** Costruisci all'inizio dell'era Steampunk
**PREREQUISITI:** Tecnologia SteamEngine

## Citazione
> "Steam transforms iron into power"
```

---

## 7. Utilizzo Backend

### CLI
```bash
# Compila in tutti i backend
lexc game.lex

# Compila in un backend specifico
lexc game.lex --target lua
lexc game.lex --target json
lexc game.lex --target godot
lexc game.lex --target unity

# Backend multipli
lexc game.lex --target lua,json,godot
```

### Libreria
```cpp
lex::CompileOptions options;
options.targets = { "lua", "json", "godot" };

lex::CompileResult result = lex::compile(source, options);

for (const auto& [target, output] : result.outputs) {
    std::cout << "=== " << target << " ===\n";
    std::cout << output << "\n";
}
```

---

## 8. Configurazione Backend

Ogni backend può avere configurazione personalizzata:

```lex
// Config backend nel file .lex
@config {
    lua: {
        namespace: "Game.Buildings",
        generate_callbacks: true
    }
    json: {
        pretty_print: true,
        include_metadata: true
    }
    godot: {
        resource_path: "res://data/structures/"
    }
}
```
