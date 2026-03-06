#include "lex.hpp"
#include "version.hpp"
#include "compiler/pipeline.hpp"
#include "schema/schema.hpp"
#include "context/context.hpp"
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/validator.hpp"
#include "codegen/backend.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <unordered_set>
#include <functional>

namespace lex {

// ============================================================================
// Version Info
// ============================================================================

std::string version() {
    return LEX_VERSION;
}

// ============================================================================
// Utility Functions
// ============================================================================

std::vector<std::string> available_targets() {
    return {"lua", "json", "gd", "cs"};
}

std::vector<std::string> default_types() {
    return {"era", "structure", "unit", "technology", "resource",
            "choice", "ending", "event", "secret"};
}
std::string target_extension(Target target) {
    switch (target) {
        case Target::Lua: return ".lua";
        case Target::JSON: return ".json";
        case Target::Godot: return ".gd";
        case Target::Unity: return ".cs";
        case Target::Love2D: return ".lua";
        case Target::Defold: return ".lua";
        case Target::TypeScript: return ".ts";
        default: return "";
    }
}
std::string target_extension(const std::string& target_name) {
    if (target_name == "lua") return ".lua";
    if (target_name == "json") return ".json";
    if (target_name == "gd" || target_name == "godot") return ".gd";
    if (target_name == "cs" || target_name == "unity" || target_name == "csharp") return ".cs";
    if (target_name == "love2d" || target_name == "love") return ".lua";
    if (target_name == "defold") return ".lua";
    if (target_name == "ts" || target_name == "typescript") return ".ts";
    return "";
}

// ============================================================================
// Main Compile Function (uses Pipeline)
// ============================================================================

CompileResult compile(const std::string& source, const CompileOptions& options) {
    // Initialize schema registry
    auto& schema = SchemaRegistry::instance();
    if (options.types.empty()) {
        schema.load_imperium_default();
    } else {
        std::string types_str;
        for (size_t i = 0; i < options.types.size(); i++) {
            if (i > 0) types_str += ",";
            types_str += options.types[i];
        }
        schema.load_from_cli(types_str);
    }

    // Create context and pipeline
    PassContext ctx;
    ctx.source = source;
    ctx.source_name = options.source_name;
    ctx.options = options;
    
    auto pipeline = CompilerPipeline::create_default(options);
    return pipeline.run(ctx);
}
// ============================================================================
// Compile from File (single file)
// ============================================================================
CompileResult compile_file(const std::string& filepath, const CompileOptions& options) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        CompileResult result;
        result.success = false;
        result.errors.push_back({
            .message = "Cannot open file: " + filepath,
            .location = "",
            .severity = CompileErrorSeverity::Error,
            .code = "F001",
            .suggestion = "Check that the file exists and is readable"
        });
        return result;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    auto new_options = options;
    new_options.source_name = filepath;

    return compile(buffer.str(), new_options);
}

// ============================================================================
// Helper functions for legacy compile_modules
// ============================================================================

namespace {

std::vector<CompileError> convert_parser_errors(const std::vector<std::string>& errors) {
    std::vector<CompileError> result;
    for (const auto& err : errors) {
        result.push_back({
            .message = err,
            .location = "",
            .severity = CompileErrorSeverity::Error,
            .code = "P001",
            .suggestion = ""
        });
    }
    return result;
}

std::vector<CompileError> convert_validation_errors(const std::vector<SemanticError>& errors) {
    std::vector<CompileError> result;
    for (const auto& err : errors) {
        result.push_back({
            .message = err.message,
            .location = err.location,
            .severity = err.severity == ErrorSeverity::ERROR 
                ? CompileErrorSeverity::Error 
                : CompileErrorSeverity::Warning,
            .code = err.code,
            .suggestion = ""
        });
    }
    return result;
}

} // anonymous namespace

// ============================================================================
// Multi-File Compilation with Module System
// ============================================================================

namespace {

// Helper to resolve file path relative to importing file
std::string resolve_import_path(const std::string& import_path, const std::string& base_dir) {
    namespace fs = std::filesystem;

    // Try relative to base directory first
    fs::path resolved = fs::path(base_dir) / import_path;
    if (fs::exists(resolved)) {
        return fs::canonical(resolved).string();
    }

    // Try as absolute path
    if (fs::exists(import_path)) {
        return fs::canonical(import_path).string();
    }

    return "";
}

// Load and parse a single file, returning ASTFile
std::unique_ptr<ASTFile> load_single_file(
    const std::string& filepath,
    std::vector<CompileError>& errors,
    const SchemaRegistry* schema = nullptr
) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        errors.push_back({
            .message = "Cannot open file: " + filepath,
            .location = "",
            .severity = CompileErrorSeverity::Error,
            .code = "F001",
            .suggestion = "Check that the file exists and is readable"
        });
        return nullptr;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Lexing
    Lexer lexer(source, filepath);
    auto tokens = lexer.tokenize();
    if (lexer.has_errors()) {
        for (const auto& err : lexer.errors()) {
            errors.push_back({
                .message = err,
                .location = "",
                .severity = CompileErrorSeverity::Error,
                .code = "L001",
                .suggestion = ""
            });
        }
        return nullptr;
    }

    // Parse as full file (with modules)
    Parser parser(tokens, schema);
    auto ast_file = std::make_unique<ASTFile>(parser.parse_file());
    if (parser.has_errors()) {
        for (const auto& err : parser.errors()) {
            errors.push_back({
                .message = err,
                .location = "",
                .severity = CompileErrorSeverity::Error,
                .code = "P001",
                .suggestion = ""
            });
        }
        return nullptr;
    }

    return ast_file;
}

} // anonymous namespace

CompileResult compile_modules(const std::string& entry_file, const CompileOptions& options) {
    CompileResult result;
    namespace fs = std::filesystem;

    // Initialize schema
    auto& schema = SchemaRegistry::instance();
    if (options.types.empty()) {
        schema.load_imperium_default();
    } else {
        std::string types_str;
        for (size_t i = 0; i < options.types.size(); i++) {
            if (i > 0) types_str += ",";
            types_str += options.types[i];
        }
        schema.load_from_cli(types_str);
    }
    // Track loaded files to avoid cycles
    std::unordered_set<std::string> loaded_files;
    std::vector<std::unique_ptr<ASTFile>> all_files;

    // Load entry file and all imports recursively
    std::function<void(const std::string&)> load_recursive = [&](const std::string& filepath) {
        // Skip if already loaded
        std::string canonical_path;
        try {
            canonical_path = fs::canonical(filepath).string();
        } catch (const std::exception& e) {
            // Log warning if verbose mode is enabled
            if (options.verbose) {
                result.warnings.push_back({
                    .message = "Could not resolve canonical path for '" + filepath + "': " + e.what(),
                    .location = filepath,
                    .severity = CompileErrorSeverity::Warning,
                    .code = "W002",
                    .suggestion = "Using original path instead"
                });
            }
            canonical_path = filepath;
        } catch (...) {
            // Unknown exception - use original path
            canonical_path = filepath;
        }

        if (loaded_files.count(canonical_path) > 0) {
            return;
        }
        loaded_files.insert(canonical_path);

        // Load the file
        auto ast_file = load_single_file(filepath, result.errors, &schema);
        if (!ast_file) {
            return;
        }

        // Process imports
        std::string file_dir = fs::path(filepath).parent_path().string();
        for (const auto& import_stmt : ast_file->imports) {
            std::string resolved = resolve_import_path(import_stmt->path, file_dir);
            if (resolved.empty()) {
                result.errors.push_back({
                    .message = "Cannot find imported file: " + import_stmt->path,
                    .location = filepath,
                    .severity = CompileErrorSeverity::Error,
                    .code = "M001",
                    .suggestion = "Check the import path is correct"
                });
                continue;
            }
            load_recursive(resolved);
        }

        all_files.push_back(std::move(ast_file));
    };
    load_recursive(entry_file);
    // Check for errors during loading
    if (result.has_errors()) {
        result.success = false;
        return result;
    }
    // Merge all definitions into a single list, filtering by visibility
    std::vector<std::unique_ptr<Definition>> merged_definitions;

    for (const auto& file : all_files) {
        for (const auto& def : file->definitions) {
            // If modder mode, skip internal/private definitions
            if (!options.allow_internal) {
                if (def->visibility == Visibility::INTERNAL ||
                    def->visibility == Visibility::PRIVATE) {
                    if (options.verbose) {
                        result.warnings.push_back({
                            .message = "Skipped internal definition: " + def->identifier,
                            .location = file->module_name,
                            .severity = CompileErrorSeverity::Info,
                            .code = "M002",
                            .suggestion = ""
                        });
                    }
                    continue;
                }
            }
            // Clone definition for merged list
            auto cloned = std::make_unique<Definition>();
            cloned->definition_type = def->definition_type;
            cloned->identifier = def->identifier;
            cloned->visibility = def->visibility;
            for (const auto& prop : def->properties) {
                cloned->properties.push_back(prop->clone());
            }
            merged_definitions.push_back(std::move(cloned));
        }
    }
    // Validate merged definitions
    if (options.validate) {
        Validator validator(&schema);
        validator.validate(merged_definitions);
        if (validator.has_warnings()) {
            auto warns = convert_validation_errors(validator.warnings());
            result.warnings.insert(result.warnings.end(), warns.begin(), warns.end());
        }
        if (validator.has_errors()) {
            auto errs = convert_validation_errors(validator.errors());
            result.errors.insert(result.errors.end(), errs.begin(), errs.end());
            result.success = false;
            return result;
        }
    }
    // Generate code using BackendRegistry
    auto& registry = BackendRegistry::instance();
    for (const auto& target : options.targets) {
        std::string target_name;
        switch (target) {
            case Target::Lua: target_name = "lua"; break;
            case Target::JSON: target_name = "json"; break;
            case Target::Godot: target_name = "godot"; break;
            case Target::Unity: target_name = "unity"; break;
            case Target::Love2D: target_name = "love2d"; break;
            case Target::Defold: target_name = "defold"; break;
            case Target::TypeScript: target_name = "typescript"; break;
            default: continue;
        }
        
        auto backend = registry.create(target_name);
        if (!backend) {
            result.warnings.push_back({
                .message = "Unknown target: " + target_name,
                .location = "",
                .severity = CompileErrorSeverity::Warning,
                .code = "W001",
                .suggestion = ""
            });
            continue;
        }
        
        backend->configure(options.source_name);
        result.outputs[target_name] = backend->generate(merged_definitions);
    }
    result.success = true;
    return result;
}
// ============================================================================
// Validate Only
// ============================================================================

CompileResult validate(const std::string& source, const CompileOptions& options) {
    auto validate_options = options;
    validate_options.targets = {};  // No code generation
    return compile(source, validate_options);
}

// ============================================================================
// AI Context Generation
// ============================================================================

ContextResult generate_context_from_source(
    const std::string& source,
    const ContextOptions& options
) {
    // Initialize schema
    auto& schema = SchemaRegistry::instance();
    schema.load_imperium_default();

    // Lexing
    Lexer lexer(source, "context_source");
    auto tokens = lexer.tokenize();
    if (lexer.has_errors()) {
        ContextResult result;
        result.success = false;
        result.error = lexer.errors().empty() ? "Lexer error" : lexer.errors()[0];
        return result;
    }

    // Parsing
    Parser parser(tokens, &schema);
    auto ast = parser.parse();
    if (parser.has_errors()) {
        ContextResult result;
        result.success = false;
        result.error = parser.errors().empty() ? "Parser error" : parser.errors()[0];
        return result;
    }

    // Generate context
    return generate_context(ast, options);
}

ContextResult generate_context_from_file(
    const std::string& filepath,
    const ContextOptions& options
) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        ContextResult result;
        result.success = false;
        result.error = "Cannot open file: " + filepath;
        return result;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    
    return generate_context_from_source(buffer.str(), options);
}

// ============================================================================
// Query API
// ============================================================================

QueryResult query_context(
    const std::string& query_str,
    const ContextResult& context
) {
    return query(query_str, context);
}

QueryResult query_file(
    const std::string& filepath,
    const std::string& query_str,
    const ContextOptions& options
) {
    auto context = generate_context_from_file(filepath, options);
    if (!context.success) {
        QueryResult result;
        result.success = false;
        result.error = context.error;
        return result;
    }
    
    return query_context(query_str, context);
}

} // namespace lex
