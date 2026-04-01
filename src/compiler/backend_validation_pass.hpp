#pragma once

#include "pass.hpp"
#include "../codegen/backend_validator.hpp"
#include "../codegen/llm_validator.hpp"
#include <memory>
#include <chrono>
#include <set>
#include <iostream>

namespace lex {

/**
 * @brief Configuration for backend validation pass
 */
struct BackendValidationConfig {
    // Tool-based validation
    bool use_tool_validation = true;
    int tool_timeout_ms = 5000;
    
    // LLM-based semantic validation
    bool use_llm_validation = false;
    double llm_confidence_threshold = 0.8;
    bool llm_include_suggestions = true;
    
    // General options
    bool fail_on_errors = true;
    bool fail_on_warnings = false;
    bool verbose = false;
    
    // Which backends to validate
    std::set<std::string> enabled_backends; // Empty = all
    
    // LLM client configuration
    std::string llm_provider = "openai"; // openai, anthropic, mock
    std::string llm_model;
    std::string llm_api_key;
};

/**
 * @brief Backend validation pass - validates generated code
 * 
 * This pass runs after code generation and validates the output
 * using both external tools and optional LLM-based semantic checking.
 */
class BackendValidationPass : public Pass {
public:
    explicit BackendValidationPass(const BackendValidationConfig& config = {})
        : config_(config) {}
    
    std::string name() const override {
        return "backend_validation";
    }
    
    bool run(PassContext& ctx) override {
        if (ctx.outputs.empty()) {
            if (config_.verbose) {
                std::cout << "[backend_validation] No outputs to validate\n";
            }
            return true;
        }
        
        bool all_valid = true;
        
        for (const auto& [target, code] : ctx.outputs) {
            // Skip if backend not in enabled set
            if (!config_.enabled_backends.empty() &&
                config_.enabled_backends.find(target) == config_.enabled_backends.end()) {
                continue;
            }
            
            if (config_.verbose) {
                std::cout << "[backend_validation] Validating " << target << "...\n";
            }
            
            // Tool-based validation
            if (config_.use_tool_validation) {
                BackendValidator validator;
                
                auto result = validator.validate(target, code);
                
                if (!result.valid) {
                    all_valid = false;
                    
                    for (const auto& err : result.errors) {
                        ctx.errors.push_back(CompileError{
                            .message = "Backend validation failed: " + err,
                            .location = ctx.source_name,
                            .severity = CompileErrorSeverity::Error,
                            .code = "E901"
                        });
                    }
                    
                    if (config_.fail_on_errors) {
                        return false;
                    }
                }
                
                if (!result.warnings.empty() && config_.verbose) {
                    for (const auto& warn : result.warnings) {
                        ctx.warnings.push_back(CompileError{
                            .message = "Backend validation warning: " + warn,
                            .location = ctx.source_name,
                            .severity = CompileErrorSeverity::Warning,
                            .code = "W901"
                        });
                    }
                }
                
                if (config_.verbose) {
                    std::cout << "[backend_validation] " << target << ": " 
                              << result.summary() << " (" << result.validation_time_ms << "ms)\n";
                }
            }
            
            // LLM-based semantic validation
            if (config_.use_llm_validation) {
                auto llm_result = run_llm_validation(target, code, ctx);
                
                if (!llm_result.valid || llm_result.confidence < config_.llm_confidence_threshold) {
                    if (config_.fail_on_errors) {
                        all_valid = false;
                    }
                    
                    for (const auto& err : llm_result.errors) {
                        ctx.errors.push_back(CompileError{
                            .message = "LLM validation error: " + err,
                            .location = ctx.source_name,
                            .severity = CompileErrorSeverity::Error,
                            .code = "E902"
                        });
                    }
                    
                    for (const auto& warn : llm_result.warnings) {
                        ctx.warnings.push_back(CompileError{
                            .message = "LLM validation warning: " + warn,
                            .location = ctx.source_name,
                            .severity = CompileErrorSeverity::Warning,
                            .code = "W902"
                        });
                    }
                }
                
                if (config_.llm_include_suggestions && !llm_result.suggestions.empty()) {
                    for (const auto& suggestion : llm_result.suggestions) {
                        ctx.warnings.push_back(CompileError{
                            .message = "Suggestion: " + suggestion,
                            .location = ctx.source_name,
                            .severity = CompileErrorSeverity::Info,
                            .code = "I901"
                        });
                    }
                }
            }
        }
        
        return all_valid || !config_.fail_on_errors;
    }
    
    bool is_optional() const override {
        return true;
    }
    
    void set_config(const BackendValidationConfig& config) {
        config_ = config;
    }
    
    const BackendValidationConfig& config() const {
        return config_;
    }

private:
    BackendValidationConfig config_;
    std::shared_ptr<LLMClient> llm_client_;
    
    LLMValidationResult run_llm_validation(
        const std::string& target,
        const std::string& code,
        PassContext& ctx) {
        
        // Initialize LLM client if needed
        if (!llm_client_) {
            llm_client_ = create_llm_client();
        }
        
        if (!llm_client_ || !llm_client_->is_available()) {
            LLMValidationResult result;
            result.warnings.push_back("LLM client not available");
            result.valid = true;
            result.confidence = 0.5;
            return result;
        }
        
        LLMValidator validator(llm_client_, LLMValidator::Config{
            .enabled = true,
            .confidence_threshold = config_.llm_confidence_threshold,
            .include_suggestions = config_.llm_include_suggestions,
            .check_best_practices = true,
            .check_performance = true,
            .max_tokens = 1000
        });
        
        return validator.validate(target, code, ctx.source);
    }
    
    std::shared_ptr<LLMClient> create_llm_client() {
        std::string provider = config_.llm_provider;
        std::string model = config_.llm_model;
        std::string api_key = config_.llm_api_key;
        
        // Check environment variables if not configured
        if (api_key.empty()) {
            const char* env_key = nullptr;
            if (provider == "openai") {
                env_key = std::getenv("OPENAI_API_KEY");
            } else if (provider == "anthropic") {
                env_key = std::getenv("ANTHROPIC_API_KEY");
            }
            if (env_key) api_key = env_key;
        }
        
        if (provider == "openai") {
            return std::make_shared<OpenAIClient>(api_key, model.empty() ? "gpt-4o-mini" : model);
        } else if (provider == "anthropic") {
            return std::make_shared<AnthropicClient>(api_key, model.empty() ? "claude-3-haiku-20240307" : model);
        } else if (provider == "mock") {
            return std::make_shared<MockLLMClient>();
        }
        
        return nullptr;
    }
};

/**
 * @brief Create a backend validation pass with default configuration
 */
inline std::unique_ptr<BackendValidationPass> create_backend_validation_pass(
    const BackendValidationConfig& config = {}) {
    return std::make_unique<BackendValidationPass>(config);
}

} // namespace lex
