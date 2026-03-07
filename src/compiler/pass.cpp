#include "pass.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "../parser/validator.hpp"
#include "../semantic/type_checker.hpp"
#include "../codegen/backend.hpp"

namespace lex {

// ============================================================================
// LexerPass Implementation
// ============================================================================

bool LexerPass::run(PassContext& ctx) {
    Lexer lexer(ctx.source, ctx.source_name);
    ctx.tokens = lexer.tokenize();
    
    if (lexer.has_errors()) {
        for (const auto& err : lexer.errors()) {
            ctx.errors.push_back({
                .message = err,
                .location = ctx.source_name,
                .severity = CompileErrorSeverity::Error,
                .code = "L001",
                .suggestion = ""
            });
        }
        return false;
    }
    return true;
}

// ============================================================================
// ParserPass Implementation
// ============================================================================

bool ParserPass::run(PassContext& ctx) {
    Parser parser(ctx.tokens);
    ctx.ast = parser.parse();
    
    if (parser.has_errors()) {
        for (const auto& err : parser.errors()) {
            ctx.errors.push_back({
                .message = err,
                .location = ctx.source_name,
                .severity = CompileErrorSeverity::Error,
                .code = "P001",
                .suggestion = ""
            });
        }
        return false;
    }
    return true;
}

// ============================================================================
// ValidationPass Implementation
// ============================================================================

bool ValidationPass::run(PassContext& ctx) {
    if (!ctx.options.validate) {
        return true;  // Skip if validation disabled
    }
    
    Validator validator;
    if (!validator.validate(ctx.ast)) {
        for (const auto& err : validator.errors()) {
            ctx.errors.push_back({
                .message = err.message,
                .location = err.location,
                .severity = CompileErrorSeverity::Error,
                .code = err.code,
                .suggestion = ""
            });
        }
        for (const auto& warn : validator.warnings()) {
            ctx.warnings.push_back({
                .message = warn.message,
                .location = warn.location,
                .severity = CompileErrorSeverity::Warning,
                .code = warn.code,
                .suggestion = ""
            });
        }
        return false;
    }
    return true;
}

// ============================================================================
// TypeCheckPass Implementation
// ============================================================================

bool TypeCheckPass::run(PassContext& ctx) {
    if (!ctx.options.validate) {
        return true;  // Skip if validation disabled
    }
    
    TypeChecker checker(ctx.schema.get());
    
    if (!checker.check(ctx.ast)) {
        for (const auto& err : checker.errors()) {
            ctx.errors.push_back({
                .message = err.message,
                .location = err.location.empty() ? ctx.source_name : err.location,
                .severity = CompileErrorSeverity::Error,
                .code = "T001",
                .suggestion = err.context.empty() ? "" : "In " + err.context
            });
        }
        return false;
    }
    return true;
}

// ============================================================================
// VisibilityPass Implementation
// ============================================================================

bool VisibilityPass::run(PassContext& ctx) {
    // Skip if internal definitions allowed (default mode)
    if (ctx.options.allow_internal) {
        return true;
    }
    
    // Remove internal/private definitions for modder mode
    auto it = ctx.ast.begin();
    while (it != ctx.ast.end()) {
        if ((*it)->visibility == Visibility::INTERNAL ||
            (*it)->visibility == Visibility::PRIVATE) {
            if (ctx.options.verbose) {
                std::string vis_str = (*it)->visibility == Visibility::INTERNAL 
                    ? "internal" : "private";
                ctx.warnings.push_back({
                    .message = "Skipped " + vis_str + " definition: " + (*it)->identifier,
                    .location = ctx.source_name,
                    .severity = CompileErrorSeverity::Info,
                    .code = "M002",
                    .suggestion = ""
                });
            }
            it = ctx.ast.erase(it);
        } else {
            ++it;
        }
    }
    return true;
}

// ============================================================================
// CodegenPass Implementation
// ============================================================================

bool CodegenPass::run(PassContext& ctx) {
    auto& registry = BackendRegistry::instance();
    
    // Process standard targets
    for (const auto& target : ctx.options.targets) {
        std::string target_name = target_to_string(target);
        auto backend = registry.create(target_name);
        
        if (!backend) {
            ctx.warnings.push_back({
                .message = "Unknown target: " + target_name,
                .location = "",
                .severity = CompileErrorSeverity::Warning,
                .code = "W001",
                .suggestion = ""
            });
            continue;
        }
        
        // Configure backend with source name (for Godot class name, etc.)
        backend->configure(ctx.options.source_name);
        
        std::string output = backend->generate(ctx.ast);
        ctx.outputs[target_name] = output;
    }
    
    // Process custom targets (by name)
    for (const auto& target_name : ctx.options.custom_targets) {
        auto backend = registry.create(target_name);
        
        if (!backend) {
            ctx.warnings.push_back({
                .message = "Unknown custom target: " + target_name,
                .location = "",
                .severity = CompileErrorSeverity::Warning,
                .code = "W002",
                .suggestion = ""
            });
            continue;
        }
        
        backend->configure(ctx.options.source_name);
        std::string output = backend->generate(ctx.ast);
        ctx.outputs[target_name] = output;
    }
    
    return true;
}

// Helper
std::string CodegenPass::target_to_string(Target t) {
    switch (t) {
        case Target::Lua: return "lua";
        case Target::JSON: return "json";
        case Target::Godot: return "godot";
        case Target::Unity: return "unity";
        case Target::Love2D: return "love2d";
        case Target::Defold: return "defold";
        case Target::TypeScript: return "typescript";
        default: return "";
    }
}

} // namespace lex
