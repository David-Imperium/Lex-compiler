# Lex Tutorial

**5 minutes to learn Lex.**

---

## 1. Your First Definition

Create a file `game.lex`:

```lex
era Ancient {
    name: "Ancient Era"
}

structure Farm {
    era: Ancient
    cost: { Gold: 50 }
    production: { Food: 5 }
}
```

Compile:
```bash
lexc game.lex --target lua
```

Output:
```lua
Buildings.Farm = {
    era = "Ancient",
    cost = { Gold = 50 },
    production = { Food = 5 }
}
```

---

## 2. Definitions

### Era (Required first)

```lex
era <Name> {
    name: "Display Name"
}
```

### Structure

```lex
structure <Name> {
    era: <EraName>              // REQUIRED
    cost: { Gold: 50 }
    production: { Food: 5 }
    maintenance: { Gold: 2 }
}
```

### Unit

```lex
unit <Name> {
    era: <EraName>              // REQUIRED
    type: military              // REQUIRED: military, civilian, hero
    
    stats: {
        attack: 25
        defense: 20
        movement: 3
    }
    
    cost: { Gold: 100 }
}
```

### Technology

```lex
technology <Name> {
    research_cost: 100          // REQUIRED
    
    prerequisites: [OtherTech]
    unlocks: {
        structures: [BuildingA]
        units: [UnitA]
    }
}
```

### Resource

```lex
resource Gold {
    name: "Gold"
    category: basic             // basic, luxury, strategic
}
```

---

## 3. Properties

| Property | Type | Example |
|----------|------|---------|
| `name` | string | `"Farm"` |
| `era` | reference | `Ancient` |
| `cost` | ResourceMap | `{ Gold: 50, Wood: 20 }` |
| `production` | ResourceMap | `{ Food: 5 }` |
| `prerequisites` | list | `[TechA, TechB]` |
| `stats` | block | `{ attack: 25 }` |

---

## 4. Conditions

```lex
structure SteamFactory {
    era: Steampunk
    
    available_if: {
        condition: civilization.industry >= 100
        message: "Need more industry"
    }
}
```

### Operators

- Comparison: `==` `!=` `>` `<` `>=` `<=`
- Logical: `and` `or` `not`

```lex
condition: population > 100 and happiness < 40
condition: era == Steampunk or era == Cyberpunk
```

---

## 5. Full Example

```lex
// Resources
resource Gold { name: "Gold", category: basic }
resource Food { name: "Food", category: basic }

// Era
era Ancient {
    name: "Ancient Era"
    dominant_color: #8B4513
}

// Technology
technology Agriculture {
    research_cost: 50
    
    unlocks: {
        structures: [Farm, Granary]
    }
}

// Structure
structure Farm {
    era: Ancient
    name: "Farm"
    
    cost: { Gold: 50 }
    production: { Food: 5 }
    
    requires: {
        technologies: [Agriculture]
    }
}

// Unit
unit Warrior {
    era: Ancient
    type: military
    
    stats: {
        attack: 15
        defense: 10
        movement: 2
    }
    
    cost: { Gold: 80 }
}
```

---

## 6. Compile

```bash
# All backends
lexc game.lex

# Specific backend
lexc game.lex --target lua
lexc game.lex --target json
lexc game.lex --target godot

# Watch mode (recompile on change)
lexc game.lex --watch
```

---

## 7. Common Errors

| Error | Fix |
|-------|-----|
| `SEM003: Missing required property 'era'` | Add `era: <EraName>` |
| `LEX_IMP_001: Unknown era reference` | Define the era first |
| `SEM001: Undefined reference` | Check spelling of referenced item |

---

## 8. Tips

1. **Define eras first**, then resources, then everything else
2. **Use English keywords** (for AI agents and international use)
3. **Run validation early** - `lexc game.lex --validate`
4. **Check docs/spec/validation.md** for all rules
