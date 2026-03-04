# Getting Started Tutorial

## Installation

### From Source

```bash
git clone https://github.com/yourorg/lex.git
cd lex
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

The compiler binary will be at `build/Release/lexc.exe`.

## Your First Program

Create `hello.lex`:

```lex
fn main() {
    let message: string = "Hello, Lex!";
    print(message);
}
```

Compile it:

```bash
lexc hello.lex -o hello.lua
```

Output (`hello.lua`):

```lua
function main()
    local message = "Hello, Lex!"
    print(message)
end
```

## Variables and Types

```lex
fn example() {
    // Numbers
    let age: int = 25;
    let price: float = 19.99;
    
    // Boolean
    let active: bool = true;
    
    // String
    let name: string = "Player1";
}
```

## Functions

```lex
fn greet(name: string): string {
    return "Hello, " + name;
}

fn add(a: int, b: int): int {
    return a + b;
}

fn main() {
    let greeting: string = greet("World");
    let sum: int = add(10, 20);
}
```

## Control Flow

```lex
fn checkAge(age: int) {
    if age >= 18 {
        print("Adult");
    } else {
        print("Minor");
    }
}

fn countDown() {
    let i: int = 10;
    while i > 0 {
        print(i);
        i = i - 1;
    }
}
```

## Game Integration

```lex
// Spawn units
fn spawnWorkers() {
    spawn("worker", 0, 0, 0);
    spawn("worker", 10, 0, 0);
}

// Event handling
on game_start {
    spawnWorkers();
}

on unit_died {
    emit("check_victory");
}
```

## Next Steps

- Read the [Language Reference](../spec/language.md)
- Learn about [Validation](../spec/validation.md)
- Explore [Imperium Integration](../integrations/imperium-engine.md)
