# AGENTS.md — Coding Guide for Lex

This document provides coding conventions for AI agents working on the Lex compiler.

---

## Build Commands

```bash
# Configure
cmake -B build -S .

# Build
cmake --build build --config Release

# Run tests
.\build\tests\Debug\lexer_tests.exe
```

---

## Code Style

### Namespace
```cpp
namespace lex {
    // All code here
} // namespace lex
```

### Naming Conventions

| Type | Style | Example |
|------|-------|---------|
| Classes | PascalCase | `class Lexer` |
| Methods | snake_case | `void tokenize()` |
| Members | snake_case_ | `std::string source_` |
| Constants | UPPER_CASE | `KEYWORDS` |
| Enums | PascalCase | `enum class TokenType` |

### Include Guards
```cpp
#pragma once  // Always use this
```

### Includes
```cpp
// System headers first
#include <string>
#include <vector>

// Internal headers second
#include "../ast/ast.h"
```

### Types
```cpp
std::string str;              // Always std::string, never char*
std::unique_ptr<T> ptr;       // Smart pointers for ownership
std::vector<T> vec;           // Vectors for sequences
std::map<K, V> map;           // Maps for key-value
std::variant<Ts...> var;      // Type-safe unions
int64_t large_int;            // Large integers
```

### Class Structure
```cpp
class ClassName {
public:
    explicit ClassName(const std::string& source);
    virtual ~ClassName() = default;

    // Public interface
    bool has_errors() const;

private:
    std::string source_;
    size_t pos_ = 0;
};
```

### Const Correctness
```cpp
// Getters should be const
bool has_errors() const { return !errors_.empty(); }

// Return const ref for heavy types
const std::string& source() const { return source_; }

// Return by value for small types
Token peek() const;
```

---

## Project Structure

```
src/
├── lexer/      # Tokenization (token.h, keywords.h, lexer.h/cpp)
├── parser/     # Parsing (parser.h/cpp)
├── ast/        # AST nodes (ast.h/cpp)
├── semantic/   # Type checking, validation
├── codegen/    # Backends (lua_backend, json_backend)
└── main.cpp    # CLI entry point
```

---

## Testing

- Framework: Catch2
- Location: `tests/`
- Run: `.\build\tests\Debug\lexer_tests.exe`

---

## Commit Style

- Language: English
- Format: `type: description`
- Types: feat, fix, docs, test, refactor, chore

**Never push without explicit permission.**

---

**Version:** 0.3.0
**Last Updated:** 2026-03-02
