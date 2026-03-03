# RFC-PRISM-001: Prism Language Design Document

**Status:** Design (Future Project)
**Created:** 2026-03-03
**Author:** David
**Base:** Fork of Odin (zlib license)

---

## 1. Motivation

### 1.1 Problems with C++

Prism nasce per risolvere i problemi fondamentali di C++:

| Problema | Esempio | Impatto |
|----------|---------|---------|
| **Hidden Control Flow** | Costruttori impliciti, conversioni automatiche | Bug subdoli, behavior inaspettato |
| **Debugging Nightmare** | Template errors di 5000 righe | Perdite ore a capire errori |
| **Memory Safety** | UB, dangling pointers, use-after-free | Crash, security vulnerabilities |
| **Complessità** | Templates, concepts, CRTP, SFINAE | Curva di apprendimento ripida |
| **Build Times** | Header proliferation | Ciclo di sviluppo lento |

### 1.2 Goals

1. **Explicit over Implicit** — Tutto è visibile, niente succede "magicamente"
2. **Debug-First Design** — Errori chiari con suggerimenti, stack trace leggibili
3. **Simple but Powerful** — Poche primitive ortogonali, molto espressive
4. **Game-Dev Friendly** — Data-oriented, SIMD nativo, hot reload
5. **Imperium Integration** — C++ interop per codebase esistente

---

## 2. Base Technology: Odin

### 2.1 Why Odin

| Criterio | Zig | Odin | Carbon |
|----------|-----|------|--------|
| Fork difficulty | 🔴 Hard (370K commit) | 🟢 Easy (4MB codebase) | 🟡 Medium |
| License | MIT | **zlib** (più permissiva) | Apache 2.0 |
| Hidden control flow | ✅ No | ✅ No | ⚠️ Some |
| Custom backend | ✅ WIP | ✅ Tilde (SSA) | ❌ No |
| Game-dev focus | ⚠️ General | ✅ Native | ❌ No |
| C++ interop | ⚠️ C only | ⚠️ C only | ✅ Full |

**Decisione:** Fork Odin per:
- Codebase gestibile (~4.2 MB C++, 64 file)
- Licenza zlib (permette fork commerciale)
- Custom backend "tilde" in sviluppo
- Data-oriented design nativo

### 2.2 Odin Architecture (Reference Implementation)

```
Compiler Pipeline:

  Source Code
       │
       ▼
┌─────────────┐
│  Tokenizer  │  1,128 LOC — Lexer
└─────────────┘
       │
       ▼
┌─────────────┐
│   Parser    │  8,000+ LOC — AST generation
└─────────────┘
       │
       ▼
┌─────────────┐
│   Checker   │  7,500+ LOC — Type checking + semantic analysis
└─────────────┘
       │
       ▼
┌─────────────┐
│   Backend   │  LLVM IR or Tilde (custom SSA)
└─────────────┘
       │
       ▼
   Machine Code
```

**Key Files:**
```
src/
├── tokenizer.cpp      — Lexer
├── parser.cpp/hpp     — Parser + AST (macro-based)
├── checker.cpp/hpp    — Type checker
├── llvm_backend.cpp   — LLVM codegen (current)
├── tilde/             — Custom SSA backend (future)
├── types.cpp          — Type system
└── main.cpp           — Entry point + orchestration
```

---

## 3. Prism Design

### 3.1 Features to Keep (from Odin)

| Feature | Description | Value |
|---------|-------------|-------|
| **Explicit Control Flow** | Niente costruttori impliciti, niente operator overloading nascosto | Debuggable |
| **Distinct Types** | Type-safe newtypes | Type safety |
| **SOA/AOS** | Struct-of-Arrays e Array-of-Structs nativi | Cache-friendly |
| **SIMD Types** | Vector types built-in | Performance |
| **Simple C Interop** | FFI diretto con C | Integration |
| **Fast Compile** | Tilde backend goal | DX |

### 3.2 Features to Add (Prism-specific)

#### 3.2.1 C++ Interop Layer
```
// Prism syntax (proposed)
#import_cpp "imperium.hpp" {
    World :: extern_cpp;
    Entity :: extern_cpp;
}

// Or: Automatic C++ header parser
#cpp_bridge "engine/core.hpp"
```

#### 3.2.2 Improved Error Messages
```
// Current C++ error (5000 lines)
// Prism goal (5 lines):

Error: Cannot assign `string` to `int`
  ┌─ main.prism:10:15
  │
10 │   x := "hello"
  │       ^^^^^^^^ expected `int`, found `string`
  │
  = help: Did you mean to declare x as `string`?
  = help: Or convert with `int.from_string("hello")?`
```

#### 3.2.3 Hot Reload Support
```
// Mark procedures as hot-reloadable
@hot_reload
proc update_world(world: ^World, dt: f32) {
    // Changes to this function can be reloaded at runtime
}
```

#### 3.2.4 Compile-Time Reflection
```
// Get type info at compile time
TypeInfo :: type_info(Entity)

// Iterate struct fields
@reflect
proc serialize(v: $T) -> []u8 {
    for field in T.fields {
        // Automatic serialization
    }
}
```

#### 3.2.5 Optional Memory Safety
```
// Global setting
#set bounds_check = true   // Debug builds
#set bounds_check = false  // Release builds

// Per-scope override
@no_bounds_check
proc hot_path(data: []f32) {
    // Maximum performance
}
```

### 3.3 Syntax Examples

```
// Basic types
x: int = 42
name: string = "Prism"
position: [3]f32 = {1.0, 2.0, 3.0}

// Struct with SOA attribute
@soa
Entity :: struct {
    position: [3]f32,
    velocity: [3]f32,
    health: f32,
}

// Procedure
proc update_entity(e: ^Entity, dt: f32) {
    e.position += e.velocity * dt
}

// SIMD vector
v: float32x4 = {1, 2, 3, 4}
result := v * 2.0  // SIMD multiply

// Error handling (no exceptions)
proc load_file(path: string) -> ([]u8, Error) {
    // Explicit error handling
}

data, err := load_file("test.txt")
if err != nil {
    return err
}
```

---

## 4. Implementation Roadmap (Future)

### Phase 1: Foundation (3-6 months)
- [ ] Fork Odin codebase
- [ ] Build system setup
- [ ] Rename to Prism
- [ ] Basic test suite pass

### Phase 2: C++ Interop (2-3 months)
- [ ] C++ header parser
- [ ] Name mangling bridge
- [ ] VTable support
- [ ] Test with Imperium

### Phase 3: Developer Experience (2-3 months)
- [ ] Error message redesign
- [ ] Better diagnostics
- [ ] VS Code extension
- [ ] Debug info improvements

### Phase 4: Advanced Features (3-6 months)
- [ ] Hot reload runtime
- [ ] Reflection system
- [ ] Custom allocator support
- [ ] Tilde backend completion

### Phase 5: Production Ready (ongoing)
- [ ] Standard library
- [ ] Package manager
- [ ] Documentation
- [ ] Community building

---

## 5. License Compliance

Odin uses zlib-style license:

```
Copyright (c) 2016-2025 Ginger Bill. All rights reserved.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented
2. Altered source versions must be plainly marked as such
3. This notice may not be removed or altered
```

**Prism compliance:**
- Keep original copyright notice
- Mark modified files as "Modified for Prism"
- Add Prism-specific copyright for new code

---

## 6. References

- **Odin Repository:** https://github.com/odin-lang/Odin
- **Odin Tilde Backend:** https://odin.handmade.network/blog/p/2210-working_on_a_custom_backend_for_odin
- **Zig Internals:** https://github.com/mikdusan/zig.internals
- **Carbon Language:** https://github.com/carbon-language/carbon-lang
- **LLVM Backend Architecture:** https://llvm.org/docs/

---

## 7. Open Questions

1. **C++ ABI:** Support Itanium ABI, MSVC ABI, or both?
2. **Runtime:** Minimal runtime or include GC option?
3. **Package Manager:** Build custom or use existing?
4. **Standard Library:** Port Odin's or start fresh?
5. **Target Platforms:** Windows/Linux first, or consoles too?

---

*This document is a design reference. Implementation is a future project.*
