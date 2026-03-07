#pragma once

#include <string>
#include <memory>
#include "../ast/ast.hpp"
#include "../lexer/token.hpp"
#include "../lex/lex.hpp"

namespace lex {

// ============================================================================
// Pass Context - Shared state during compilation
// ============================================================================

struct PassContext {
    // Input
    std::string source;
    std::string source_name;
    CompileOptions options;
    
    // Intermediate state
    std::vector<Token> tokens;
    std::vector<std::unique_ptr<Definition>> ast;
    std::shared_ptr<SchemaRegistry> schema;
    
    // Accumulated output
    std::map<std::string, std::string> outputs;
    
    // Errors/Warnings
    std::vector<CompileError> errors;
    std::vector<CompileError> warnings;
    
    bool has_errors() const { return !errors.empty(); }
};

// ============================================================================
// Pass Interface - Single compilation phase
// ============================================================================

class Pass {
public:
    virtual ~Pass() = default;
    
    // Pass name for logging/debugging
    virtual std::string name() const = 0;
    
    // Execute the pass, modifying context
    // Returns true on success, false on error
    virtual bool run(PassContext& ctx) = 0;
    
    // Can this pass be skipped? (e.g., validation disabled)
    virtual bool is_optional() const { return false; }
};

// ============================================================================
// Pass Types
// ============================================================================

// Lexer pass: source -> tokens
class LexerPass : public Pass {
public:
    std::string name() const override { return "lexer"; }
    bool run(PassContext& ctx) override;
};

// Parser pass: tokens -> AST
class ParserPass : public Pass {
public:
    std::string name() const override { return "parser"; }
    bool run(PassContext& ctx) override;
};

// Validation pass: semantic checks on AST
class ValidationPass : public Pass {
public:
    std::string name() const override { return "validation"; }
    bool run(PassContext& ctx) override;
    bool is_optional() const override { return true; }
};

// Type checking pass: validate types in expressions
class TypeCheckPass : public Pass {
public:
    std::string name() const override { return "typecheck"; }
    bool run(PassContext& ctx) override;
    bool is_optional() const override { return true; }
};

// Visibility filter: remove internal/private defs for modders
class VisibilityPass : public Pass {
public:
    std::string name() const override { return "visibility"; }
    bool run(PassContext& ctx) override;
    bool is_optional() const override { return true; }
};

// Codegen pass: AST -> target output
class CodegenPass : public Pass {
public:
    std::string name() const override { return "codegen"; }
    bool run(PassContext& ctx) override;

private:
    static std::string target_to_string(Target t);
};

} // namespace lex
