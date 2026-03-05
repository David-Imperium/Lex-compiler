# Backend di Output Lex

**Versione:** 0.4.0

---

## Panoramica

Lex compila i file `.lex` in molteplici formati di output:

| Backend | Output | Caso d'uso |
|---------|--------|------------|
| **Lua** | script `.lua` | Logica di gioco, script embedded |
| **JSON** | dati `.json` | Scambio dati, strumenti |
| **Godot** | risorse `.gd` | Godot Engine 4.x |
| **Unity** | classi `.cs` | Unity ScriptableObject |
| **Love2D** | moduli `.lua` | Framework LÖVE2D |
| **Defold** | `.go` + `.lua` | Game object Defold |

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
        Cost = new Dictionary&lt;string, int&gt;
        {
            { "Gold", 50 }
        };

        Production = new Dictionary&lt;string, int&gt;
        {
            { "Food", 5 }
        };
    }
}
```

---

## 5. Backend Love2D

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
-- data/structures/farm.lua
local Farm = {
    id = "Farm",
    era = "Ancient",
    cost = { Gold = 50 },
    production = { Food = 5 }
}

-- Supporto hot reload
function Farm:reload()
    local newData = love.files.load("data/structures/farm.lua")
    for k, v in pairs(newData) do
        self[k] = v
    end
end

return Farm
```

### Funzionalità
- Supporto hot reload per sviluppo
- Organizzazione a moduli
- Integrazione eventi LÖVE2D

---

## 6. Backend Defold

### Input
```lex
structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

### Output

**farm.go** (Game Object):
```lua
-- game.objects/farm.go
go.property("id", "Farm")
go.property("era", "Ancient")
go.property("cost", msg.url("#cost_data"))
go.property("production", msg.url("#production_data"))
```

**farm.lua** (Script):
```lua
-- scripts/structures/farm.lua
local M = {}

M.data = {
    id = "Farm",
    era = "Ancient",
    cost = { Gold = 50 },
    production = { Food = 5 }
}

function M.init(self)
    self.cost = M.data.cost
    self.production = M.data.production
end

return M
```

### Funzionalità
- Game object nativi Defold
- Integrazione collection factory
- Configurazione tramite proprietà

---

## 7. AI Context Generator

**Nuovo in v0.4.0**

L'AI Context Generator crea contesto strutturato per agenti AI dai file `.lex`.

### Input
```lex
structure SteamFactory {
    era: Steampunk
    name: "Steam Factory"
    description: "Converts coal into energy"

    cost: { Coal: 8, Steel: 5, Gold: 50 }
    production: { Energy: 15, Industry: 10 }

    lore: {
        quote: "Steam transforms iron into power"
        ai_hint: "Essential for Steampunk era energy production"
    }
}
```

### Formati Output

**Markdown**:
```markdown
# Steam Factory (Building)

## Basic Info
- **Era:** Steampunk
- **Description:** Converts coal into energy

## Resources
| Input | Output |
|-------|--------|
| Coal: 8, Steel: 5, Gold: 50 | Energy: 15, Industry: 10 |

## AI Hint
> Essential for Steampunk era energy production

## Quote
> "Steam transforms iron into power"
```

**JSON**:
```json
{
  "id": "SteamFactory",
  "type": "structure",
  "era": "Steampunk",
  "description": "Converts coal into energy",
  "cost": {"Coal": 8, "Steel": 5, "Gold": 50},
  "production": {"Energy": 15, "Industry": 10},
  "ai_context": {
    "importance": "high",
    "keywords": ["energy", "steampunk", "production"]
  }
}
```

**Minimal** (compatto per context window LLM):
```
SteamFactory:Steampunk|Coal:8,Steel:5,Gold:50|Energy:15,Industry:10|Essential for energy
```

### Query System

Interroga il contesto generato con linguaggio naturale:

```bash
# Trova strutture che producono energia
lexc game.lex --query "structures producing energy"

# Trova tutto il contenuto era Steampunk
lexc game.lex --query "era:Steampunk"

# Trova elementi ad alta importanza AI
lexc game.lex --query "ai_importance:high"
```

---

## 8. Utilizzo Backend

### CLI
```bash
# Compila in tutti i backend
lexc game.lex

# Compila in un backend specifico
lexc game.lex --target lua
lexc game.lex --target json
lexc game.lex --target godot
lexc game.lex --target unity
lexc game.lex --target love2d
lexc game.lex --target defold

# Backend multipli
lexc game.lex --target lua,json,godot

# Genera contesto AI
lexc game.lex --ai-context markdown
lexc game.lex --ai-context json
lexc game.lex --ai-context minimal

# Query contesto AI
lexc game.lex --query "structures producing energy"
```

### Libreria
```cpp
lex::CompileOptions options;
options.targets = { "lua", "json", "godot" };

lex::CompileResult result = lex::compile(source, options);

for (const auto& [target, output] : result.outputs) {
    std::cout &lt;&lt; "=== " &lt;&lt; target &lt;&lt; " ===\n";
    std::cout &lt;&lt; output &lt;&lt; "\n";
}
```

---

## 9. Configurazione Backend

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
