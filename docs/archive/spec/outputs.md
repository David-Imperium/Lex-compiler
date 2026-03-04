# Lex Output Backends

**Version:** 0.3.2

---

## Overview

Lex compiles `.lex` files to multiple output formats:

| Backend | Output | Use Case |
|---------|--------|----------|
| **Lua** | `.lua` scripts | Love2D, game logic |
| **JSON** | `.json` data | Data exchange, tools |
| **Godot** | `.gd` resources | Godot Engine |
| **Unity** | `.cs` classes | Unity Engine |
| **React** | `.ts` types | Web UI |
| **Lore** | `.md` context | AI companion |

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

## 5. React Backend

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

## 6. Lore Backend (AI Context)

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
# Steam Factory (Building)

## Basic Info
- **Era:** Steampunk
- **Description:** Converts coal into energy

## Costs
- Coal: 8
- Steel: 5
- Gold: 50

## Production
- Energy: +15
- Industry: +10

## AI Context
**IMPORTANCE:** HIGH
**RECOMMENDATION:** Build early in Steampunk era
**PREREQUISITES:** SteamEngine technology

## Quote
> "Steam transforms iron into power"
```

---

## 7. Using Backends

### CLI
```bash
# Compile to all backends
lexc game.lex

# Compile to specific backend
lexc game.lex --target lua
lexc game.lex --target json
lexc game.lex --target godot
lexc game.lex --target unity

# Multiple backends
lexc game.lex --target lua,json,godot
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

## 8. Backend Configuration

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
