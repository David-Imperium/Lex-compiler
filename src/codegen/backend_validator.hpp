#pragma once

#include <string>
#include <vector>
#include <map>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#include <tlhelp32.h>
#else
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#endif

namespace lex {

/**
 * @brief Validation result from backend code validation
 */
struct BackendValidationResult {
    bool valid = true;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::vector<std::string> suggestions;
    
    // Additional metadata
    std::string validator_used;
    double validation_time_ms = 0.0;
    
    bool has_issues() const {
        return !errors.empty() || !warnings.empty();
    }
    
    std::string summary() const {
        std::stringstream ss;
        ss << (valid ? "OK" : "FAILED");
        if (!errors.empty()) ss << " | " << errors.size() << " errors";
        if (!warnings.empty()) ss << " | " << warnings.size() << " warnings";
        return ss.str();
    }
};

/**
 * @brief Temporary file helper for validation
 */
class TempFile {
public:
    TempFile(const std::string& extension, const std::string& content)
        : path_(generate_temp_path(extension)) {
        std::ofstream file(path_);
        file << content;
        file.close();
    }
    
    ~TempFile() {
        std::filesystem::remove(path_);
    }
    
    const std::string& path() const { return path_; }
    
private:
    std::string path_;
    
    static std::string generate_temp_path(const std::string& ext) {
        std::filesystem::path temp_dir = std::filesystem::temp_directory_path();
        std::string filename = "lex_validate_" + std::to_string(std::time(nullptr)) + ext;
        return (temp_dir / filename).string();
    }
};

/**
 * @brief External tool executor
 */
class ToolExecutor {
public:
    struct Result {
        int exit_code = 0;
        std::string stdout_output;
        std::string stderr_output;
        bool timed_out = false;
        double execution_time_ms = 0.0;
    };
    
    static Result execute(const std::string& command, int timeout_ms = 5000) {
        Result result;
        auto start = std::chrono::high_resolution_clock::now();
        
#ifdef _WIN32
        // Windows implementation using pipes
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = nullptr;
        
        HANDLE hReadPipe, hWritePipe;
        CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
        SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);
        
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.hStdError = hWritePipe;
        si.hStdOutput = hWritePipe;
        si.dwFlags |= STARTF_USESTDHANDLES;
        
        // Create mutable command string
        std::string cmd = command;
        
        if (CreateProcessA(
            nullptr,
            const_cast<char*>(cmd.c_str()),
            nullptr,
            nullptr,
            TRUE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi)) {
            
            CloseHandle(hWritePipe);
            
            // Read output with timeout
            std::string output;
            char buffer[4096];
            DWORD bytes_read;
            
            auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
            
            while (std::chrono::steady_clock::now() < deadline) {
                DWORD available = 0;
                PeekNamedPipe(hReadPipe, nullptr, 0, nullptr, &available, nullptr);
                
                if (available == 0) {
                    // Check if process is still running
                    DWORD exit_code;
                    if (GetExitCodeProcess(pi.hProcess, &exit_code) && exit_code != STILL_ACTIVE) {
                        break;
                    }
                    Sleep(10);
                    continue;
                }
                
                if (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytes_read, nullptr) && bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    output += buffer;
                }
            }
            
            // Get exit code
            DWORD win_exit_code;
            GetExitCodeProcess(pi.hProcess, &win_exit_code);
            result.exit_code = static_cast<int>(win_exit_code);
            
            // Check if timed out
            if (std::chrono::steady_clock::now() >= deadline) {
                TerminateProcess(pi.hProcess, 1);
                result.timed_out = true;
            }
            
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            CloseHandle(hReadPipe);
        } else {
            result.exit_code = -1;
            result.stderr_output = "Failed to execute command: " + command;
        }
#else
        // Unix implementation
        int stdout_pipe[2];
        int stderr_pipe[2];
        pipe(stdout_pipe);
        pipe(stderr_pipe);
        
        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process
            close(stdout_pipe[0]);
            close(stderr_pipe[0]);
            dup2(stdout_pipe[1], STDOUT_FILENO);
            dup2(stderr_pipe[1], STDERR_FILENO);
            close(stdout_pipe[1]);
            close(stderr_pipe[1]);
            
            execl("/bin/sh", "sh", "-c", command.c_str(), nullptr);
            _exit(127);
        } else if (pid > 0) {
            // Parent process
            close(stdout_pipe[1]);
            close(stderr_pipe[1]);
            
            // Set non-blocking
            fcntl(stdout_pipe[0], F_SETFL, O_NONBLOCK);
            fcntl(stderr_pipe[0], F_SETFL, O_NONBLOCK);
            
            std::string stdout_output;
            std::string stderr_output;
            char buffer[4096];
            
            auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
            
            while (std::chrono::steady_clock::now() < deadline) {
                bool any_data = false;
                
                // Read stdout
                ssize_t bytes = read(stdout_pipe[0], buffer, sizeof(buffer) - 1);
                if (bytes > 0) {
                    buffer[bytes] = '\0';
                    stdout_output += buffer;
                    any_data = true;
                }
                
                // Read stderr
                bytes = read(stderr_pipe[0], buffer, sizeof(buffer) - 1);
                if (bytes > 0) {
                    buffer[bytes] = '\0';
                    stderr_output += buffer;
                    any_data = true;
                }
                
                if (!any_data) {
                    // Check if child is still running
                    int status;
                    pid_t result_pid = waitpid(pid, &status, WNOHANG);
                    if (result_pid == pid) {
                        // Child exited
                        if (WIFEXITED(status)) {
                            result.exit_code = WEXITSTATUS(status);
                        } else if (WIFSIGNALED(status)) {
                            result.exit_code = -1;
                        }
                        break;
                    }
                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
            
            close(stdout_pipe[0]);
            close(stderr_pipe[0]);
            
            // Check if timed out
            if (std::chrono::steady_clock::now() >= deadline) {
                kill(pid, SIGKILL);
                waitpid(pid, nullptr, 0);
                result.timed_out = true;
            }
            
            result.stdout_output = stdout_output;
            result.stderr_output = stderr_output;
        }
#endif
        
        auto end = std::chrono::high_resolution_clock::now();
        result.execution_time_ms = std::chrono::duration<double, std::milli>(end - start).count();
        
        return result;
    }
    
    static bool is_tool_available(const std::string& tool) {
#ifdef _WIN32
        std::string cmd = "where " + tool + " >nul 2>&1";
#else
        std::string cmd = "which " + tool + " >/dev/null 2>&1";
#endif
        return std::system(cmd.c_str()) == 0;
    }
};

/**
 * @brief Backend code validator using external tools
 */
class BackendValidator {
public:
    // Validation configuration
    struct Config {
        bool use_lua_validator = true;
        bool use_godot_validator = true;
        bool use_csharp_validator = true;
        bool use_json_validator = true;
        bool use_python_validator = true;
        bool use_typescript_validator = true;
        
        int timeout_ms = 5000;
        bool verbose = false;
    };
    
    BackendValidator(const Config& config = {}) : config_(config) {}
    
    /**
     * @brief Validate generated code for a specific target
     * @param target Backend target name (lua, godot, unity, json, etc.)
     * @param code Generated code to validate
     * @return Validation result with errors, warnings, and suggestions
     */
    BackendValidationResult validate(const std::string& target, const std::string& code) {
        BackendValidationResult result;
        result.validator_used = target;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        if (target == "lua") {
            result = validate_lua(code);
        } else if (target == "godot" || target == "gd") {
            result = validate_gdscript(code);
        } else if (target == "unity" || target == "cs" || target == "csharp") {
            result = validate_csharp(code);
        } else if (target == "json") {
            result = validate_json(code);
        } else if (target == "python" || target == "py") {
            result = validate_python(code);
        } else if (target == "typescript" || target == "ts") {
            result = validate_typescript(code);
        } else {
            // Unknown target - just check it's not empty
            result.valid = !code.empty();
            if (code.empty()) {
                result.errors.push_back("Generated code is empty");
            }
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        result.validation_time_ms = std::chrono::duration<double, std::milli>(end - start).count();
        
        return result;
    }
    
    /**
     * @brief Check if a specific validator is available
     */
    bool is_validator_available(const std::string& target) const {
        if (target == "lua") return ToolExecutor::is_tool_available("lua");
        if (target == "godot" || target == "gd") return ToolExecutor::is_tool_available("godot");
        if (target == "unity" || target == "cs") return ToolExecutor::is_tool_available("dotnet");
        if (target == "json") return true; // Built-in
        if (target == "python") return ToolExecutor::is_tool_available("python");
        if (target == "typescript") return ToolExecutor::is_tool_available("tsc") || ToolExecutor::is_tool_available("node");
        return false;
    }
    
    /**
     * @brief Get list of available validators
     */
    std::vector<std::string> available_validators() const {
        std::vector<std::string> result;
        for (const auto& [name, _] : validator_map()) {
            if (is_validator_available(name)) {
                result.push_back(name);
            }
        }
        return result;
    }

private:
    Config config_;
    
    // Validator implementations
    BackendValidationResult validate_lua(const std::string& code) {
        BackendValidationResult result;
        result.validator_used = "lua";
        
        if (code.empty()) {
            result.valid = false;
            result.errors.push_back("Code is empty");
            return result;
        }
        
        if (!ToolExecutor::is_tool_available("lua")) {
            // Lua not available - do basic syntax check
            result.warnings.push_back("Lua interpreter not available for validation");
            result.valid = check_lua_syntax_basic(code);
            if (!result.valid) {
                result.errors.push_back("Basic syntax check failed: unbalanced brackets or braces");
            }
            return result;
        }
        
        // Create temp file and validate with lua -c
        TempFile tmp(".lua", code);
        ToolExecutor::Result exec_result = ToolExecutor::execute("lua -c " + tmp.path(), config_.timeout_ms);
        
        if (exec_result.exit_code == 0) {
            result.valid = true;
        } else {
            result.valid = false;
            result.errors = parse_lua_errors(exec_result.stderr_output);
        }
        
        if (!exec_result.stdout_output.empty()) {
            result.warnings.push_back(exec_result.stdout_output);
        }
        
        return result;
    }
    
    BackendValidationResult validate_gdscript(const std::string& code) {
        BackendValidationResult result;
        result.validator_used = "godot";
        
        if (!ToolExecutor::is_tool_available("godot")) {
            // Godot not available - do basic syntax check
            result.warnings.push_back("Godot not available for GDScript validation");
            result.valid = check_gdscript_syntax_basic(code);
            return result;
        }
        
        // Create temp file and validate with Godot
        TempFile tmp(".gd", code);
        
        // Godot 4 has --script-check option
        std::string cmd = "godot --headless --script " + tmp.path() + " --quit-after 1";
        ToolExecutor::Result exec_result = ToolExecutor::execute(cmd, config_.timeout_ms);
        
        if (exec_result.exit_code == 0) {
            result.valid = true;
        } else {
            result.valid = false;
            result.errors = parse_godot_errors(exec_result.stderr_output);
        }
        
        return result;
    }
    
    BackendValidationResult validate_csharp(const std::string& code) {
        BackendValidationResult result;
        result.validator_used = "csharp";
        
        if (!ToolExecutor::is_tool_available("dotnet")) {
            // dotnet not available - do basic syntax check
            result.warnings.push_back("dotnet not available for C# validation");
            result.valid = check_csharp_syntax_basic(code);
            return result;
        }
        
        // Create a minimal project for compilation
        TempFile cs_file(".cs", code);
        TempFile csproj_file(".csproj", generate_minimal_csproj());
        
        std::string cmd = "dotnet build /nologo /v:minimal " + csproj_file.path();
        ToolExecutor::Result exec_result = ToolExecutor::execute(cmd, config_.timeout_ms);
        
        if (exec_result.exit_code == 0) {
            result.valid = true;
        } else {
            result.valid = false;
            result.errors = parse_csharp_errors(exec_result.stdout_output + "\n" + exec_result.stderr_output);
        }
        
        return result;
    }
    
    BackendValidationResult validate_json(const std::string& code) {
        BackendValidationResult result;
        result.validator_used = "json";
        
        // Try to parse as JSON
        try {
            // Simple JSON validation without external dependency
            result.valid = is_valid_json(code);
            if (!result.valid) {
                result.errors.push_back("Invalid JSON syntax");
            }
        } catch (const std::exception& e) {
            result.valid = false;
            result.errors.push_back(std::string("JSON parse error: ") + e.what());
        }
        
        return result;
    }
    
    BackendValidationResult validate_python(const std::string& code) {
        BackendValidationResult result;
        result.validator_used = "python";
        
        if (!ToolExecutor::is_tool_available("python")) {
            result.warnings.push_back("Python not available for validation");
            result.valid = check_python_syntax_basic(code);
            return result;
        }
        
        TempFile tmp(".py", code);
        ToolExecutor::Result exec_result = ToolExecutor::execute("python -m py_compile " + tmp.path(), config_.timeout_ms);
        
        if (exec_result.exit_code == 0) {
            result.valid = true;
        } else {
            result.valid = false;
            result.errors = parse_python_errors(exec_result.stderr_output);
        }
        
        return result;
    }
    
    BackendValidationResult validate_typescript(const std::string& code) {
        BackendValidationResult result;
        result.validator_used = "typescript";
        
        // Try tsc first, then fall back to node for basic JS syntax
        if (ToolExecutor::is_tool_available("tsc")) {
            TempFile tmp(".ts", code);
            ToolExecutor::Result exec_result = ToolExecutor::execute("tsc --noEmit " + tmp.path(), config_.timeout_ms);
            
            if (exec_result.exit_code == 0) {
                result.valid = true;
            } else {
                result.valid = false;
                result.errors = parse_typescript_errors(exec_result.stdout_output);
            }
        } else if (ToolExecutor::is_tool_available("node")) {
            // Fall back to basic JS syntax check
            TempFile tmp(".js", code); // Rename to .js for node
            ToolExecutor::Result exec_result = ToolExecutor::execute("node --check " + tmp.path(), config_.timeout_ms);
            
            if (exec_result.exit_code == 0) {
                result.valid = true;
                result.warnings.push_back("TypeScript validator not available, validated as JavaScript");
            } else {
                result.valid = false;
                result.errors = parse_node_errors(exec_result.stderr_output);
            }
        } else {
            result.warnings.push_back("TypeScript/Node not available for validation");
            result.valid = check_typescript_syntax_basic(code);
        }
        
        return result;
    }
    
    // Basic syntax checkers (fallback when tools not available)
    bool check_lua_syntax_basic(const std::string& code) const {
        // Empty code is invalid
        if (code.empty()) return false;
        
        // Check for balanced brackets and basic structure
        int brace_count = 0;
        int bracket_count = 0;
        int paren_count = 0;
        bool in_string = false;
        char string_char = '\0';
        
        for (size_t i = 0; i < code.size(); i++) {
            char c = code[i];
            
            // Handle strings
            if (!in_string && (c == '"' || c == '\'')) {
                in_string = true;
                string_char = c;
            } else if (in_string && c == string_char) {
                // Check for escape
                if (i > 0 && code[i-1] != '\\') {
                    in_string = false;
                }
            } else if (!in_string) {
                if (c == '{') brace_count++;
                else if (c == '}') brace_count--;
                else if (c == '[') bracket_count++;
                else if (c == ']') bracket_count--;
                else if (c == '(') paren_count++;
                else if (c == ')') paren_count--;
            }
        }
        
        return brace_count == 0 && bracket_count == 0 && paren_count == 0;
    }
    
    bool check_gdscript_syntax_basic(const std::string& code) const {
        // Empty code is invalid
        if (code.empty()) return false;
        
        // Similar to Lua but with GDScript-specific checks
        int brace_count = 0;
        int bracket_count = 0;
        int paren_count = 0;
        bool in_string = false;
        char string_char = '\0';
        
        for (size_t i = 0; i < code.size(); i++) {
            char c = code[i];
            
            if (!in_string && (c == '"' || c == '\'')) {
                in_string = true;
                string_char = c;
            } else if (in_string && c == string_char) {
                if (i > 0 && code[i-1] != '\\') {
                    in_string = false;
                }
            } else if (!in_string) {
                if (c == '{') brace_count++;
                else if (c == '}') brace_count--;
                else if (c == '[') bracket_count++;
                else if (c == ']') bracket_count--;
                else if (c == '(') paren_count++;
                else if (c == ')') paren_count--;
            }
        }
        
        return brace_count == 0 && bracket_count == 0 && paren_count == 0;
    }
    
    bool check_csharp_syntax_basic(const std::string& code) const {
        // Empty code is invalid
        if (code.empty()) return false;
        
        // Check for balanced brackets and basic C# structure
        int brace_count = 0;
        int bracket_count = 0;
        int paren_count = 0;
        bool in_string = false;
        bool in_char = false;
        bool in_comment = false;
        bool in_block_comment = false;
        
        for (size_t i = 0; i < code.size(); i++) {
            char c = code[i];
            
            // Handle comments
            if (!in_string && !in_char && !in_block_comment && c == '/' && i + 1 < code.size()) {
                if (code[i+1] == '/') {
                    in_comment = true;
                    continue;
                } else if (code[i+1] == '*') {
                    in_block_comment = true;
                    i++;
                    continue;
                }
            }
            
            if (in_comment && c == '\n') {
                in_comment = false;
                continue;
            }
            
            if (in_block_comment && c == '*' && i + 1 < code.size() && code[i+1] == '/') {
                in_block_comment = false;
                i++;
                continue;
            }
            
            if (in_comment || in_block_comment) continue;
            
            // Handle strings
            if (!in_string && !in_char && c == '"') {
                in_string = true;
            } else if (in_string && c == '"' && (i == 0 || code[i-1] != '\\')) {
                in_string = false;
            } else if (!in_string && !in_char && c == '\'') {
                in_char = true;
            } else if (in_char && c == '\'' && (i == 0 || code[i-1] != '\\')) {
                in_char = false;
            } else if (!in_string && !in_char) {
                if (c == '{') brace_count++;
                else if (c == '}') brace_count--;
                else if (c == '[') bracket_count++;
                else if (c == ']') bracket_count--;
                else if (c == '(') paren_count++;
                else if (c == ')') paren_count--;
            }
        }
        
        return brace_count == 0 && bracket_count == 0 && paren_count == 0 && !in_string && !in_char;
    }
    
    bool check_python_syntax_basic(const std::string& code) const {
        // Empty code is invalid
        if (code.empty()) return false;
        
        // Python uses indentation, check for basic structure
        int paren_count = 0;
        int bracket_count = 0;
        int brace_count = 0;
        bool in_string = false;
        char string_char = '\0';
        bool triple_string = false;
        
        for (size_t i = 0; i < code.size(); i++) {
            char c = code[i];
            
            // Handle triple-quoted strings
            if (!in_string && i + 2 < code.size() && (c == '"' || c == '\'') && 
                code[i+1] == c && code[i+2] == c) {
                in_string = true;
                string_char = c;
                triple_string = true;
                i += 2;
                continue;
            }
            
            if (triple_string && i + 2 < code.size() && c == string_char && 
                code[i+1] == c && code[i+2] == c) {
                in_string = false;
                triple_string = false;
                i += 2;
                continue;
            }
            
            if (!in_string && (c == '"' || c == '\'')) {
                in_string = true;
                string_char = c;
            } else if (in_string && c == string_char && !triple_string && 
                       (i == 0 || code[i-1] != '\\')) {
                in_string = false;
            } else if (!in_string) {
                if (c == '(') paren_count++;
                else if (c == ')') paren_count--;
                else if (c == '[') bracket_count++;
                else if (c == ']') bracket_count--;
                else if (c == '{') brace_count++;
                else if (c == '}') brace_count--;
            }
        }
        
        return paren_count == 0 && bracket_count == 0 && brace_count == 0;
    }
    
    bool check_typescript_syntax_basic(const std::string& code) const {
        // TypeScript is similar to JavaScript but with type annotations
        return check_csharp_syntax_basic(code); // Similar bracket structure
    }
    
    // JSON validation (built-in)
    bool is_valid_json(const std::string& str) const {
        if (str.empty()) return false;
        
        size_t pos = 0;
        if (!parse_json_value(str, pos)) return false;
        skip_json_whitespace(str, pos);
        return pos == str.length();
    }
    
    // JSON parsing helpers
    bool parse_json_value(const std::string& str, size_t& pos) const {
        skip_json_whitespace(str, pos);
        
        if (pos >= str.size()) return false;
        
        char c = str[pos];
        
        if (c == '"') return parse_json_string(str, pos);
        if (c == '{') return parse_json_object(str, pos);
        if (c == '[') return parse_json_array(str, pos);
        if (c == 't' || c == 'f') return parse_json_bool(str, pos);
        if (c == 'n') return parse_json_null(str, pos);
        if (c == '-' || std::isdigit(c)) return parse_json_number(str, pos);
        
        return false;
    }
    
    void skip_json_whitespace(const std::string& str, size_t& pos) const {
        while (pos < str.size() && std::isspace(str[pos])) pos++;
    }
    
    bool parse_json_string(const std::string& str, size_t& pos) const {
        if (pos >= str.size() || str[pos] != '"') return false;
        pos++;
        
        while (pos < str.size()) {
            if (str[pos] == '\\') {
                pos += 2;
                continue;
            }
            if (str[pos] == '"') {
                pos++;
                return true;
            }
            pos++;
        }
        
        return false;
    }
    
    bool parse_json_object(const std::string& str, size_t& pos) const {
        if (pos >= str.size() || str[pos] != '{') return false;
        pos++;
        skip_json_whitespace(str, pos);
        
        if (pos < str.size() && str[pos] == '}') {
            pos++;
            return true;
        }
        
        while (pos < str.size()) {
            if (!parse_json_string(str, pos)) return false;
            skip_json_whitespace(str, pos);
            if (pos >= str.size() || str[pos] != ':') return false;
            pos++;
            if (!parse_json_value(str, pos)) return false;
            skip_json_whitespace(str, pos);
            
            if (pos >= str.size()) return false;
            if (str[pos] == '}') {
                pos++;
                return true;
            }
            if (str[pos] != ',') return false;
            pos++;
            skip_json_whitespace(str, pos);
        }
        
        return false;
    }
    
    bool parse_json_array(const std::string& str, size_t& pos) const {
        if (pos >= str.size() || str[pos] != '[') return false;
        pos++;
        skip_json_whitespace(str, pos);
        
        if (pos < str.size() && str[pos] == ']') {
            pos++;
            return true;
        }
        
        while (pos < str.size()) {
            if (!parse_json_value(str, pos)) return false;
            skip_json_whitespace(str, pos);
            
            if (pos >= str.size()) return false;
            if (str[pos] == ']') {
                pos++;
                return true;
            }
            if (str[pos] != ',') return false;
            pos++;
            skip_json_whitespace(str, pos);
        }
        
        return false;
    }
    
    bool parse_json_bool(const std::string& str, size_t& pos) const {
        if (pos + 4 <= str.size() && str.substr(pos, 4) == "true") {
            pos += 4;
            return true;
        }
        if (pos + 5 <= str.size() && str.substr(pos, 5) == "false") {
            pos += 5;
            return true;
        }
        return false;
    }
    
    bool parse_json_null(const std::string& str, size_t& pos) const {
        if (pos + 4 <= str.size() && str.substr(pos, 4) == "null") {
            pos += 4;
            return true;
        }
        return false;
    }
    
    bool parse_json_number(const std::string& str, size_t& pos) const {
        if (pos >= str.size()) return false;
        
        size_t start = pos;
        
        // Optional minus
        if (str[pos] == '-') pos++;
        
        // Integer part
        if (pos < str.size() && str[pos] == '0') {
            pos++;
        } else if (pos < str.size() && std::isdigit(str[pos])) {
            while (pos < str.size() && std::isdigit(str[pos])) pos++;
        } else {
            return false;
        }
        
        // Optional fractional part
        if (pos < str.size() && str[pos] == '.') {
            pos++;
            if (pos >= str.size() || !std::isdigit(str[pos])) return false;
            while (pos < str.size() && std::isdigit(str[pos])) pos++;
        }
        
        // Optional exponent
        if (pos < str.size() && (str[pos] == 'e' || str[pos] == 'E')) {
            pos++;
            if (pos < str.size() && (str[pos] == '+' || str[pos] == '-')) pos++;
            if (pos >= str.size() || !std::isdigit(str[pos])) return false;
            while (pos < str.size() && std::isdigit(str[pos])) pos++;
        }
        
        return pos > start;
    }
    
    // Error parsers
    std::vector<std::string> parse_lua_errors(const std::string& output) const {
        std::vector<std::string> errors;
        std::istringstream stream(output);
        std::string line;
        
        while (std::getline(stream, line)) {
            // Lua error format: lua: filename:line: message
            if (line.find("lua:") != std::string::npos || 
                line.find("error:") != std::string::npos ||
                line.find("Error:") != std::string::npos) {
                errors.push_back(line);
            }
        }
        
        return errors;
    }
    
    std::vector<std::string> parse_godot_errors(const std::string& output) const {
        std::vector<std::string> errors;
        std::istringstream stream(output);
        std::string line;
        
        while (std::getline(stream, line)) {
            // Godot error format: ERROR: message at line X
            if (line.find("ERROR") != std::string::npos ||
                line.find("SCRIPT ERROR") != std::string::npos ||
                line.find("Parse Error") != std::string::npos) {
                errors.push_back(line);
            }
        }
        
        return errors;
    }
    
    std::vector<std::string> parse_csharp_errors(const std::string& output) const {
        std::vector<std::string> errors;
        std::istringstream stream(output);
        std::string line;
        
        while (std::getline(stream, line)) {
            // C# compiler error format: file.cs(line,col): error CSXXXX: message
            if (line.find("error CS") != std::string::npos ||
                line.find(": error ") != std::string::npos) {
                errors.push_back(line);
            }
        }
        
        return errors;
    }
    
    std::vector<std::string> parse_python_errors(const std::string& output) const {
        std::vector<std::string> errors;
        std::istringstream stream(output);
        std::string line;
        
        while (std::getline(stream, line)) {
            // Python error format: SyntaxError: message
            if (line.find("SyntaxError") != std::string::npos ||
                line.find("IndentationError") != std::string::npos ||
                line.find("Error:") != std::string::npos) {
                errors.push_back(line);
            }
        }
        
        return errors;
    }
    
    std::vector<std::string> parse_typescript_errors(const std::string& output) const {
        std::vector<std::string> errors;
        std::istringstream stream(output);
        std::string line;
        
        while (std::getline(stream, line)) {
            // TypeScript error format: file.ts(line,col): error TSXXXX: message
            if (line.find("error TS") != std::string::npos ||
                line.find(": error ") != std::string::npos) {
                errors.push_back(line);
            }
        }
        
        return errors;
    }
    
    std::vector<std::string> parse_node_errors(const std::string& output) const {
        std::vector<std::string> errors;
        std::istringstream stream(output);
        std::string line;
        
        while (std::getline(stream, line)) {
            // Node.js error format: SyntaxError: message
            if (line.find("SyntaxError") != std::string::npos ||
                line.find("TypeError") != std::string::npos ||
                line.find("ReferenceError") != std::string::npos) {
                errors.push_back(line);
            }
        }
        
        return errors;
    }
    
    // Helper for minimal C# project
    std::string generate_minimal_csproj() const {
        return R"(<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>Library</OutputType>
    <TargetFramework>net6.0</TargetFramework>
    <NoWarn>CS0169;CS0649;CS1591</NoWarn>
  </PropertyGroup>
</Project>)";
    }
    
    // Validator registry
    static const std::map<std::string, std::function<BackendValidationResult(const std::string&)>>& validator_map() {
        static std::map<std::string, std::function<BackendValidationResult(const std::string&)>> validators;
        return validators;
    }
};

} // namespace lex
