# Imperium Engine Integration

lex_engine provides the FFI bridge between Lex Compiler and Imperium Engine.

## Architecture

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

## API Functions

### Compilation

```cpp
// Create compiler instance
void* lex_create();

// Compile from file
int lex_compile_file(void* ctx, const char* path);

// Compile from string
int lex_compile_string(void* ctx, const char* source);

// Get output
const char* lex_get_output(void* ctx);

// Destroy instance
void lex_destroy(void* ctx);
```

### Error Handling

```cpp
// Get last error (JSON)
const char* lex_get_error_json(void* ctx);

// Get verbose log (JSON)
const char* lex_get_verbose_log(void* ctx);
```

### Schema Management

```cpp
// Set game schema for validation
int lex_set_schema(void* ctx, const char* schema_json);

// Validate without compilation
int lex_validate(void* ctx, const char* source);
```

## Usage Example

```cpp
#include <lex_engine.h>

int main() {
    // Create context
    auto* ctx = lex_create();
    
    // Load schema
    lex_set_schema(ctx, R"({
        "eras": ["stone_age", "bronze_age"],
        "units": ["worker", "warrior"]
    })");
    
    // Compile
    const char* source = R"(
        fn main() {
            spawn("worker", 10, 20, 0);
        }
    )";
    
    if (lex_compile_string(ctx, source) == 0) {
        printf("Output: %s\n", lex_get_output(ctx));
    } else {
        printf("Error: %s\n", lex_get_error_json(ctx));
    }
    
    lex_destroy(ctx);
    return 0;
}
```

## Rust Bindings (Editor)

```rust
use lex_ffi::LexCompiler;

fn main() {
    let mut compiler = LexCompiler::new();
    compiler.set_schema(include_str!("game_schema.json"));
    
    match compiler.compile_string(source) {
        Ok(result) => println!("{}", result.output),
        Err(errors) => {
            for e in errors {
                eprintln!("Error: {}", e.message);
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
