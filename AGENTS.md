# AGENTS.md - Agent Coding Guide for Lex

This document provides essential information for agentic coding systems working on the Lex compiler codebase.

---

## Build Commands

### Standard Build
```bash
# Configure (first time)
cmake -B build -S .

# Build
cmake --build build

# Clean
cmake --build build --clean-first
```

### With Tests (Not Yet Implemented)
```bash
# Enable tests and build
cmake -B build -S . -DBUILD_TESTS=ON
cmake --build build

# Run all tests (when test framework is added)
ctest --test-dir build

# Run single test (example - adapt to actual test framework)
./build/tests/lexer_tests --run-test=Lex.IdentifierKeyword
```

### Build with Specific Configuration
```bash
# Release build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Debug build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
```

---

## Code Style Guidelines

### File Organization
```
src/
├── lexer/          # Lexical analysis (tokens, keywords, lexer)
├── parser/         # Syntax parsing (recursive descent)
├── ast/            # AST node definitions and visitors
├── codegen/        # Backend implementations (lua, json, react)
├── error.h/cpp     # Error types and formatting (TODO)
└── main.cpp        # CLI entry point
```

### Include Guards
```cpp
// Always use #pragma once
#pragma once
```

### Includes
```cpp
// System headers first (angle brackets)
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Internal headers second (relative paths)
#include "../ast/ast.h"
#include "token.h"
```

### Namespace
```cpp
// All code wrapped in lex namespace
namespace lex {

class MyClass {
    // ...
};

} // namespace lex
```

### Naming Conventions

**Types and Classes:** PascalCase
```cpp
class Lexer;
class Token;
enum class TokenType;
```

**Methods and Functions:** snake_case
```cpp
void tokenize();
Token next_token();
std::unique_ptr<Definition> parse_definition();
```

**Member Variables:** snake_case with trailing underscore
```cpp
class Lexer {
private:
    std::string source_;
    size_t pos_;
    int line_ = 1;
    std::vector<std::string> errors_;
};
```

**Constants:** UPPER_CASE
```cpp
inline const std::map<std::string, TokenType> KEYWORDS = {
    {"era", TokenType::ERA},
    {"structure", TokenType::STRUCTURE}
};
```

**Enums:** PascalCase for enum class, UPPER_CASE for values
```cpp
enum class Type { INTEGER, FLOAT, STRING, BOOLEAN };
enum class TokenType { ERA, STRUCTURE, ERROR };
```

### Type System
```cpp
// Use standard library types consistently
std::string str;              // Not char*, always std::string
std::vector<Token> tokens;   // Vector for sequences
std::map<std::string, Token> map;  // Map for key-value
std::unique_ptr<T> ptr;      // Smart pointers for ownership

// Use std::variant for type-safe unions
std::variant<int64_t, double, std::string, bool> value;

// Use int64_t for large integers (from <cstdint>)
int64_t large_number;
```

### Class Structure
```cpp
class ClassName {
public:
    // Constructors (explicit for single-arg)
    explicit ClassName(const std::string& source);

    // Virtual destructor for polymorphic classes
    virtual ~ClassName() = default;

    // Public interface
    std::vector<Token> tokenize();
    bool has_errors() const;
    const std::vector<std::string>& errors() const;

protected:
    // Protected utilities for derived classes

private:
    // Private implementation
    std::string source_;
    size_t pos_ = 0;

    // Private helper methods
    Token scan_token();
    void skip_whitespace();
};
```

### Method Guidelines

**Const correctness:**
```cpp
// Getter should be const
bool has_errors() const { return !errors_.empty(); }
const std::vector<std::string>& errors() const { return errors_; }

// Return by value for small types
Token peek() const;

// Return by const reference for heavy types
const std::string& source() const { return source_; }
```

**Inline simple getters:**
```cpp
bool has_errors() const { return !errors_.empty(); }
int line() const { return line_; }
```

**Use explicit for single-argument constructors:**
```cpp
explicit Lexer(const std::string& source);  // Prevents implicit conversion
```

### Error Handling Pattern
```cpp
// Accumulate errors in vector
class Lexer {
private:
    std::vector<std::string> errors_;

public:
    bool has_errors() const { return !errors_.empty(); }
    const std::vector<std::string>& errors() const { return errors_; }
};

// Report errors with context
void Lexer::error(const std::string& message, int line, int column) {
    errors_.push_back(format_error(message, line, column));
}
```

### Virtual Base Classes
```cpp
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    // Pure virtual interface
    virtual void visit(EraDefinition& node) = 0;
    virtual void visit(StructureDefinition& node) = 0;
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    std::string location;
};
```

### Comment Style
```cpp
// Use // for single-line comments
void skip_whitespace() {
    // Skip spaces, tabs, newlines
    while (is_whitespace(current())) {
        advance();
    }
}

/* Use /* */ for block comments (rarely needed)
   This is a block comment
*/
```

### Compiler Warnings
```bash
# MSVC: /W4 (high warning level)
# GCC/Clang: -Wall -Wextra -pedantic
# All warnings should be addressed before committing
```

---

## Testing Guidelines

### Test Framework (To Be Implemented)
The codebase currently has `BUILD_TESTS=OFF` and no test files. When adding tests:
- Recommend GoogleTest (gtest) or Catch2
- Place test files in `tests/` directory
- Use descriptive test names: `Lexer.IdentifierKeyword`, `Parser.StructureDefinition`

### Single Test Execution (Example)
```bash
# GoogleTest format
./build/tests/lexer_tests --gtest_filter=Lexer.IdentifierKeyword

# Catch2 format
./build/tests/lexer_tests "Lexer.IdentifierKeyword"

# CTest format
ctest --test-dir build -R "Lexer.Identifier"
```

---

## Architecture Principles

1. **Separation of Concerns**: Lexer, Parser, AST, CodeGen are independent
2. **Visitor Pattern**: AST traversal uses visitors for flexibility
3. **Extensibility**: New backends via BackendRegistry
4. **Error Recovery**: Parser continues after errors for better reporting
5. **RAII**: Use smart pointers (`std::unique_ptr`) for ownership
6. **English Keywords Only**: All language keywords in English

---

## Common Patterns

### Return unique_ptr for heap-allocated objects
```cpp
std::unique_ptr<Definition> parse_definition();
std::unique_ptr<Expression> parse_expression();
```

### Use map for keyword lookups
```cpp
inline const std::map<std::string, TokenType> KEYWORDS = {
    {"era", TokenType::ERA},
    {"structure", TokenType::STRUCTURE}
};
```

### Forward references allowed (resolved in validation pass)
```cpp
// Parser accepts any identifier
// Validator checks existence in second pass
```

---

## Quick Reference

| Aspect | Style |
|--------|-------|
| Include guard | `#pragma once` |
| Namespace | `namespace lex { }` |
| Class names | `PascalCase` |
| Method names | `snake_case` |
| Member vars | `snake_case_` |
| Const correct | Always use `const` where applicable |
| Ownership | `std::unique_ptr<T>` |
| Collections | `std::vector<T>`, `std::map<K,V>` |
| Strings | `std::string` (never `char*`) |
| Integer types | `int64_t` for large, `int` otherwise |
| Virtual base | `virtual ~Type() = default;` |
| Single-arg ctor | `explicit` required |
| Errors | `std::vector<std::string> errors_` |

---

## Regole per lo Sviluppo con Agenti AI

Lavorare con agenti AI su un compilatore ha insidie specifiche.

### Regola 1 — Un problema alla volta
Dare all'agente obiettivi piccoli e verificabili. Non "implementa l'expression parser" ma "implementa il parsing di `a > b` con il test corrispondente."

### Regola 2 — Test prima di procedere
Ogni modifica deve compilare e passare i test esistenti. Gli agenti tendono ad accumulare debito — funziona la feature nuova ma si rompe quella vecchia.

### Regola 3 — Contesto obbligatorio
Prima di ogni sessione, l'agente deve leggere `AGENTS.md` e documentazione rilevante. Senza contesto genera codice plausibile ma non coerente con l'architettura esistente.

### Regola 4 — Criteri di successo espliciti
Ogni task deve avere un criterio verificabile. Esempio: "dopo questa modifica, `lexc examples/basic_structure.lex -t lua` deve girare senza errori e produrre output valido."

### Regola 5 — Non documentare il futuro
Aggiornare la documentazione solo dopo che il codice funziona, non prima.

---

**Version:** 0.3.0
**Last Updated:** 2026-03-02
