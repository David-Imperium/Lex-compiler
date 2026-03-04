# Output Backends

Lex supports multiple compilation targets.

## Supported Backends

| Backend | Status | Use Case |
|---------|--------|----------|
| Lua | ✅ Stable | Game scripting |
| JSON | ✅ Stable | Data exchange |
| Godot GDScript | 🚧 WIP | Godot integration |
| C++ | 📋 Planned | Native compilation |

## Lua Backend

Default output for game scripting:

```lex
// Input
fn main() {
    let x: int = 42;
    spawn("worker", x, 0, 0);
}
```

```lua
-- Output
function main()
    local x = 42
    spawn("worker", x, 0, 0)
end
```

## JSON Backend

For data-driven configurations:

```lex
// Input
config GameSettings {
    difficulty: string = "normal";
    maxUnits: int = 100;
}
```

```json
{
  "type": "config",
  "name": "GameSettings",
  "fields": {
    "difficulty": "normal",
    "maxUnits": 100
  }
}
```

## Backend Selection

Command line:
```bash
lexc input.lex --backend lua -o output.lua
lexc input.lex --backend json -o output.json
```

Programmatic:
```cpp
lex::Compiler compiler;
compiler.setBackend(lex::Backend::Lua);
compiler.compile("input.lex");
```
