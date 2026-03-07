# Lex Tiers

Lex offers two tiers — free for modders, premium for indie developers.

## Quick Comparison

| Tier | Price | Backends | Best For |
|------|-------|----------|----------|
| **Lex (Free)** | FREE | 6 backends | Modders, hobbyists, open source |
| **Lex-Plus Core** | $49 lifetime | +10 backends | Indie developers, studios |

---

## Lex (Free)

**MIT License — No purchase required**

### Backends (6)
| Backend | Target | Status |
|---------|--------|--------|
| `lua` | Generic Lua | ✅ Ready |
| `json` | Universal JSON | ✅ Ready |
| `gd` / `godot` | Godot 4.x | ✅ Ready |
| `cs` / `unity` | Unity C# | ✅ Ready |
| `love2d` | LÖVE2D | ✅ Ready |
| `defold` | Defold | ✅ Ready |

### Features
- Module system (`use` keyword)
- Visibility filtering (modder/developer modes)
- Schema-driven validation
- AI context generator
- Query system
- VS Code extension

### Distribution
- **Source:** https://github.com/David-Imperium/Lex-compiler
- **License:** MIT (free forever)

---

## Lex-Plus Core ($49 lifetime)

**For indie developers who need more engine support**

### Backends (+10)
| Backend | Target | Status |
|---------|--------|--------|
| `unreal` | Unreal Engine C++ | ✅ Ready |
| `gamemaker` | GameMaker GML | 📋 Planned |
| `rust` | Rust structs | 📋 Planned |
| `cpp` | C++ headers | 📋 Planned |
| `python` | Python dataclasses | 📋 Planned |
| `typescript` | TypeScript | 📋 Planned |
| `go` | Go structs | 📋 Planned |
| `java` | Java records | 📋 Planned |
| `c` | C headers | 📋 Planned |
| `rpc` | RPC protocol | ✅ Ready |

### Features
- All Free features
- Multi-file projects
- Advanced expressions
- Priority support

### Distribution
- **Source:** https://codeberg.org/David-Imperium/Lex-Plus (private, invite-only)
- **License:** Commercial (requires purchase)

---

## Pricing

### Early Bird (Limited Time)

| Tier | Price |
|------|-------|
| **Lex-Plus Core** | $49 lifetime |

**Early adopters keep lifetime access forever.**

### Standard Pricing (After Launch)

| Tier | Monthly | Annual |
|------|---------|--------|
| **Lex-Plus Core** | $9/mo | $79/year |

Lifetime pricing is for early supporters. Once Lex matures, we'll switch to subscriptions.

---

## How to Purchase

1. Visit [GitHub Sponsors](https://github.com/sponsors/David-Imperium)
2. Choose Core tier ($49)
3. Receive Codeberg invite + license key
4. Clone Lex-Plus from Codeberg
5. Build and activate

---

## Backend Status

| Backend | Tier | Status |
|---------|------|--------|
| lua | Free | ✅ Ready |
| json | Free | ✅ Ready |
| gd (Godot) | Free | ✅ Ready |
| cs (Unity) | Free | ✅ Ready |
| love2d | Free | ✅ Ready |
| defold | Free | ✅ Ready |
| unreal | Core | ✅ Ready |
| gamemaker | Core | 📋 Planned |
| rust | Core | 📋 Planned |
| cpp | Core | 📋 Planned |
| python | Core | 📋 Planned |
| typescript | Core | 📋 Planned |
| go | Core | 📋 Planned |
| java | Core | 📋 Planned |
| c | Core | 📋 Planned |
| rpc | Core | ✅ Ready |

---

## FAQ

### General

**Why use Lex instead of JSON/Lua directly?**
Write once, compile to any engine. Change engine? Recompile. No manual translation. Your `.lex` file is the single source of truth.

**Why not just use a spreadsheet?**
Spreadsheets lack: type validation, references, visibility filtering, code generation, version control. Lex gives you all of that.

**Is Lex only for games?**
Primarily, but works for any declarative data: configs, content management, API schemas.

**What if my engine isn't supported?**
- Free: `lua`, `json` work everywhere
- Core: more engines coming
- Custom: implement the `Backend` interface (C++)

Or [request a backend](https://github.com/David-Imperium/Lex-compiler/issues).

### Licensing

**Can I use Lex commercially?**
Yes! MIT license allows any use — personal, commercial, open source.

**Does every team member need a license?**
Yes, each developer needs their own license. Contact for team pricing (5+ seats).

**Does lifetime license expire?**
Never. Pay once, use forever. Including all future updates.

**Do I need internet to use Lex?**
No. Lex works completely offline. License verification is local.

**Can I get a refund?**
Yes, 30-day money-back guarantee. No questions asked.
