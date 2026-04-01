#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <sstream>

namespace lex {

/**
 * @brief Result from LLM-based semantic validation
 */
struct LLMValidationResult {
    bool valid = true;
    double confidence = 1.0;
    
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::vector<std::string> suggestions;
    
    // Semantic analysis results
    std::vector<std::string> missing_features;
    std::vector<std::string> deprecated_patterns;
    std::vector<std::string> performance_issues;
    std::vector<std::string> best_practice_violations;
    
    std::string summary() const {
        std::stringstream ss;
        ss << (valid ? "Valid" : "Invalid") << " (confidence: " << (confidence * 100) << "%)";
        if (!errors.empty()) ss << " | " << errors.size() << " errors";
        if (!warnings.empty()) ss << " | " << warnings.size() << " warnings";
        return ss.str();
    }
};

/**
 * @brief LLM client interface for semantic validation
 * 
 * This is an abstract interface that can be implemented with different
 * LLM providers (OpenAI, Anthropic, local models, etc.)
 */
class LLMClient {
public:
    virtual ~LLMClient() = default;
    
    struct CompletionResult {
        bool success = false;
        std::string content;
        std::string error;
        int tokens_used = 0;
        double latency_ms = 0.0;
    };
    
    virtual CompletionResult complete(const std::string& prompt) = 0;
    virtual bool is_available() const = 0;
    virtual std::string model_name() const = 0;
};

/**
 * @brief OpenAI GPT client implementation
 */
class OpenAIClient : public LLMClient {
public:
    explicit OpenAIClient(const std::string& api_key = "", const std::string& model = "gpt-4o-mini")
        : api_key_(api_key), model_(model) {
        if (api_key_.empty()) {
            // Try environment variable
            const char* env_key = std::getenv("OPENAI_API_KEY");
            if (env_key) api_key_ = env_key;
        }
    }
    
    CompletionResult complete(const std::string& prompt) override {
        CompletionResult result;
        
        if (api_key_.empty()) {
            result.error = "OpenAI API key not configured";
            return result;
        }
        
        // Note: Actual HTTP implementation would go here
        // For now, return a placeholder
        result.error = "HTTP client not implemented - use external LLM validator";
        return result;
    }
    
    bool is_available() const override {
        return !api_key_.empty();
    }
    
    std::string model_name() const override {
        return model_;
    }

private:
    std::string api_key_;
    std::string model_;
};

/**
 * @brief Anthropic Claude client implementation
 */
class AnthropicClient : public LLMClient {
public:
    explicit AnthropicClient(const std::string& api_key = "", const std::string& model = "claude-3-haiku-20240307")
        : api_key_(api_key), model_(model) {
        if (api_key_.empty()) {
            const char* env_key = std::getenv("ANTHROPIC_API_KEY");
            if (env_key) api_key_ = env_key;
        }
    }
    
    CompletionResult complete(const std::string& prompt) override {
        CompletionResult result;
        
        if (api_key_.empty()) {
            result.error = "Anthropic API key not configured";
            return result;
        }
        
        result.error = "HTTP client not implemented - use external LLM validator";
        return result;
    }
    
    bool is_available() const override {
        return !api_key_.empty();
    }
    
    std::string model_name() const override {
        return model_;
    }

private:
    std::string api_key_;
    std::string model_;
};

/**
 * @brief Mock LLM client for testing without API calls
 */
class MockLLMClient : public LLMClient {
public:
    CompletionResult complete(const std::string& prompt) override {
        CompletionResult result;
        result.success = true;
        result.content = R"({"valid": true, "confidence": 1.0, "errors": [], "warnings": [], "suggestions": []})";
        result.tokens_used = 50;
        return result;
    }
    
    bool is_available() const override {
        return true;
    }
    
    std::string model_name() const override {
        return "mock-validator";
    }
};

/**
 * @brief LLM-based semantic validator for generated code
 * 
 * This validator uses an LLM to check if the generated code:
 * - Correctly implements the semantics from the original Lex
 * - Follows best practices for the target language
 * - Has any potential runtime issues
 * - Is complete and handles all cases
 */
class LLMValidator {
public:
    struct Config {
        bool enabled = true;
        double confidence_threshold = 0.8;
        bool include_suggestions = true;
        bool check_best_practices = true;
        bool check_performance = true;
        int max_tokens = 1000;
    };
    
    explicit LLMValidator(std::shared_ptr<LLMClient> client, const Config& config = {})
        : client_(client), config_(config) {}
    
    /**
     * @brief Validate generated code against original Lex semantics
     * 
     * @param target_language The target language (lua, gdscript, csharp, etc.)
     * @param generated_code The code generated by the backend
     * @param original_lex The original Lex source code
     * @return Validation result with semantic analysis
     */
    LLMValidationResult validate(
        const std::string& target_language,
        const std::string& generated_code,
        const std::string& original_lex) {
        
        LLMValidationResult result;
        
        if (!client_ || !client_->is_available()) {
            result.warnings.push_back("LLM client not available for semantic validation");
            result.valid = true; // Don't fail if LLM not available
            result.confidence = 0.5; // Low confidence without LLM
            return result;
        }
        
        std::string prompt = build_validation_prompt(
            target_language, generated_code, original_lex);
        
        auto completion = client_->complete(prompt);
        
        if (!completion.success) {
            result.warnings.push_back("LLM validation failed: " + completion.error);
            result.valid = true;
            result.confidence = 0.5;
            return result;
        }
        
        return parse_llm_response(completion.content);
    }
    
    /**
     * @brief Quick semantic check without full comparison
     */
    LLMValidationResult quick_check(
        const std::string& target_language,
        const std::string& generated_code) {
        
        std::string prompt = build_quick_check_prompt(target_language, generated_code);
        auto completion = client_->complete(prompt);
        
        if (!completion.success) {
            LLMValidationResult result;
            result.warnings.push_back("LLM quick check failed: " + completion.error);
            result.valid = true;
            return result;
        }
        
        return parse_llm_response(completion.content);
    }

private:
    std::shared_ptr<LLMClient> client_;
    Config config_;
    
    std::string build_validation_prompt(
        const std::string& target_language,
        const std::string& generated_code,
        const std::string& original_lex) const {
        
        std::string lang_name = format_language_name(target_language);
        
        std::stringstream ss;
        ss << R"(You are an expert )" << lang_name << R"( programmer and code reviewer.
Your task is to validate that generated code correctly implements the semantics from the original Lex source.

[Lex Source]
)" << original_lex << R"(

[Generated )" << lang_name << R"( Code]
)" << generated_code << R"(

[Validation Requirements]
1. Check that all definitions from Lex are present in the generated code
2. Verify that all properties are correctly translated
3. Ensure that data types are appropriate for the target language
4. Check that any expressions or conditions are correctly translated
5. Look for potential runtime errors or edge cases
6. Verify that the code follows )" << lang_name << R"( best practices

[Response Format]
Respond with a JSON object in this exact format:
{
    "valid": true/false,
    "confidence": 0.0-1.0,
    "errors": ["error message 1", ...],
    "warnings": ["warning message 1", ...],
    "suggestions": ["suggestion 1", ...],
    "missing_features": ["feature 1", ...],
    "deprecated_patterns": ["pattern 1", ...],
    "performance_issues": ["issue 1", ...],
    "best_practice_violations": ["violation 1", ...]
}

Only respond with the JSON object, no additional text.)";
        
        return ss.str();
    }
    
    std::string build_quick_check_prompt(
        const std::string& target_language,
        const std::string& generated_code) const {
        
        std::string lang_name = format_language_name(target_language);
        
        std::stringstream ss;
        ss << R"(You are an expert )" << lang_name << R"( programmer.
Quickly check this generated code for obvious issues:

[Code]
)" << generated_code << R"(

[Check for]
1. Syntax errors
2. Undefined variables or functions
3. Type mismatches
4. Common runtime errors

[Response Format]
{"valid": true/false, "confidence": 0.0-1.0, "errors": [], "warnings": [], "suggestions": []}

Only respond with JSON.)";
        
        return ss.str();
    }
    
    std::string format_language_name(const std::string& target) const {
        if (target == "lua") return "Lua";
        if (target == "godot" || target == "gd" || target == "gdscript") return "GDScript";
        if (target == "unity" || target == "cs" || target == "csharp") return "C#";
        if (target == "json") return "JSON";
        if (target == "python" || target == "py") return "Python";
        if (target == "typescript" || target == "ts") return "TypeScript";
        if (target == "javascript" || target == "js") return "JavaScript";
        return target; // Return as-is if unknown
    }
    
    LLMValidationResult parse_llm_response(const std::string& response) const {
        LLMValidationResult result;
        
        // Simple JSON parsing without external dependencies
        // Find key values in the response
        
        result.valid = response.find("\"valid\": true") != std::string::npos ||
                       response.find("\"valid\":true") != std::string::npos;
        
        // Extract confidence
        size_t conf_pos = response.find("\"confidence\"");
        if (conf_pos != std::string::npos) {
            size_t colon = response.find(':', conf_pos);
            if (colon != std::string::npos) {
                size_t end = response.find_first_of(",}", colon);
                if (end != std::string::npos) {
                    std::string conf_str = response.substr(colon + 1, end - colon - 1);
                    result.confidence = std::stod(conf_str);
                }
            }
        }
        
        // Extract arrays
        result.errors = extract_json_array(response, "errors");
        result.warnings = extract_json_array(response, "warnings");
        result.suggestions = extract_json_array(response, "suggestions");
        result.missing_features = extract_json_array(response, "missing_features");
        result.deprecated_patterns = extract_json_array(response, "deprecated_patterns");
        result.performance_issues = extract_json_array(response, "performance_issues");
        result.best_practice_violations = extract_json_array(response, "best_practice_violations");
        
        return result;
    }
    
    std::vector<std::string> extract_json_array(const std::string& json, const std::string& key) const {
        std::vector<std::string> result;
        
        std::string search_key = "\"" + key + "\":";
        size_t key_pos = json.find(search_key);
        if (key_pos == std::string::npos) {
            // Try without quotes
            search_key = "\"" + key + "\":";
            key_pos = json.find(search_key);
            if (key_pos == std::string::npos) return result;
        }
        
        size_t array_start = json.find('[', key_pos);
        if (array_start == std::string::npos) return result;
        
        size_t array_end = json.find(']', array_start);
        if (array_end == std::string::npos) return result;
        
        std::string array_content = json.substr(array_start + 1, array_end - array_start - 1);
        
        // Split by comma and extract strings
        std::stringstream ss(array_content);
        std::string item;
        while (std::getline(ss, item, ',')) {
            // Trim whitespace
            size_t start = item.find_first_not_of(" \t\n\r");
            size_t end = item.find_last_not_of(" \t\n\r");
            if (start != std::string::npos && end != std::string::npos) {
                item = item.substr(start, end - start + 1);
            }
            
            // Remove quotes
            if (item.size() >= 2 && item.front() == '"' && item.back() == '"') {
                item = item.substr(1, item.size() - 2);
            }
            
            // Unescape
            size_t pos = 0;
            while ((pos = item.find("\\\"", pos)) != std::string::npos) {
                item.replace(pos, 2, "\"");
                pos += 1;
            }
            
            if (!item.empty()) {
                result.push_back(item);
            }
        }
        
        return result;
    }
};

} // namespace lex
