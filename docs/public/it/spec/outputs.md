# Backend di Output

Lex supporta diversi target di compilazione.

## Backend Supportati

| Backend | Stato | Caso d'Uso |
|---------|-------|------------|
| Lua | ✅ Stabile | Scripting gioco |
| JSON | ✅ Stabile | Scambio dati |
| Godot GDScript | 🚧 In Corso | Integrazione Godot |
| C++ | 📋 Pianificato | Compilazione nativa |

## Backend Lua

Output predefinito per scripting:

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

## Backend JSON

Per configurazioni data-driven:

```lex
// Input
config ImpostazioniGioco {
    difficolta: string = "normale";
    maxUnita: int = 100;
}
```

```json
{
  "type": "config",
  "name": "ImpostazioniGioco",
  "fields": {
    "difficolta": "normale",
    "maxUnita": 100
  }
}
```

## Selezione Backend

Da riga di comando:
```bash
lexc input.lex --backend lua -o output.lua
lexc input.lex --backend json -o output.json
```

Programmaticamente:
```cpp
lex::Compiler compiler;
compiler.setBackend(lex::Backend::Lua);
compiler.compile("input.lex");
```
