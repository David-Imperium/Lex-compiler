# Aether Debug Protocol — RFC

**Version:** 0.1.0-draft
**Date:** 2026-03-02
**Status:** Draft
**Author:** David
**Target:** Imperium 2.0 + Lex + AI Agents

---

## Problem Statement

When AI agents debug code, they lack:

1. **Symbolic stack traces** - Crash logs show `???` instead of function names
2. **Structured context** - Agents must parse unstructured text logs
3. **Causal hints** - No automatic analysis of likely causes
4. **Runtime state** - Cannot inspect variables at crash time

Current workflow:
```
Crash → Agent reads log → Sees "???" → Guesses → Tries fix → Repeat 10-20x
```

Target workflow:
```
Crash → Structured JSON → Agent analyzes → Suggests fix → 1-2 attempts
```

---

## Solution: Aether Debug Protocol

### Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                    AETHER DEBUG ARCHITECTURE                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                      │
│   ┌─────────────┐     ┌─────────────────┐     ┌─────────────────┐  │
│   │   CRASH     │     │   CRASH LOG     │     │   CRASH JSON    │  │
│   │   HANDLER   │────▶│   (human)       │────▶│   (agent)       │  │
│   └─────────────┘     └─────────────────┘     └─────────────────┘  │
│         │                                              │             │
│         │                                              ▼             │
│         │                                     ┌─────────────────┐  │
│         │                                     │    AETHER       │  │
│         │                                     │    ANALYZER     │  │
│         │                                     └────────┬────────┘  │
│         │                                              │             │
│         │                                              ▼             │
│         │                                     ┌─────────────────┐  │
│         │                                     │   DEBUG CONTEXT │  │
│         │                                     │   (for agents)  │  │
│         │                                     └─────────────────┘  │
│         │                                              │             │
│         ▼                                              ▼             │
│   ┌─────────────────────────────────────────────────────────────┐  │
│   │                    OUTPUT FILES                              │  │
│   │                                                              │  │
│   │   crash_20260302.json     ← Machine-readable for agents      │  │
│   │   crash_20260302.md       ← Human-readable summary           │  │
│   │   debug_context.json      ← Project-wide debug state         │  │
│   │   suggested_fixes.md      ← AI-generated hypotheses          │  │
│   │                                                              │  │
│   └─────────────────────────────────────────────────────────────┘  │
│                                                                      │
└─────────────────────────────────────────────────────────────────────┘
```

---

## Implementation Plan

### Phase 1: Enhanced Crash Handler

Extend existing `CrashHandler.cpp` to output JSON:

```cpp
// New file: engine/src/Debug/CrashJsonExporter.cpp

namespace Imperium::Debug {

struct CrashInfo {
    std::string timestamp;
    std::string exceptionCode;
    std::string exceptionName;
    uint64_t exceptionAddress;
    std::string accessType;
    uint64_t accessAddress;
    
    struct StackFrame {
        int index;
        std::string module;
        std::string function;
        std::string file;
        int line;
        uint64_t address;
    };
    std::vector<StackFrame> stackTrace;
    
    struct RegisterDump {
        uint64_t rip, rsp, rbp;
        uint64_t rax, rbx, rcx, rdx;
        uint64_t rsi, rdi;
    } registers;
    
    // NEW: Analysis hints
    struct AnalysisHints {
        std::vector<std::string> likelyCauses;
        std::vector<std::string> relatedFiles;
        std::string suggestedArea;
    } hints;
};

void CrashJsonExporter::export(const std::string& path, const CrashInfo& info) {
    nlohmann::json j;
    j["version"] = "1.0";
    j["generator"] = "Aether Debug Protocol v0.1";
    j["crash"] = {
        {"timestamp", info.timestamp},
        {"exception", {
            {"code", info.exceptionCode},
            {"name", info.exceptionName},
            {"address", info.exceptionAddress}
        }}
    };
    
    if (!info.accessType.empty()) {
        j["crash"]["access_violation"] = {
            {"type", info.accessType},
            {"address", info.accessAddress}
        };
    }
    
    // Stack trace with symbols
    for (const auto& frame : info.stackTrace) {
        j["stack_trace"].push_back({
            {"index", frame.index},
            {"module", frame.module},
            {"function", frame.function},
            {"file", frame.file},
            {"line", frame.line},
            {"address", frame.address}
        });
    }
    
    // Analysis hints (NEW)
    j["analysis"] = {
        {"likely_causes", info.hints.likelyCauses},
        {"related_files", info.hints.relatedFiles},
        {"suggested_area", info.hints.suggestedArea}
    };
    
    std::ofstream file(path);
    file << j.dump(2);
}

} // namespace Imperium::Debug
```

### Phase 2: Symbol Resolution

Fix PDB loading to get real function names:

```cpp
// Enhanced symbol loading in CrashHandler.cpp

bool LoadSymbolsProperly() {
    // 1. Set symbol path to include PDB directory
    std::string symbolPath = ".;C:\\Imperium 2.0\\build\\bin\\Release";
    SymSetSearchPath(GetCurrentProcess(), symbolPath.c_str());
    
    // 2. Force load PDB for main module
    char modulePath[MAX_PATH];
    GetModuleFileNameA(nullptr, modulePath, MAX_PATH);
    
    DWORD64 baseAddr = SymLoadModuleEx(
        GetCurrentProcess(),
        nullptr,
        modulePath,
        nullptr,
        0,
        0,
        nullptr,
        0
    );
    
    return baseAddr != 0;
}
```

### Phase 3: Aether Analyzer

Rust component that analyzes crashes and generates context:

```rust
// aether/src/analyzer/crash_analyzer.rs

pub struct CrashAnalyzer {
    project_context: ProjectContext,
}

impl CrashAnalyzer {
    pub fn analyze(&self, crash: &CrashInfo) -> DebugContext {
        let mut context = DebugContext::new();
        
        // 1. Identify crash area
        let area = self.identify_crash_area(&crash.stack_trace);
        context.crash_area = area;
        
        // 2. Find related code
        context.related_files = self.find_related_files(&crash);
        
        // 3. Generate hypotheses
        context.hypotheses = self.generate_hypotheses(&crash, &area);
        
        // 4. Find similar past crashes
        context.similar_crashes = self.find_similar(&crash);
        
        // 5. Suggest debugging steps
        context.suggested_steps = self.suggest_steps(&crash);
        
        context
    }
    
    fn generate_hypotheses(&self, crash: &CrashInfo, area: &str) -> Vec<Hypothesis> {
        let mut hypotheses = Vec::new();
        
        // Access violation at low address = null pointer
        if crash.exception_code == "EXCEPTION_ACCESS_VIOLATION" 
            && crash.access_address < 0x1000 {
            hypotheses.push(Hypothesis {
                confidence: 0.9,
                description: "Null pointer dereference".to_string(),
                likely_cause: format!(
                    "A pointer in {} is null or uninitialized",
                    area
                ),
                suggested_fix: "Check if pointer is valid before use".to_string(),
            });
        }
        
        // RAX = 0x10 = likely null + offset
        if crash.registers.rax < 0x100 {
            hypotheses.push(Hypothesis {
                confidence: 0.8,
                description: "Accessing member of null object".to_string(),
                likely_cause: format!(
                    "Object pointer is null, accessing member at offset 0x{:x}",
                    crash.access_address
                ),
                suggested_fix: "Add null check before member access".to_string(),
            });
        }
        
        hypotheses
    }
}
```

### Phase 4: Agent Integration

Generate `.factory/context/debug.json` for Droid/Cursor:

```json
{
  "version": "1.0",
  "type": "crash_analysis",
  "timestamp": "2026-03-02T15:30:00Z",
  
  "summary": {
    "exception": "EXCEPTION_ACCESS_VIOLATION",
    "crash_area": "EditorApp::handle_shortcuts",
    "confidence": 0.85,
    "primary_hypothesis": "Null pointer dereference in entity deletion"
  },
  
  "stack_trace_resolved": [
    {
      "function": "EditorApp::handle_shortcuts",
      "file": "editor/src/app.rs",
      "line": 85,
      "confidence": 0.95
    },
    {
      "function": "Scene::delete_entity",
      "file": "editor/src/scene.rs", 
      "line": 45,
      "confidence": 0.90
    }
  ],
  
  "hypotheses": [
    {
      "confidence": 0.90,
      "description": "Null pointer in selected_entities during deletion",
      "evidence": [
        "Access violation at address 0x10 (null + offset)",
        "RAX register contains 0x10",
        "Crash occurs in entity deletion code path"
      ],
      "suggested_fix": {
        "file": "editor/src/app.rs",
        "line": 82,
        "current_code": "self.scene.delete_entity(*id);",
        "suggested_code": "if self.scene.entity_exists(*id) {\n    self.scene.delete_entity(*id);\n}"
      }
    },
    {
      "confidence": 0.70,
      "description": "Race condition with scene mutation",
      "evidence": [
        "HashSet iteration while modifying",
        "No synchronization visible"
      ],
      "suggested_fix": "Collect entities to delete first, then delete"
    }
  ],
  
  "related_code": [
    {
      "file": "editor/src/scene.rs",
      "function": "delete_entity",
      "relevance": "high"
    },
    {
      "file": "editor/src/app.rs",
      "function": "handle_shortcuts",
      "relevance": "critical"
    }
  ],
  
  "suggested_investigation": [
    "1. Read editor/src/app.rs lines 80-90",
    "2. Check Scene::delete_entity implementation",
    "3. Verify entity exists before deletion",
    "4. Add defensive null checks"
  ]
}
```

---

## File Structure

```
C:\Imperium 2.0\
├── logs/
│   ├── crash_20260302_153000.log      # Human-readable (existing)
│   ├── crash_20260302_153000.json     # Machine-readable (NEW)
│   └── crash.dmp                      # Minidump (existing)
│
├── .factory/
│   └── context/
│       └── debug.json                 # Agent context (NEW)
│
├── aether/                            # NEW: Aether analyzer module
│   ├── src/
│   │   ├── analyzer/
│   │   │   ├── crash_analyzer.rs
│   │   │   └── hypothesis_generator.rs
│   │   └── context/
│   │       └── agent_context.rs
│   └── Cargo.toml
│
└── engine/src/Debug/
    ├── CrashHandler.cpp               # Enhanced (existing)
    ├── CrashJsonExporter.cpp          # NEW
    └── CrashJsonExporter.h
```

---

## Integration with Lex

Lex can generate debug context for game data:

```lex
// Game data with debug hints
structure SteamFactory {
    era: Steampunk
    cost: { Coal: 8, Steel: 5 }
    
    // Debug context (NEW)
    debug: {
        common_issues: [
            "Null pointer if scene not initialized",
            "Crash if Coal resource missing"
        ],
        validation: "Check scene != null before create"
    }
}
```

Generated output includes validation code:

```cpp
// Auto-generated by Lex
void SteamFactory::validate(Context& ctx) {
    if (!ctx.scene) {
        IMP_DEBUG_WARN("SteamFactory: scene is null");
    }
    if (!ctx.hasResource("Coal")) {
        IMP_DEBUG_ERROR("SteamFactory: Coal resource missing");
    }
}
```

---

## Benefits

| Before | After |
|--------|-------|
| Agent sees `???` in stack trace | Agent sees resolved function names |
| 10-20 iterations to fix crash | 1-3 iterations with hints |
| Manual log parsing | Structured JSON analysis |
| No causal hints | Automatic hypothesis generation |
| No code suggestions | Suggested fixes with confidence |

---

## Implementation Priority

1. **Phase 1** (1-2 days): Enhanced CrashHandler with JSON output
2. **Phase 2** (1 day): Fix PDB symbol loading
3. **Phase 3** (3-5 days): Aether Analyzer in Rust
4. **Phase 4** (1 day): Agent context generation

**Total: ~1 week for MVP**

---

## Success Metrics

| Metric | Before | Target |
|--------|--------|--------|
| Stack trace resolution | 20% `???` | 95% resolved |
| Avg debug iterations | 10-20 | 1-3 |
| Time to fix | 30-60 min | 5-10 min |
| Agent confidence | Low | High (hints provided) |

---

## Future Extensions

1. **Live Debugging** - DAP integration for runtime inspection
2. **Crash Telemetry** - Aggregate crash data across users
3. **Auto-fix** - AI can propose and apply fixes automatically
4. **Regression Detection** - Match new crashes to known patterns
