#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <set>
#include <chrono>
#include <thread>
#include <map>
#include <iomanip>

#include "../lex/lex.hpp"
#include "../context/context.hpp"

namespace fs = std::filesystem;

// File modification times for watch mode
std::map<std::string, std::filesystem::file_time_type> file_mtimes;

void print_usage(const char* program) {
    std::cerr << "Lex Compiler v" << lex::version() << "\n\n";
    std::cerr << "Usage: " << program << " <input.lex> [options]\n\n";
    std::cerr << "Options:\n";
    std::cerr << "  -o, --output <dir>   Output directory (default: same as input)\n";
    std::cerr << "  -t, --target <fmt>   Output format(s): lua, json, gd, cs, love2d, defold\n";
    std::cerr << "  --context <fmt>     Generate AI context: json, md, minimal, all\n";
    std::cerr << "  --types <list>       Definition types (comma-separated)\n";
    std::cerr << "                       Default: Imperium types (era,structure,unit,...)\n";
    std::cerr << "  --mode <mode>        Visibility mode: modder (default) or developer\n";
    std::cerr << "                       modder = internal/private hidden, developer = all visible\n";
    std::cerr << "  --watch              Watch for file changes and recompile automatically\n";
    std::cerr << "  --no-validate        Skip semantic validation\n";
    std::cerr << "  --verbose            Show detailed output\n";
    std::cerr << "  -h, --help           Show this help\n";
}

void write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot write file: " + path);
    }
    file << content;
}

// Collect all .lex files from path (file or directory)
std::vector<std::string> collect_lex_files(const std::string& path) {
    std::vector<std::string> files;
    
    if (fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.path().extension() == ".lex") {
                files.push_back(entry.path().string());
            }
        }
    } else if (fs::is_regular_file(path)) {
        files.push_back(path);
    }
    
    return files;
}

// Check if any file has been modified
bool check_file_changes(const std::vector<std::string>& files) {
    bool changed = false;
    
    for (const auto& file : files) {
        try {
            auto current_mtime = fs::last_write_time(file);
            
            if (file_mtimes.find(file) == file_mtimes.end()) {
                // First time seeing this file
                file_mtimes[file] = current_mtime;
            } else if (file_mtimes[file] != current_mtime) {
                // File has been modified
                file_mtimes[file] = current_mtime;
                changed = true;
            }
        } catch (const fs::filesystem_error& e) {
            // File might not exist yet, skip
        }
    }
    
    return changed;
}

// Compile a single file
bool compile_file(const std::string& input_file, const std::string& output_dir,
                  const std::vector<lex::Target>& targets,
                  const std::vector<std::string>& target_names,
                  const lex::CompileOptions& options) {
    
    lex::CompileResult result = lex::compile_file(input_file, options);
    
    // Show warnings
    if (result.has_warnings()) {
        std::cerr << "Warnings:\n";
        for (const auto& warn : result.warnings) {
            std::cerr << "  [" << warn.code << "] " << warn.message;
            if (!warn.location.empty()) std::cerr << " at " << warn.location;
            std::cerr << "\n";
        }
    }
    
    // Show errors
    if (result.has_errors()) {
        std::cerr << "Errors:\n";
        for (const auto& err : result.errors) {
            std::cerr << "  [" << err.code << "] " << err.message;
            if (!err.location.empty()) std::cerr << " at " << err.location;
            if (!err.suggestion.empty()) std::cerr << "\n    Suggestion: " << err.suggestion;
            std::cerr << "\n";
        }
        return false;
    }
    
    // Write output files
    fs::path input_path(input_file);
    std::string base_name = input_path.stem().string();
    
    for (size_t i = 0; i < targets.size() && i < target_names.size(); ++i) {
        std::string extension = lex::target_extension(target_names[i]);
        fs::path output_path = fs::path(output_dir) / (base_name + extension);
        
        try {
            write_file(output_path.string(), result.outputs.count(target_names[i]) ? 
                       result.outputs.at(target_names[i]) : "");
            std::cout << "Generated: " << output_path.string() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return false;
        }
    }
    
    return true;
}

// Get current timestamp for logging
std::string get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S");
    return ss.str();
}

// Watch mode - monitor files and recompile on changes
int watch_mode(const std::string& input_path, const std::string& output_dir,
               const std::vector<lex::Target>& targets,
               const std::vector<std::string>& target_names,
               lex::CompileOptions& options) {
    
    std::cout << "Lex Compiler v" << lex::version() << " - Watch Mode\n";
    std::cout << "Watching: " << input_path << "\n";
    std::cout << "Output: " << output_dir << "\n";
    std::cout << "Press Ctrl+C to stop\n\n";
    
    // Collect files to watch
    std::vector<std::string> files = collect_lex_files(input_path);
    
    if (files.empty()) {
        std::cerr << "Error: No .lex files found\n";
        return 1;
    }
    
    std::cout << "Monitoring " << files.size() << " file(s)\n\n";
    
    // Initial compilation
    for (const auto& file : files) {
        std::cout << "Compiling: " << file << "\n";
        options.source_name = file;
        if (!compile_file(file, output_dir, targets, target_names, options)) {
            std::cerr << "Initial compilation failed for: " << file << "\n";
        }
    }
    
    std::cout << "\nWatching for changes...\n";
    
    // Watch loop
    while (true) {
        // Check for changes
        if (check_file_changes(files)) {
            std::cout << "\n[" << get_timestamp() << "] Change detected\n";
            
            for (const auto& file : files) {
                try {
                    auto mtime = fs::last_write_time(file);
                    if (file_mtimes[file] == mtime) continue; // Skip unchanged
                    
                    std::cout << "Recompiling: " << file << "\n";
                    options.source_name = file;
                    
                    if (compile_file(file, output_dir, targets, target_names, options)) {
                        std::cout << "Success: " << file << "\n";
                    } else {
                        std::cerr << "Failed: " << file << "\n";
                    }
                } catch (const fs::filesystem_error& e) {
                    std::cerr << "Error accessing: " << file << "\n";
                }
            }
            
            std::cout << "Watching for changes...\n";
        }
        
        // Sleep to avoid busy waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string input_file;
    std::string output_dir;
    std::string target_str = "lua";
    std::string types_str;
    std::string mode_str = "modder";
    std::string context_str;  // AI context format
    bool validate = true;
    bool verbose = false;
    bool watch_mode_enabled = false;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --output requires a directory\n";
                return 1;
            }
            output_dir = argv[++i];
        } else if (arg == "-t" || arg == "--target") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --target requires a format\n";
                return 1;
            }
            target_str = argv[++i];
        } else if (arg == "--context") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --context requires a format (json, md, minimal, all)\n";
                return 1;
            }
            context_str = argv[++i];
        } else if (arg == "--types") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --types requires a list\n";
                return 1;
            }
            types_str = argv[++i];
        } else if (arg == "--mode") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --mode requires modder or developer\n";
                return 1;
            }
            mode_str = argv[++i];
        } else if (arg == "--watch") {
            watch_mode_enabled = true;
        } else if (arg == "--no-validate") {
            validate = false;
        } else if (arg == "--verbose") {
            verbose = true;
        } else if (arg[0] != '-') {
            input_file = arg;
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            return 1;
        }
    }

    if (input_file.empty()) {
        std::cerr << "Error: No input file specified\n";
        return 1;
    }

    // Default output dir to input file's directory
    if (output_dir.empty()) {
        output_dir = fs::path(input_file).parent_path().string();
        if (output_dir.empty()) output_dir = ".";
    }

    // Configure compilation options
    lex::CompileOptions options;
    options.validate = validate;
    options.verbose = verbose;
    options.source_name = input_file;

    // Set mode (visibility filtering)
    if (mode_str == "developer" || mode_str == "dev") {
        options.allow_internal = true;
    } else if (mode_str == "modder") {
        options.allow_internal = false;
    } else {
        std::cerr << "Error: --mode must be 'modder' or 'developer'\n";
        return 1;
    }

    // Parse types
    if (!types_str.empty()) {
        std::stringstream ss(types_str);
        std::string type;
        while (std::getline(ss, type, ',')) {
            type.erase(0, type.find_first_not_of(" \t"));
            type.erase(type.find_last_not_of(" \t") + 1);
            if (!type.empty()) {
                options.types.push_back(type);
            }
        }
    }

    // Parse targets
    std::vector<std::string> target_names;
    std::vector<lex::Target> targets;
    {
        std::stringstream ts(target_str);
        std::string t;
        while (std::getline(ts, t, ',')) {
            t.erase(0, t.find_first_not_of(" \t"));
            t.erase(t.find_last_not_of(" \t") + 1);
            if (!t.empty()) {
                target_names.push_back(t);
                if (t == "lua") targets.push_back(lex::Target::Lua);
                else if (t == "json") targets.push_back(lex::Target::JSON);
                else if (t == "gd" || t == "godot") targets.push_back(lex::Target::Godot);
                else if (t == "cs" || t == "unity" || t == "csharp") targets.push_back(lex::Target::Unity);
                else if (t == "love2d" || t == "love") targets.push_back(lex::Target::Love2D);
                else if (t == "defold") targets.push_back(lex::Target::Defold);
            }
        }
    }

    // Watch mode
    if (watch_mode_enabled) {
        return watch_mode(input_file, output_dir, targets, target_names, options);
    }

    // Single compilation
    lex::CompileResult result = lex::compile_file(input_file, options);

    // Show warnings
    if (result.has_warnings()) {
        std::cerr << "Warnings:\n";
        for (const auto& warn : result.warnings) {
            std::cerr << "  [" << warn.code << "] " << warn.message;
            if (!warn.location.empty()) std::cerr << " at " << warn.location;
            std::cerr << "\n";
        }
    }

    // Show errors
    if (result.has_errors()) {
        std::cerr << "Errors:\n";
        for (const auto& err : result.errors) {
            std::cerr << "  [" << err.code << "] " << err.message;
            if (!err.location.empty()) std::cerr << " at " << err.location;
            if (!err.suggestion.empty()) std::cerr << "\n    Suggestion: " << err.suggestion;
            std::cerr << "\n";
        }
        return 1;
    }

    // Write output files
    fs::path input_path(input_file);
    std::string base_name = input_path.stem().string();

    for (const auto& [target_name, output] : result.outputs) {
        std::string extension = lex::target_extension(target_name);
        fs::path output_path = fs::path(output_dir) / (base_name + extension);

        try {
            write_file(output_path.string(), output);
            std::cout << "Generated: " << output_path.string() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
    }

    // Generate AI context if requested
    if (!context_str.empty()) {
        lex::ContextOptions ctx_options;
        ctx_options.include_graph = true;
        ctx_options.include_statistics = true;
        ctx_options.include_summaries = true;

        auto ctx_result = lex::generate_context_from_file(input_file, ctx_options);
        
        if (!ctx_result.success) {
            std::cerr << "Error generating context: " << ctx_result.error << "\n";
            return 1;
        }

        // Helper to write context file
        auto write_context = [&](lex::ContextFormat format) {
            std::string output;
            std::string ext = lex::context_extension(format);
            
            switch (format) {
                case lex::ContextFormat::JSON:
                    output = lex::format_context_json(ctx_result, ctx_options);
                    break;
                case lex::ContextFormat::Markdown:
                    output = lex::format_context_markdown(ctx_result, ctx_options);
                    break;
                case lex::ContextFormat::Minimal:
                    output = lex::format_context_minimal(ctx_result, ctx_options);
                    break;
            }
            
            fs::path output_path = fs::path(output_dir) / (base_name + ext);
            write_file(output_path.string(), output);
            std::cout << "Generated: " << output_path.string() << "\n";
        };

        // Generate requested format(s)
        if (context_str == "json") {
            write_context(lex::ContextFormat::JSON);
        } else if (context_str == "md" || context_str == "markdown") {
            write_context(lex::ContextFormat::Markdown);
        } else if (context_str == "minimal" || context_str == "txt") {
            write_context(lex::ContextFormat::Minimal);
        } else if (context_str == "all") {
            write_context(lex::ContextFormat::JSON);
            write_context(lex::ContextFormat::Markdown);
            write_context(lex::ContextFormat::Minimal);
        } else {
            std::cerr << "Error: --context must be json, md, minimal, or all\n";
            return 1;
        }
    }

    if (verbose) {
        std::cerr << "Compilation successful\n";
        std::cerr << "  Targets: " << target_names.size() << "\n";
        std::cerr << "  Warnings: " << result.warnings.size() << "\n";
    }

    return 0;
}