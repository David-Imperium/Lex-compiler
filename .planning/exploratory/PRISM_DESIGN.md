# PRISM — Language Design Document

**Status:** Draft / Exploration
**Created:** 2026-03-04
**Base:** Fork of Odin (https://odin-lang.org)

---

## 1. Vision

**Prism** is a **general-purpose systems programming language**. 

Game dev is the primary focus, but Prism works for anything you'd use C/C++ for:
- CLI tools
- Web servers / backends
- Embedded systems
- System utilities
- Desktop applications

**Prism** is a systems programming language for developers who want:
- C++ power without C++ complexity
- Memory safety without garbage collection
- Fast builds and fast executables
- Seamless interop with existing C/C++ codebases

```
Prism: C++ power + Odin simplicity + Rust safety (optional)
```

---

## 2. Why Fork Odin?

Odin is already excellent for game dev:
- Simple, readable syntax
- No hidden control flow
- Built-in arrays, slices, dynamic arrays
- Context system for allocation/logging
- Cross-platform, fast compiler

**What Prism adds:**
- Stronger type system (optionally)
- Better metaprogramming (comptime)
- Improved error messages
- Lex integration (optional)
- More ergonomic syntax in places

---

## 3. Design Goals

### 3.1 Simplicity First

```prism
// No header files
// No templates (use comptime instead)
// No OOP bloat (composition over inheritance)

// Clean entry point
main :: proc() {
    print("Hello, Prism!")
}
```

### 3.2 Memory Safety Without GC

```prism
// Owner/borrow semantics (inspired by Rust, but optional)
create_player :: proc() -> Owner<Player> {
    return Player{ name = "Hero", hp = 100 }
}

take_damage :: proc(p: Borrow<Player>, damage: int) {
    p.hp -= damage  // Borrow can read/write
}

// Or just manual management like Odin
manual_player :: proc() {
    p: Player
    defer free(p)
    // use p
}
```

### 3.3 Fast Builds

- Incremental compilation
- Single-pass compilation where possible
- Fast linker integration (mold/lld)
- Caching of compiled modules

### 3.4 C/C++ Interop

```prism
// Direct include of C headers
#import "raylib.h" @c

// Use C functions natively
InitWindow(800, 600, "Prism Game")

// Export functions to C
export game_update :: proc(dt: f32) @c {
    // callable from C/C++
}
```

---

## 4. Syntax Examples

### 4.1 Basic Types

```prism
// Primitives
x: int = 42
pi: f64 = 3.14159
name: string = "Prism"
flag: bool = true

// Arrays
scores: [10]int
items: []string = ["sword", "shield", "potion"]

// Structs
Player :: struct {
    name: string
    hp: int
    max_hp: int
    position: Vec3
}

Vec3 :: struct {
    x, y, z: f32
}
```

### 4.2 Functions

```prism
// Basic function
add :: proc(a: int, b: int) -> int {
    return a + b
}

// Multiple returns
divide :: proc(a: int, b: int) -> (quotient: int, remainder: int) {
    return a / b, a % b
}

// Named parameters (for readability)
draw_rect :: proc(
    x: int, y: int,
    width: int, height: int,
    color: Color = Color.Black
) {
    // ...
}

// Call with named args
draw_rect(x=10, y=20, width=100, height=50)
```

### 4.3 Control Flow

```prism
// If expressions (not statements)
result := if score >= 90 { "A" }
          elif score >= 80 { "B" }
          else { "F" }

// For loops
for i in 0..10 {
    print(i)
}

for item in items {
    print(item)
}

// Switch (no fallthrough)
switch day {
case .Monday, .Tuesday:
    print("Weekday")
case .Saturday, .Sunday:
    print("Weekend")
default:
    print("Unknown")
}
```

### 4.4 Enums and Unions

```prism
// Simple enum
Direction :: enum {
    North
    East
    South
    West
}

// Tagged union (sum type)
Result :: union {
    Ok: int
    Error: string
}

handle_result :: proc(r: Result) {
    switch v in r {
    case .Ok:
        print("Got: {}", v)
    case .Error:
        print("Error: {}", v)
    }
}
```

### 4.5 Error Handling

```prism
// Multiple return with error
load_file :: proc(path: string) -> (data: []byte, err: Error) {
    // ...
}

// Usage
content, err := load_file("game.json")
if err != nil {
    print("Failed: {}", err)
    return
}
```

---

## 5. Unique Features

### 5.1 Context System (from Odin)

```prism
// Allocator context
push_allocator(temp_allocator)
defer pop_allocator()

data := make([1024]byte)  // Uses temp allocator

// Logger context
push_logger(game_logger)
log_info("Player spawned")
```

### 5.2 Comptime (compile-time execution)

```prism
// Compile-time code generation
vector :: proc($T: type, $N: int) -> type {
    return struct {
        data: [N]T
    }
}

Vec3f :: vector(f32, 3)
Vec4i :: vector(int, 4)

// Compile-time conditions
when DEBUG {
    print("Debug mode enabled")
}
```

### 5.3 Lex Integration (optional)

```prism
// Load .lex files at compile time
#load "game.lex"

// Generated types available
player: Player = Player{
    name = "Hero",
    hp = player_default_hp  // from .lex
}
```

---

## 6. Module System

```prism
// math.prism
module math

Pi :: f64 = 3.14159

Vec2 :: struct {
    x, y: f32
}

// main.prism
import "math"

main :: proc() {
    v: math.Vec2 = {1.0, 2.0}
    print("Pi = {}", math.Pi)
}
```

---

## 7. Build System

```prism
// prism.toml
[project]
name = "my_game"
version = "0.1.0"

[dependencies]
raylib = "5.0"

[build]
optimize = "release"
target = "native"
```

```bash
# Build commands
prism build          # Build project
prism run            # Build and run
prism test           # Run tests
prism release        # Optimized build
```

---

## 8. Comparison

| Feature | C++ | Rust | Odin | **Prism** |
|---------|-----|------|------|-----------|
| Simplicity | ❌ | ❌ | ✅ | ✅ |
| Memory Safety | ⚠️ | ✅ | ⚠️ | ✅ (optional) |
| GC | ❌ | ❌ | ❌ | ❌ |
| Fast Builds | ❌ | ⚠️ | ✅ | ✅ |
| C Interop | ✅ | ⚠️ | ✅ | ✅ |
| Metaprogramming | ⚠️ | ❌ | ⚠️ | ✅ |
| Game Dev Focus | ⚠️ | ❌ | ✅ | ✅ |

---

## 9. Implementation Roadmap

### Phase 1: Bootstrap
- [ ] Fork Odin compiler
- [ ] Rename to Prism
- [ ] Set up build system
- [ ] Basic test suite

### Phase 2: Language Changes
- [ ] Syntax tweaks (if any)
- [ ] Add comptime
- [ ] Improve error messages
- [ ] Add optional borrow checker

### Phase 3: Tooling
- [ ] VS Code extension
- [ ] Package manager
- [ ] LSP server
- [ ] Debugger integration

### Phase 4: Ecosystem
- [ ] Standard library
- [ ] Game dev library bindings
- [ ] Documentation
- [ ] Example projects

---

## 10. Design Decisions

| Decision | Choice | Rationale |
|----------|--------|-----------|
| **Borrow checker** | Optional | Attiva quando vuoi sicurezza, disattiva per semplicità. Meglio entrambi i mondi. |
| **Generics** | Comptime (Zig-style) | Più semplice dei template C++, più potente dei generics Go. Codice che gira a compile-time. |
| **Error handling** | Multiple return | `result, err := do_thing()` è più leggibile di `Result<T, E>` annidati. Pratico vince. |
| **Package manager** | Built-in | Come Cargo/Rust. Un comando per tutto. |
| **Backends** | LLVM | Industry standard, ottime ottimizzazioni, supporto multipiattaforma. |

---

## 12. Future: Self-Hosting & Lex

**Long-term vision:** Prism compila Prism, Prism compila Lex.

```
Current:  C++ compiles Lex, C++ compiles Prism
Future:   Prism compiles Prism, Prism compiles Lex
```

**Benefits:**
- Lex written in simpler language than C++
- Unified toolchain (Prism compiles everything)
- Better error messages for Lex users
- Single language for the entire ecosystem

**Roadmap:**
1. Prism v0.1 (written in C++, can compile Prism)
2. Prism bootstrap (Prism compiles itself)
3. Lex rewritten in Prism
4. Profit

**Status:** Idea for future. Lex stays C++ for now.

---

## 13. Philosophy

> "Prism is for programmers who want to write games, not fight the language."

**Principles:**
1. **Simplicity wins** — Se serve un PhD per capirlo, è sbagliato
2. **Practical over pure** — Meglio funzionare che essere eleganti
3. **Fast iteration** — Build veloci, errori chiari
4. **C interoperability** — Non reinventare, riutilizza
5. **Game dev first** — Ottimizzato per quello che serve ai giochi

---

## 11. References

- Odin Language: https://odin-lang.org
- Val Language: https://val-lang.dev
- Hylo Language: https://github.com/hylo-lang/hylo
- Zig Language: https://ziglang.org
- Rust Language: https://rust-lang.org

---

*This document is a reconstruction of the original PRISM design notes.*
