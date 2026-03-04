# Lex + Imperium Engine Integration

**Version:** 0.3.2  
**Status:** Active

---

## Overview

`lex_engine` is the **safety layer** between Lex and Imperium Engine.

```
┌─────────────────┐     ┌──────────────────┐     ┌─────────────────┐
│  EDITOR RUST    │     │   LEX ENGINE     │     │ IMPERIUM ENGINE │
│    (egui)       │────▶│    (DLL/FFI)     │────▶│     (C++)       │
│                 │◀────│                  │◀────│                 │
└─────────────────┘     └──────────────────┘     └─────────────────┘
                               │
                        Errori strutturati
                        per AI e debug
```

**Key Principle:** If lex_engine says OK, Engine CANNOT crash from that input.

---

## Location

```
C:\Imperium 2.0\lex_engine\
├── src/
│   ├── runtime/
│   │   ├── lex_runtime.cpp      # FFI interface
│   │   └── verbose_logger.cpp   # Structured errors
│   ├── bridge/
│   │   └── engine_bridge.cpp    # Engine calls
│   └── validation/
│       └── schema_validator.cpp # Cross-reference validation
├── CMakeLists.txt
└── README.md
```

**Note:** This is a PRIVATE module - not in public Lex repo.

---

## API

### Lifecycle

```c
bool lex_runtime_init(const char* engine_dll_path);
void lex_runtime_shutdown();
```

### Context

```c
void* lex_context_create();
void lex_context_destroy(void* ctx);
```

### Compilation

```c
bool lex_compile_file(void* ctx, const char* path);
bool lex_compile_string(void* ctx, const char* source);
bool lex_execute(void* ctx);
```

### Errors

```c
const char* lex_get_last_error_json(void* ctx);
bool lex_has_errors(void* ctx);
void lex_clear_errors(void* ctx);
```

### Verbose Mode

```c
void lex_set_verbose(void* ctx, bool enabled);
const char* lex_get_verbose_log(void* ctx);
```

### Engine Bridge

```c
uint64_t lex_engine_create_entity(const char* name);
void lex_engine_set_position(uint64_t id, float x, float y, float z);
bool lex_engine_save_scene(const char* path);
bool lex_engine_load_scene(const char* path);
```

---

## Error Format

```json
{
  "severity": "error",
  "code": "LEX_IMP_001",
  "category": "validation",
  "message": "Entity 'Warrior' requires 'faction' property",
  "file": "scene.lex",
  "line": 1,
  "column": 1,
  "suggestion": "Add: faction: \"Player\"",
  "ai_hint": "Required property for all entities"
}
```

---

## Rust Bindings

Location: `C:\Imperium 2.0\editor\src\lex_ffi.rs`

```rust
use lex_ffi::LexCompiler;

let mut compiler = LexCompiler::new();

// Compile from file
let result = compiler.compile_file("scene.lex");
if !result.success {
    for error in result.errors {
        println!("{}: {}", error.code, error.message);
    }
}

// Compile from string
let result = compiler.compile_string(r#"
    entity Warrior {
        pos: (10.0, 0.0, 5.0)
        faction: "Player"
    }
"#);
```

---

## Validation Flow

```
Input (.lex file or string)
         │
         ▼
    ┌─────────────────────────────┐
    │  1. Parse with Lex Compiler │
    │  2. Extract definitions     │
    │     (era, resource, etc.)   │
    └─────────────────────────────┘
         │
         ▼
    ┌─────────────────────────────┐
    │  3. Validate references     │
    │     - era exists?           │
    │     - resource defined?     │
    │     - tech exists?          │
    └─────────────────────────────┘
         │
         ▼
    ┌─────────────────────────────┐
    │  4. Type checking           │
    │     - pos is 3 floats       │
    │     - faction is valid enum │
    │     - health is int > 0     │
    └─────────────────────────────┘
         │
         ▼
    ┌─────────────────────────────┐
    │  5. Engine validation       │
    │     - asset exists?         │
    │     - values in range?      │
    └─────────────────────────────┘
         │
         ▼
    OK ──▶ Send to Engine
    ERROR ──▶ Return JSON error
```

---

## Entity Keywords

For scene/entity definitions:

```lex
entity <Name> {
    pos: (x, y, z)              // REQUIRED: position
    faction: "Player"           // REQUIRED: Player, Enemy, Neutral
    
    rot: (x, y, z, w)           // optional: quaternion
    scale: (x, y, z)            // optional: scale
    health: 100                 // optional: default 100
    model: "path/to/model.glb"  // optional: asset path
}

scene "Level1" {
    entities: [Warrior, Tower, Resource]
    environment: {
        skybox: "sky.hdr"
        fog: (0.1, 0.2, 0.3, 0.5)
    }
}
```

---

## Error Codes

| Code | Message |
|------|---------|
| `LEX_IMP_001` | Unknown era reference |
| `LEX_IMP_002` | Unknown resource reference |
| `LEX_IMP_003` | Unknown technology reference |
| `LEX_IMP_004` | Unknown structure reference |
| `LEX_IMP_005` | Unknown unit reference |
| `LEX_IMP_006` | File not found |
| `LEX_IMP_007` | Scene operation failed |
| `LEX_IMP_008` | Engine not initialized |
| `LEX_IMP_009` | Invalid enum value |
| `LEX_IMP_010` | No eras defined (warning) |
| `LEX_IMP_011` | No resources defined (warning) |
