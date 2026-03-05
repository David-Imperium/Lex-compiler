# Lex Output Backends

**Version:** 0.4.0

---

## Overview

Lex compiles `.lex` files to multiple output formats:

| Backend | Output | Use Case |
|---------|--------|----------|
| **Lua** | `.lua` scripts | Game logic, embedded scripts |
| **JSON** | `.json` data | Data exchange, tools |
| **Godot** | `.gd` resources | Godot Engine 4.x |
| **Unity** | `.cs` classes | Unity ScriptableObject |
| **Love2D** | `.lua` modules | LÖVE2D framework |
| **Defold** | `.go` + `.lua` | Defold game objects |

---

## 1. Lua Backend

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
-- Auto-generated from: farm.lex
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

## 2. JSON Backend

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

## 3. Godot Backend

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

## 4. Unity Backend

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
// Farm.cs - Auto-generated from Lex
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

## 5. Love2D Backend

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

-- Hot reload support
function Farm:reload()
    local newData = love.files.load("data/structures/farm.lua")
    for k, v in pairs(newData) do
        self[k] = v
    end
end

return Farm
```

### Features
- Hot reload support for development
- Module-based organization
- LÖVE2D event integration

---

## 6. Defold Backend

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

### Features
- Native Defold game objects
- Collection factory integration
- Property-based configuration

---

## 7. AI Context Generator

**New in v0.4.0**

The AI Context Generator creates structured context for AI agents from `.lex` files.

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

### Output Formats

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

**Minimal** (compact for LLM context):
```
SteamFactory:Steampunk|Coal:8,Steel:5,Gold:50|Energy:15,Industry:10|Essential for energy
```

### Query System

Query the generated context with natural language:

```bash
# Find structures that produce energy
lexc game.lex --query "structures producing energy"

# Find all Steampunk era content
lexc game.lex --query "era:Steampunk"

# Find high-importance items for AI
lexc game.lex --query "ai_importance:high"
```

---

## 8. Using Backends

### CLI
```bash
# Compile to all backends
lexc game.lex

# Compile to specific backend
lexc game.lex --target lua
lexc game.lex --target json
lexc game.lex --target godot
lexc game.lex --target unity
lexc game.lex --target love2d
lexc game.lex --target defold

# Multiple backends
lexc game.lex --target lua,json,godot

# Generate AI context
lexc game.lex --ai-context markdown
lexc game.lex --ai-context json
lexc game.lex --ai-context minimal

# Query AI context
lexc game.lex --query "structures producing energy"
```

### Library
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

## 9. Backend Configuration

Each backend can have custom configuration:

```lex
// Backend config in .lex file
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
