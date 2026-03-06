# Lex: Beyond Gaming

## The Hidden Power of a Declarative Transpiler

Lex was born in the game development world, but its architecture reveals something far more powerful: **a universal declarative-to-code compiler**.

### The Core Insight

Traditional code generation requires you to learn a new DSL for each target:
- OpenAPI/Swagger for REST APIs
- Protocol Buffers for RPC
- Prisma for databases
- Kubernetes YAML for infrastructure

**What if you could write your domain model once, in a simple, readable format, and generate code for ALL of these?**

```lex
// Define your domain ONCE
struct User {
    id: int
    name: string
    email: string
    created_at: datetime
}

method GetUser {
    input: int
    output: User
}

method CreateUser {
    input: User
    output: User
}
```

### Multi-Target Generation

```bash
# One definition, many outputs
lexc api.lex -t python,go,typescript,sql,openapi,grpc
```

This single command generates:
- **Python** dataclasses and FastAPI handlers
- **Go** structs and methods
- **TypeScript** interfaces for the frontend
- **SQL** schema and migrations
- **OpenAPI** spec for documentation
- **gRPC** protobuf definitions

### The Architecture

```
                    ┌─────────────────┐
                    │   Your .lex     │
                    │   Definition    │
                    └────────┬────────┘
                             │
                    ┌────────▼────────┐
                    │    Lex Parser   │
                    │    (AST)        │
                    └────────┬────────┘
                             │
        ┌────────────────────┼────────────────────┐
        │                    │                    │
   ┌────▼────┐         ┌────▼────┐         ┌────▼────┐
   │ Python  │         │   Go    │         │   SQL   │
   │ Backend │         │ Backend │         │ Backend │
   └────┬────┘         └────┬────┘         └────┬────┘
        │                    │                    │
   ┌────▼────┐         ┌────▼────┐         ┌────▼────┐
   │  .py    │         │   .go   │         │  .sql   │
   └─────────┘         └─────────┘         └─────────┘
```

### What Makes This Possible

1. **Schema-Agnostic Parser**: Lex parses definitions without knowing what they represent
2. **Pluggable Backends**: Each backend is a self-contained code generator
3. **`--types` Flag**: Define your own domain types (`struct`, `method`, `table`, `message`...)
4. **No Runtime Dependency**: Generated code is standalone, no Lex library required

### Real-World Use Cases

#### API Development
Define your API once, generate server code, client SDKs, and documentation simultaneously.

#### Database Schemas
Define entities once, generate SQL DDL, ORM models, and migration scripts.

#### Infrastructure as Code
Define your infrastructure declaratively, generate Terraform, Kubernetes manifests, or CloudFormation.

#### Protocol Buffers / gRPC
Generate protobuf definitions AND the implementation code from the same source.

#### State Machines
Define states and transitions, generate implementation for any language.

### The Vision

**Write once, generate everywhere.**

The goal isn't to replace specialized tools like OpenAPI or Prisma. It's to have a **single source of truth** for your domain model, with the ability to generate for any target you need.

### Current Status

| Target | Status | Notes |
|--------|--------|-------|
| Lua | ✅ Stable | Game modding |
| JSON | ✅ Stable | Data serialization |
| Godot (GDScript) | ✅ Stable | Godot Engine |
| Unity (C#) | ✅ Stable | Unity Engine |
| Love2D (Lua) | ✅ Stable | LÖVE framework |
| Defold (Lua) | ✅ Stable | Defold Engine |
| RPC (Python/Go) | ✅ New | JSON-RPC protocols |

### Future Backends (Community Welcome)

- **OpenAPI/Swagger** - REST API documentation
- **TypeScript** - Frontend interfaces
- **Prisma/SQL** - Database schemas
- **gRPC/Protobuf** - RPC protocols
- **Kubernetes** - Infrastructure as code
- **GraphQL** - Schema generation
- **Rust** - Systems programming
- **Python** - Data classes, Pydantic models

### How to Contribute a Backend

```cpp
// src/codegen/your_backend.cpp
class YourBackend : public Backend {
public:
    std::string name() const override { return "your_target"; }
    std::string file_extension() const override { return ".ext"; }
    std::string generate(const AST& ast) override {
        // Your generation logic
    }
};

// Register it
registry.register_factory("your_target", []() {
    return std::make_unique<YourBackend>();
});
```

### Why This Matters

1. **Consistency**: One source of truth, no sync issues between API/server/client
2. **Speed**: Generate boilerplate, focus on business logic
3. **Flexibility**: Switch targets without rewriting definitions
4. **Collaboration**: Non-developers can read and write `.lex` files
5. **Extensibility**: Create your own backends for custom needs

---

## The Bottom Line

**Lex is not just a game content transpiler.** It's a new way of thinking about code generation: declarative definitions that transcend any single target or language.

Start with game development. Stay for the universal transpilation.

---

*Want to create a new backend? Check out `src/codegen/` for examples, or open an issue to discuss your idea.*
