# RFC-LEX-IMPERIUM-001: Lex Core + Imperium Engine Integration

**Status:** Draft
**Created:** 2026-03-03
**Author:** David + Droid
**Layer:** Lex Core

---

## 1. Summary

Integrate Lex Compiler with Imperium Engine C++ to create a **runtime DSL controller** that:
1. Validates input before reaching the engine (no more obscure crashes)
2. Provides verbose, structured errors for AI agents
3. Acts as middleware between Editor Rust and Engine C++
4. Enables hot-reload of game logic via `.lex` files

---

## 2. Problem Statement

### 2.1 Current Issues

1. **Rust-C++ Bridge Problems**: Editor Rust cannot properly initialize 3D renderer when calling Engine directly
2. **Obscure Errors**: Engine crashes with cryptic C++ errors, hard to debug
3. **No Validation Layer**: Invalid data reaches engine, causing runtime failures
4. **AI Agents Struggle**: Agents write C++ poorly, need simpler interface

### 2.2 Proposed Solution

```
┌─────────────────┐     ┌──────────────────┐     ┌─────────────────┐
│  EDITOR RUST    │     │   LEX RUNTIME    │     │ IMPERIUM ENGINE │
│    (egui)       │────▶│    (DLL/FFI)     │────▶│     (C++)       │
│                 │◀────│                  │◀────│                 │
└─────────────────┘     └──────────────────┘     └─────────────────┘
                               │
                        ┌──────┴──────┐
                        │  VERBOSE    │
                        │  LOG/ERRORS │
                        │  (per AI)   │
                        └─────────────┘
```

**Lex becomes the "safety shell" around Imperium Engine.**

---

## 3. Architecture

### 3.1 Layer Position

```
┌─────────────────────────────────────────────────────┐
│                    Aether                           │
├─────────────────────────────────────────────────────┤
│                  Lex Neural                         │
├─────────────────────────────────────────────────────┤
│                  Lex Shader                         │
├─────────────────────────────────────────────────────┤
│                  Lex Core        ◀── NEW BACKEND    │
│     (Imperium FFI, Engine Bridge, Validation)       │
├─────────────────────────────────────────────────────┤
│                  Lex Base                           │
└─────────────────────────────────────────────────────┘
```

### 3.2 Components

| Component | Location | Purpose |
|-----------|----------|---------|
| `lex_runtime.cpp` | `C:\Imperium 2.0\lex_engine\src\runtime\` | FFI interface |
| `engine_bridge.cpp` | `C:\Imperium 2.0\lex_engine\src\bridge\` | Chiama Imperium Engine |
| `verbose_logger.cpp` | `C:\Imperium 2.0\lex_engine\src\runtime\` | Structured error reporting |
| `lex_engine.dll` | `C:\Imperium 2.0\build\` | Output DLL |

**Nota:** Lex Engine è PRIVATO — vive dentro Imperium 2.0, non nel repo pubblico di Lex.

### 3.3 Data Flow

```
┌──────────────────────────────────────────────────────────────────┐
│                        WRITE .lex FILE                           │
│                                                                  │
│   entity Warrior {                                               │
│       pos: (10, 0, 5)                                            │
│       health: 100                                                │
│       faction: "Player"                                          │
│   }                                                              │
└────────────────────────────┬─────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────────┐
│                     LEX COMPILER RUNTIME                         │
│                                                                  │
│   1. Parse .lex → AST                                            │
│   2. Validate semantically                                       │
│   3. Generate FFI calls                                          │
│   4. Execute OR serialize                                        │
│                                                                  │
│   ERROR EXAMPLE:                                                 │
│   {                                                              │
│     "code": "LEX_IMP_001",                                       │
│     "message": "Entity 'Warrior' requires 'faction' property",   │
│     "file": "scene.lex",                                         │
│     "line": 1,                                                   │
│     "suggestion": "Add: faction: \"Player\""                     │
│   }                                                              │
└────────────────────────────┬─────────────────────────────────────┘
                             │
                             ▼
┌──────────────────────────────────────────────────────────────────┐
│                     IMPERIUM ENGINE (C++)                        │
│                                                                  │
│   engineBridge->createEntity("Warrior");                         │
│   engineBridge->setEntityPosition(id, 10, 0, 5);                 │
│   engineBridge->setEntityProperty(id, "health", 100);            │
│   engineBridge->setEntityProperty(id, "faction", "Player");      │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
```

---

## 4. API Design

### 4.1 Engine FFI Functions (Existing)

From `C:\Imperium 2.0\engine\src\Engine\EngineBridge.h`:

```cpp
// Lifecycle
bool initialize(u32 width, u32 height);
void shutdown();
bool beginFrame();
void endFrame();
void present();

// Entities
u64 createEntity(const char* name);
bool deleteEntity(u64 id);
void setEntityPosition(u64 id, float x, float y, float z);
u32 getEntityCount();

// Scene
bool saveScene(const char* path);
bool loadScene(const char* path);
```

### 4.2 Lex Runtime FFI (New)

```cpp
// lex_runtime.h - FFI interface for Rust Editor

extern "C" {
    // Lifecycle
    LEX_API bool lex_runtime_init(const char* engine_dll_path);
    LEX_API void lex_runtime_shutdown();
    
    // Compilation
    LEX_API bool lex_compile_file(const char* lex_path);
    LEX_API bool lex_compile_string(const char* lex_source);
    
    // Execution
    LEX_API bool lex_execute_compiled();
    
    // Error Handling
    LEX_API const char* lex_get_last_error_json();
    LEX_API void lex_clear_errors();
    
    // Verbose Mode
    LEX_API void lex_set_verbose(bool enabled);
    LEX_API const char* lex_get_verbose_log();
    
    // Engine Bridge Pass-through
    LEX_API uint64_t lex_engine_create_entity(const char* name);
    LEX_API void lex_engine_set_position(uint64_t id, float x, float y, float z);
    LEX_API bool lex_engine_save_scene(const char* path);
    LEX_API bool lex_engine_load_scene(const char* path);
}
```

### 4.3 Verbose Error Format

```json
{
  "severity": "error",
  "code": "LEX_IMP_001",
  "category": "validation",
  "message": "Entity 'Warrior' requires 'faction' property",
  "file": "scene.lex",
  "location": {
    "line": 1,
    "column": 1,
    "start": 0,
    "end": 20
  },
  "context": {
    "entity_type": "unit",
    "missing_property": "faction",
    "valid_values": ["Player", "Enemy", "Neutral"]
  },
  "suggestion": "Add: faction: \"Player\"",
  "ai_hint": "This is a required property for all entities. Check schema for required fields."
}
```

### 4.4 Error Codes

| Code | Category | Description |
|------|----------|-------------|
| LEX_IMP_001 | Validation | Missing required property |
| LEX_IMP_002 | Validation | Invalid property value |
| LEX_IMP_003 | Reference | Undefined entity reference |
| LEX_IMP_004 | Type | Type mismatch (expected int, got string) |
| LEX_IMP_005 | Engine | Engine call failed |
| LEX_IMP_006 | Resource | Resource not found |
| LEX_IMP_007 | Scene | Scene operation failed |
| LEX_IMP_008 | FFI | FFI bridge error |

---

## 5. Lex Core Keywords (New)

Extended keywords for Imperium integration:

```lex
// Entity definition
entity UnitName {
    pos: (x, y, z)
    rot: (x, y, z, w)      // quaternion
    scale: (x, y, z)
    health: 100
    faction: "Player"
    model: "path/to/model.glb"
}

// Scene definition
scene "Level1" {
    entities: [Warrior, Tower, Resource]
    environment: {
        skybox: "sky.hdr"
        fog: (0.1, 0.2, 0.3, 0.5)  // rgba, density
    }
}

// Resource definition
resource Gold {
    amount: 1000
    node: (50, 0, 50)
}

// Trigger definition
trigger "OnZoneEnter" {
    condition: entity.pos.x > 100
    action: spawn_enemy("Goblin", 5)
}

// Engine command
engine {
    load_scene: "Level1"
    set_time: 12.0
    set_weather: "clear"
}
```

---

## 6. Implementation Plan

### Phase 1: Backend Foundation
- [ ] Create `imperium_backend.cpp/h`
- [ ] Implement entity/scene generation
- [ ] Add Engine FFI call generation

### Phase 2: Runtime
- [ ] Create `lex_runtime.dll`
- [ ] Implement FFI interface
- [ ] Add Engine DLL loading

### Phase 3: Verbose Mode
- [ ] Create `VerboseLogger` class
- [ ] Implement JSON error output
- [ ] Add AI hints system

### Phase 4: Integration
- [ ] Test with Imperium Engine
- [ ] Test with Rust Editor
- [ ] Document API

### Phase 5: Rust Bridge Fix
- [ ] Route Rust → Lex Runtime → Engine
- [ ] Test renderer initialization
- [ ] Verify hot-reload works

---

## 7. Benefits

| Benefit | Before | After |
|---------|--------|-------|
| Error clarity | C++ crash | JSON with suggestion |
| Agent productivity | Struggles with C++ | Writes .lex easily |
| Hot reload | Recompile | Edit .lex, reload |
| Rust-C++ issues | Direct FFI crashes | Lex mediates safely |
| Debugging | GDB/LLDB | Structured logs |

---

## 8. Risks

| Risk | Mitigation |
|------|------------|
| Performance overhead | Compile mode for production |
| FFI complexity | Well-defined interface, tests |
| Feature coverage | Incremental API exposure |

---

## 9. Open Questions

1. Should Lex Runtime support **async operations** for large scenes?
2. Should we support **streaming** for big .lex files?
3. How to handle **engine callbacks** (events from Engine to Lex)?

---

## 10. References

- `C:\Lex\docs\LEX_ARCHITECTURE.md` — Layer system
- `C:\Imperium 2.0\engine\src\Engine\EngineBridge.h` — Engine API
- `C:\Imperium 2.0\engine\src\Engine\OffscreenRendererFFI.cpp` — FFI example
