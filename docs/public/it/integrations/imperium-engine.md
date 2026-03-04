# Integrazione Imperium Engine

lex_engine fornisce il bridge FFI tra Lex Compiler e Imperium Engine.

## Architettura

```
┌──────────────┐     FFI      ┌─────────────────┐
│ Lex Compiler │ ──────────── │  lex_engine.dll │
│   (C++)      │              │   (C Bridge)    │
└──────────────┘              └────────┬────────┘
                                       │
                                       ▼
                              ┌─────────────────┐
                              │ Imperium Engine │
                              │   (C++ Core)    │
                              └─────────────────┘
```

## Funzioni API

### Compilazione

```cpp
// Crea istanza compilatore
void* lex_create();

// Compila da file
int lex_compile_file(void* ctx, const char* path);

// Compila da stringa
int lex_compile_string(void* ctx, const char* source);

// Ottieni output
const char* lex_get_output(void* ctx);

// Distruggi istanza
void lex_destroy(void* ctx);
```

### Gestione Errori

```cpp
// Ottieni ultimo errore (JSON)
const char* lex_get_error_json(void* ctx);

// Ottieni log verbose (JSON)
const char* lex_get_verbose_log(void* ctx);
```

### Gestione Schema

```cpp
// Imposta schema di gioco per validazione
int lex_set_schema(void* ctx, const char* schema_json);

// Valida senza compilare
int lex_validate(void* ctx, const char* source);
```

## Esempio di Utilizzo

```cpp
#include <lex_engine.h>

int main() {
    // Crea contesto
    auto* ctx = lex_create();
    
    // Carica schema
    lex_set_schema(ctx, R"({
        "eras": ["stone_age", "bronze_age"],
        "units": ["worker", "warrior"]
    })");
    
    // Compila
    const char* source = R"(
        fn main() {
            spawn("worker", 10, 20, 0);
        }
    )";
    
    if (lex_compile_string(ctx, source) == 0) {
        printf("Output: %s\n", lex_get_output(ctx));
    } else {
        printf("Errore: %s\n", lex_get_error_json(ctx));
    }
    
    lex_destroy(ctx);
    return 0;
}
```

## Binding Rust (Editor)

```rust
use lex_ffi::LexCompiler;

fn main() {
    let mut compiler = LexCompiler::new();
    compiler.set_schema(include_str!("game_schema.json"));
    
    match compiler.compile_string(source) {
        Ok(result) => println!("{}", result.output),
        Err(errors) => {
            for e in errors {
                eprintln!("Errore: {}", e.message);
            }
        }
    }
}
```

## Build

```bash
cd lex_engine
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

Output: `build/Release/lex_engine.dll`
