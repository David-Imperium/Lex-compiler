# Lex Tiers

Lex offers multiple tiers to match different needs — from free modding tools to professional game development.

## Quick Comparison

| Tier | Price | Backends | Best For |
|------|-------|----------|----------|
| **Lex (Free)** | FREE | 6 backends | Modders, hobbyists |
| **Core** | $49 lifetime | +9 backends | Indie developers |
| **Net** | $69 lifetime | +4 RPC backends | Multiplayer devs |
| **Shader** | $149 lifetime | +3 shader backends | Graphics engineers |
| **Neural** | $149 lifetime | +2 AI backends | AI integration |

---

## Lex (Free)

**MIT License — No purchase required**

### Backends
- Lua — Gameplay scripts
- JSON — Data files
- Godot (GDScript) — Godot Engine
- Unity (C#) — Unity game development
- LÖVE2D — Lua framework
- Defold — Defold engine

### Features
- Module system
- Visibility filtering (modder/developer modes)
- Schema-driven validation
- AI context generator
- Query system

---

## Core ($49 lifetime)

**For indie developers who need more engine support**

### Backends (+9)
- **Unreal** — C++ Headers for Unreal Engine
- **GameMaker** — GML scripts
- **Construct 3** — Event sheets
- **Bevy** — Rust ECS
- **Pygame** — Python game framework
- **Raylib** — C game library
- **C++ Header** — Raw C++ data structures
- **Rust** — Rust structs and enums
- **Python** — Python dataclasses

### Features
- Multi-file projects
- Advanced expressions
- All Free features

---

## Net ($69 lifetime)

**For multiplayer and networked games**

### Backends (+4)
- **RPC** — Generic RPC output
- **RPC-Python** — Python server bindings
- **RPC-Go** — Go server bindings
- **RPC-Full** — Full stack (client + server)

### Features
- Multiplayer data synchronization
- Server code generation
- All Core features

---

## Shader ($149 lifetime)

**For graphics engineers**

### Backends (+3)
- **GLSL** — OpenGL shaders
- **HLSL** — DirectX shaders
- **SPIRV** — Vulkan shaders

### Features
- Cross-platform shader compilation
- Material system DSL
- All Core features

---

## Neural ($149 lifetime)

**For AI integration**

### Backends (+2)
- **ONNX** — ONNX model integration
- **TensorRT** — NVIDIA TensorRT

### Features
- AI model integration
- Upscaling backends (DLSS, XeSS, FSR)
- All Core features

---

## Pricing

### Early Bird (Limited Time)

| Tier | Price |
|------|-------|
| Core | $49 lifetime |
| Net | $69 lifetime |
| Shader | $149 lifetime |
| Neural | $149 lifetime |

**Early adopters keep lifetime access forever.**

### Bundle Pricing

| Bundle | Tiers Included | Price | Savings |
|--------|----------------|-------|---------|
| **All-in-One** | Core + Net + Shader + Neural | $299 | $67 |

### Standard Pricing (After Launch)

| Tier | Monthly | Annual | Lifetime |
|------|---------|--------|----------|
| Core | $9/mo | $79/year | Not available |
| Net | $12/mo | $99/year | Not available |
| Shader | $29/mo | $249/year | Not available |
| Neural | $29/mo | $249/year | Not available |

**Why the change?**
Lifetime pricing is for early supporters. Once Lex matures, we'll switch to subscriptions for sustainable development.

---

## FAQ

### Why Lex?

**Why use Lex instead of writing JSON/Lua directly?**
You write once, compile to any engine. Change your engine? Recompile. No manual translation. No copy-paste errors. Your `.lex` file is the single source of truth.

**Why not just use a spreadsheet?**
Spreadsheets are great for data, but:
- No type validation
- No references between tables
- No visibility filtering
- No code generation
- Hard to version control

Lex gives you all of that + compiles to real code.

**How is Lex different from other DSLs?**
- **No lock-in:** Output is standard code, not a proprietary format
- **Multi-target:** One file → 6+ engines
- **Schema-agnostic:** Define your own types, not hardcoded to any game
- **Offline:** No cloud, no API, runs locally

**Is Lex only for games?**
Primarily, but not exclusively. Any project that needs:
- Declarative data definitions
- Multiple output formats
- Type validation
- Cross-references

...can use Lex. Some non-game use cases: configuration files, content management, API schemas.

**What if my engine isn't supported?**
- Free backends: lua, json work everywhere
- Core backends: more engines coming
- Custom backend: implement the `Backend` interface (C++)

Or [request a backend](https://github.com/David-Imperium/Lex-compiler/issues).

**Is Lex production-ready?**
Yes. Lex (free) is stable with 35+ tests, used in production projects. Core backends are newer but follow the same quality standards.

---

### Pricing & Licensing

**Can I use Lex commercially?**
Yes! The MIT license allows any use — personal, commercial, open source. No attribution required.

**Does every team member need a license?**
Yes, each developer needs their own license. Contact us for team pricing (5+ seats).

**Can I upgrade later?**
Early adopters get upgrade discounts when new tiers launch.

**Does lifetime license expire?**
Never. Pay once, use forever. Including all future updates to that tier.

**What happens if I cancel my subscription?**
You lose access to premium backends when the billing period ends. Your `.lex` files still work with free backends.

**Do I need internet to use Lex?**
No. Lex works completely offline. License verification is local, no call-home.

**Can I get a refund?**
Yes, 30-day money-back guarantee. No questions asked.

**What backends are actually ready?**
- Free: lua, json, godot, unity, love2d, defold (all ready)
- Core: unreal (ready), others planned
- Net: rpc (ready), others planned

**When will other backends be ready?**
Check [BACKENDS.md](BACKENDS.md) for status. No fixed dates — quality over speed.

---

## How to Purchase

1. Visit [GitHub Sponsors](https://github.com/sponsors/David-Imperium)
2. Choose your tier
3. Receive Lex-Plus package + license key
4. Install and activate

---

## FAQ

**Can I use Lex commercially?**
Yes! MIT license allows any use.

**Do I need separate licenses for team members?**
Yes, each developer needs their own license. Contact for team pricing.

**Lifetime license never expires?**
Correct. Pay once, use forever.
