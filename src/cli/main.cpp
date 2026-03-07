#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <thread>
#include <map>
#include <iomanip>
#include <mutex>
#include <future>
#include <functional>
#include <csignal>
#include <atomic>
#include <set>

#include <CLI/CLI.hpp>

#include "../lex/lex.hpp"
#include "../context/context.hpp"
#include "../context/query.hpp"
#include "../license/license.hpp"

// Lex-Plus premium backends (conditional include)
#if __has_include("lex_plus.hpp")
#include "lex_plus.hpp"
#endif

namespace fs = std::filesystem;

// ============================================================================
// Animated Spinner / Progress Bar
// ============================================================================

class AnimatedSpinner {
public:
    enum Style { Dots, Braille, Arrow, Pulse, Earth, Moon, Clock, Bars };

    AnimatedSpinner(const std::string& message = "", Style style = Style::Dots)
        : message_(message), style_(style), running_(false), progress_(-1) {}

    void set_progress(int percent) { progress_ = percent; }
    void set_message(const std::string& msg) { message_ = msg; }

    void start() {
        running_ = true;
        thread_ = std::thread(&AnimatedSpinner::animate, this);
    }

    void stop() {
        running_ = false;
        if (thread_.joinable()) thread_.join();
        clear_line();
    }

    void success(const std::string& msg = "") {
        stop();
        std::cout << "\r\033[32m✓\033[0m " << (msg.empty() ? message_ : msg) << "\n";
    }

    void fail(const std::string& msg = "") {
        stop();
        std::cout << "\r\033[31m✗\033[0m " << (msg.empty() ? message_ : msg) << "\n";
    }

private:
    std::string message_;
    Style style_;
    std::atomic<bool> running_;
    std::atomic<int> progress_;
    std::thread thread_;

    const std::vector<std::vector<std::string>> frames_ = {
        {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"}, // Braille
        {"◜", "◠", "◝", "◞", "◡", "◟"},                       // Dots
        {"←", "↖", "↑", "↗", "→", "↘", "↓", "↙"},               // Arrow
        {"◉", "◎", "●", "◎"},                                   // Pulse
        {"🌍", "🌎", "🌏"},                                      // Earth
        {"🌑", "🌒", "🌓", "🌔", "🌕", "🌖", "🌗", "🌘"},      // Moon
        {"🕛", "🕐", "🕑", "🕒", "🕓", "🕔", "🕕", "🕖", "🕗", "🕘", "🕙", "🕚"}, // Clock
        {"▁", "▂", "▃", "▄", "▅", "▆", "▇", "█"}                // Bars
    };

    void animate() {
        const auto& frames = frames_[static_cast<int>(style_)];
        size_t frame = 0;

        // Hide cursor
        std::cout << "\033[?25l";

        while (running_) {
            std::string output = "\r";

            if (progress_ >= 0 && progress_ <= 100) {
                // Progress bar mode
                int filled = progress_ / 5;
                output += "[";
                for (int i = 0; i < filled; ++i) output += "\u2588";  // █
                for (int i = 0; i < 20 - filled; ++i) output += "\u2591";  // ░
                output += "] ";
                output += std::to_string(progress_) + "% ";
                output += message_;
            } else {
                // Spinner mode
                output += "\033[36m" + frames[frame] + "\033[0m ";
                output += message_;
            }

            std::cout << output << std::flush;
            frame = (frame + 1) % frames.size();
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }

        // Show cursor
        std::cout << "\033[?25h";
    }

    void clear_line() {
        std::cout << "\r" << std::string(message_.size() + 50, ' ') << "\r" << std::flush;
    }
};

// ============================================================================
// Color utilities
// ============================================================================

namespace color {
    constexpr const char* reset = "\033[0m";
    constexpr const char* bold = "\033[1m";
    constexpr const char* dim = "\033[2m";
    constexpr const char* red = "\033[31m";
    constexpr const char* green = "\033[32m";
    constexpr const char* yellow = "\033[33m";
    constexpr const char* blue = "\033[34m";
    constexpr const char* magenta = "\033[35m";
    constexpr const char* cyan = "\033[36m";
    constexpr const char* white = "\033[37m";

    inline std::string logo() {
        return std::string(green) + bold + R"(
    ╔═══════════════════════════════════╗
    ║  " + cyan + "Lex" + green + " Compiler v" + white + "1.0.0" + green + "              ║
    ║  Declarative Game Content        ║
    ╚═══════════════════════════════════╝
)" + reset;
    }
}

// ============================================================================
// File Watcher (cross-platform with efficient polling)
// ============================================================================

class FileWatcher {
public:
    using Callback = std::function<void(const std::string&)>;

    FileWatcher(const std::vector<std::string>& files, Callback callback)
        : files_(files), callback_(std::move(callback)), running_(true) {}

    void start() {
        // Record initial modification times
        for (const auto& file : files_) {
            try {
                mtimes_[file] = fs::last_write_time(file);
            } catch (...) {}
        }
        watch_thread_ = std::thread(&FileWatcher::watch_loop, this);
    }

    void stop() {
        running_ = false;
        if (watch_thread_.joinable()) {
            watch_thread_.join();
        }
    }

    ~FileWatcher() { stop(); }

private:
    std::vector<std::string> files_;
    Callback callback_;
    std::atomic<bool> running_;
    std::thread watch_thread_;
    std::map<std::string, fs::file_time_type> mtimes_;

    void watch_loop() {
        while (running_) {
            check_changes();
            // Poll every 200ms (more responsive than 500ms)
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    void check_changes() {
        for (const auto& file : files_) {
            try {
                auto current_mtime = fs::last_write_time(file);
                if (mtimes_[file] != current_mtime) {
                    mtimes_[file] = current_mtime;
                    callback_(file);
                }
            } catch (...) {}
        }
    }
};

// ============================================================================
// Parallel Compilation
// ============================================================================

class ParallelCompiler {
public:
    ParallelCompiler(size_t max_threads = 0) {
        max_threads_ = max_threads > 0 ? max_threads : 
            std::max(1u, std::thread::hardware_concurrency());
    }

    std::vector<std::pair<std::string, bool>> compile_all(
        const std::vector<std::string>& files,
        const std::string& output_dir,
        const std::vector<lex::Target>& targets,
        const std::vector<std::string>& target_names,
        const lex::CompileOptions& options) {

        std::vector<std::pair<std::string, bool>> results;
        std::vector<std::future<std::pair<std::string, bool>>> futures;

        for (const auto& file : files) {
            futures.push_back(std::async(std::launch::async, [=]() {
                auto opts = options;
                opts.source_name = file;
                bool success = compile_single(file, output_dir, targets, target_names, opts);
                return std::make_pair(file, success);
            }));

            // Limit concurrent tasks
            if (futures.size() >= max_threads_) {
                for (auto& f : futures) results.push_back(f.get());
                futures.clear();
            }
        }

        for (auto& f : futures) results.push_back(f.get());
        return results;
    }

private:
    size_t max_threads_;

    static bool compile_single(
        const std::string& input_file,
        const std::string& output_dir,
        const std::vector<lex::Target>& targets,
        const std::vector<std::string>& target_names,
        const lex::CompileOptions& options) {

        lex::CompileResult result = lex::compile_file(input_file, options);
        if (result.has_errors()) return false;

        fs::path input_path(input_file);
        std::string base_name = input_path.stem().string();

        for (size_t i = 0; i < targets.size() && i < target_names.size(); ++i) {
            std::string extension = lex::target_extension(target_names[i]);
            fs::path output_path = fs::path(output_dir) / (base_name + extension);

            try {
                std::ofstream file(output_path);
                if (!file.is_open()) return false;
                file << (result.outputs.count(target_names[i]) ?
                         result.outputs.at(target_names[i]) : "");
            } catch (...) { return false; }
        }
        return true;
    }
};

// ============================================================================
// Utility Functions
// ============================================================================

void write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) throw std::runtime_error("Cannot write file: " + path);
    file << content;
}

std::string get_timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%H:%M:%S");
    return ss.str();
}

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

std::vector<lex::Target> parse_targets(const std::string& target_str) {
    std::vector<lex::Target> targets;
    std::stringstream ss(target_str);
    std::string t;
    while (std::getline(ss, t, ',')) {
        if (t == "lua") targets.push_back(lex::Target::Lua);
        else if (t == "json") targets.push_back(lex::Target::JSON);
        else if (t == "gd" || t == "godot") targets.push_back(lex::Target::Godot);
        else if (t == "cs" || t == "unity") targets.push_back(lex::Target::Unity);
        else if (t == "love2d" || t == "love") targets.push_back(lex::Target::Love2D);
        else if (t == "defold") targets.push_back(lex::Target::Defold);
    }
    return targets;
}

std::vector<std::string> parse_target_names(const std::string& target_str) {
    std::vector<std::string> names;
    std::stringstream ss(target_str);
    std::string t;
    while (std::getline(ss, t, ',')) {
        t.erase(0, t.find_first_not_of(" \t"));
        t.erase(t.find_last_not_of(" \t") + 1);
        if (!t.empty()) names.push_back(t);
    }
    return names;
}

// ============================================================================
// Watch Mode
// ============================================================================

int run_watch_mode(
    const std::string& input_path,
    const std::string& output_dir,
    const std::vector<lex::Target>& targets,
    const std::vector<std::string>& target_names,
    lex::CompileOptions& options) {

    std::cout << color::logo();
    std::cout << color::cyan << "Watch Mode" << color::reset << "\n";
    std::cout << "  Input:  " << input_path << "\n";
    std::cout << "  Output: " << output_dir << "\n";
    std::cout << color::dim << "Press Ctrl+C to stop" << color::reset << "\n\n";

    std::vector<std::string> files = collect_lex_files(input_path);
    if (files.empty()) {
        std::cerr << color::red << "✗ No .lex files found" << color::reset << "\n";
        return 1;
    }

    std::cout << "Monitoring " << files.size() << " file(s)...\n\n";

    // Initial compilation with spinner
    AnimatedSpinner spinner("Initial compilation...", AnimatedSpinner::Earth);
    spinner.start();

    ParallelCompiler compiler;
    auto results = compiler.compile_all(files, output_dir, targets, target_names, options);

    int errors = 0;
    for (const auto& [file, ok] : results) {
        if (!ok) errors++;
    }

    if (errors == 0) {
        spinner.success("Compiled " + std::to_string(results.size()) + " files");
    } else {
        spinner.fail(std::to_string(errors) + " failed, " + std::to_string(results.size() - errors) + " ok");
        for (const auto& [file, ok] : results) {
            if (!ok) std::cout << color::red << "  ✗ " << file << color::reset << "\n";
        }
    }

    std::cout << "\n" << color::dim << "Watching for changes..." << color::reset << "\n";

    // File watcher with animated callback
    std::mutex output_mutex;
    FileWatcher watcher(files, [&](const std::string& file) {
        std::lock_guard<std::mutex> lock(output_mutex);
        
        AnimatedSpinner compile_spinner("Recompiling " + fs::path(file).filename().string(), AnimatedSpinner::Moon);
        compile_spinner.start();

        auto opts = options;
        opts.source_name = file;
        lex::CompileResult result = lex::compile_file(file, opts);

        if (result.has_errors()) {
            compile_spinner.fail("Compilation failed");
            for (const auto& err : result.errors) {
                std::cout << color::red << "  [" << err.code << "] " << err.message;
                if (!err.location.empty()) std::cout << " at " << err.location;
                std::cout << color::reset << "\n";
            }
        } else {
            compile_spinner.success("Compiled");
            fs::path input_path(file);
            std::string base_name = input_path.stem().string();

            for (const auto& [target_name, output] : result.outputs) {
                fs::path output_path = fs::path(output_dir) / (base_name + lex::target_extension(target_name));
                try {
                    write_file(output_path.string(), output);
                    std::cout << color::green << "  → " << color::reset << output_path.string() << "\n";
                } catch (const std::exception& e) {
                    std::cout << color::red << "  ✗ " << e.what() << color::reset << "\n";
                }
            }
        }
        std::cout << color::dim << "\nWatching..." << color::reset << "\n";
    });

    watcher.start();

    // Handle Ctrl+C
    std::signal(SIGINT, [](int) {
        std::cout << "\n" << color::yellow << "Stopping watch mode..." << color::reset << "\n";
        std::exit(0);
    });

    while (true) std::this_thread::sleep_for(std::chrono::hours(1));
    return 0;
}

// ============================================================================
// Main Entry Point with CLI11
// ============================================================================

int main(int argc, char* argv[]) {
    CLI::App app{"Lex Compiler - Declarative multi-target transpiler for game content", "lexc"};

    std::string input_file;
    app.add_option("input", input_file, "Input .lex file or directory")
       ->required()
       ->check(CLI::ExistingPath);

    std::string output_dir;
    app.add_option("-o,--output", output_dir, "Output directory");

    std::string target_str = "lua";
    app.add_option("-t,--target", target_str,
        "Output format(s): lua,json,gd,cs,love2d,defold\n"
        "  Free: lua, json, godot, unity, love2d, defold\n"
        "  Premium (Lex-Plus): unreal, gamemaker, construct, bevy, pygame, raylib, cpp, rust, python, rpc");

    std::string context_str;
    app.add_option("--context", context_str, "Generate AI context: json,md,minimal,all");

    std::string query_str;
    app.add_option("--query", query_str, "Query game data in natural language");

    std::string types_str;
    app.add_option("--types", types_str, "Custom definition types (comma-separated)");

    std::string mode_str = "modder";
    app.add_option("--mode", mode_str, "Visibility mode: modder or developer");

    bool watch_mode = false;
    app.add_flag("--watch", watch_mode, "Watch for file changes and recompile");

    bool no_validate = false;
    app.add_flag("--no-validate", no_validate, "Skip semantic validation");

    bool verbose = false;
    app.add_flag("--verbose", verbose, "Show detailed output");

    bool show_version = false;
    app.add_flag("-v,--version", show_version, "Show version");

    std::string license_key;
    app.add_option("--license", license_key, "License key (or set LEX_LICENSE env)");

    CLI11_PARSE(app, argc, argv);

    // Register premium backends if Lex-Plus is available
#if __has_include("lex_plus.hpp")
    lex::register_premium_backends();
#endif

    // Show version with animated logo
    if (show_version) {
        std::cout << color::logo();
        return 0;
    }

    if (output_dir.empty()) {
        output_dir = fs::path(input_file).parent_path().string();
        if (output_dir.empty()) output_dir = ".";
    }

    lex::CompileOptions options;
    options.validate = !no_validate;
    options.verbose = verbose;
    options.source_name = input_file;
    options.allow_internal = (mode_str == "developer" || mode_str == "dev");

    if (!types_str.empty()) {
        std::stringstream ss(types_str);
        std::string type;
        while (std::getline(ss, type, ',')) {
            type.erase(0, type.find_first_not_of(" \t"));
            type.erase(type.find_last_not_of(" \t") + 1);
            if (!type.empty()) options.types.push_back(type);
        }
    }

    std::vector<std::string> target_names = parse_target_names(target_str);
    std::vector<lex::Target> targets = parse_targets(target_str);
    options.targets = targets;
    
    // Add custom targets (names not in the standard enum)
    std::set<std::string> standard_targets = {"lua", "json", "gd", "godot", "cs", "unity", "love2d", "love", "defold", "ts", "typescript"};
    for (const auto& name : target_names) {
        if (standard_targets.find(name) == standard_targets.end()) {
            options.custom_targets.push_back(name);
        }
    }

    // ========================================================================
    // License Verification
    // ========================================================================

    std::optional<lex::license::License> license;

    // Check --license flag first, then LEX_LICENSE env
    if (!license_key.empty()) {
        license = lex::license::LicenseManager::parse(license_key);
        if (!license) {
            std::cerr << color::red << "✗ Invalid license key format" << color::reset << "\n";
            return 1;
        }
    } else {
        license = lex::license::LicenseManager::from_env();
    }

    // Check for premium backends without license
    if (!license) {
        // Free tier: only free backends allowed
        for (const auto& target_name : target_names) {
            if (lex::license::CORE_BACKENDS.count(target_name) > 0) {
                std::cerr << color::red << "✗ Backend '" << target_name
                          << "' requires a license" << color::reset << "\n";
                std::cerr << color::yellow << "  Get a license: https://github.com/sponsors/David-Imperium"
                          << color::reset << "\n";
                std::cerr << color::dim << "  Free backends: lua, json, godot, unity, love2d, defold"
                          << color::reset << "\n";
                return 1;
            }
        }
    } else {
        // Licensed: check tier access
        auto available_backends = license->get_available_backends();
        for (const auto& target_name : target_names) {
            if (available_backends.count(target_name) == 0) {
                std::cerr << color::red << "✗ Your " << license->tier_name()
                          << " license doesn't include '" << target_name << "'" << color::reset << "\n";
                std::cerr << color::yellow << "  Upgrade: https://github.com/sponsors/David-Imperium"
                          << color::reset << "\n";
                return 1;
            }
        }

        if (verbose) {
            std::cout << color::green << "✓ License: " << license->tier_name()
                      << " (" << (license->is_lifetime ? "lifetime" : "subscription") << ")"
                      << color::reset << "\n";
        }
    }

    // ========================================================================
    // Compilation
    // ========================================================================

    if (watch_mode) {
        return run_watch_mode(input_file, output_dir, targets, target_names, options);
    }

    std::vector<std::string> files = collect_lex_files(input_file);
    if (files.empty()) {
        std::cerr << color::red << "✗ No .lex files found" << color::reset << "\n";
        return 1;
    }

    // Parallel compilation for multiple files
    if (files.size() > 1) {
        std::cout << color::cyan << "Compiling " << files.size() << " files..." << color::reset << "\n\n";

        AnimatedSpinner spinner("Parallel compilation...", AnimatedSpinner::Braille);
        spinner.start();

        ParallelCompiler compiler;
        auto results = compiler.compile_all(files, output_dir, targets, target_names, options);

        int errors = 0;
        int success = 0;
        for (const auto& [file, ok] : results) {
            if (!ok) errors++;
            else success++;
        }

        if (errors == 0) {
            spinner.success("Compiled " + std::to_string(success) + " files successfully");
            if (verbose) {
                for (const auto& [file, ok] : results) {
                    std::cout << color::dim << "  " << file << color::reset << "\n";
                }
            }
        } else {
            spinner.fail(std::to_string(errors) + " file(s) failed, " + std::to_string(success) + " succeeded");
            for (const auto& [file, ok] : results) {
                if (!ok) {
                    std::cout << color::red << "  ✗ " << file << color::reset << "\n";
                }
            }
        }
        return errors > 0 ? 1 : 0;
    }

    // Single file compilation with animation
    AnimatedSpinner spinner("Compiling " + input_file, AnimatedSpinner::Braille);
    spinner.start();

    lex::CompileResult result = lex::compile_file(input_file, options);

    if (result.has_warnings()) {
        spinner.stop();
        std::cout << "\n";
        for (const auto& warn : result.warnings) {
            std::cout << color::yellow << "  ⚠ " << warn.message;
            if (!warn.location.empty()) std::cout << " at " << warn.location;
            std::cout << color::reset << "\n";
        }
        spinner.start();
    }

    if (result.has_errors()) {
        spinner.fail("Compilation failed");
        std::cout << "\n";
        for (const auto& err : result.errors) {
            std::cout << color::red << "  [" << err.code << "] " << err.message;
            if (!err.location.empty()) std::cout << " at " << err.location;
            if (!err.suggestion.empty()) std::cout << "\n    " << color::cyan << "💡 " << err.suggestion << color::reset;
            std::cout << "\n";
        }
        return 1;
    }

    spinner.success("Compiled successfully");

    fs::path input_path(input_file);
    std::string base_name = input_path.stem().string();

    for (const auto& [target_name, output] : result.outputs) {
        fs::path output_path = fs::path(output_dir) / (base_name + lex::target_extension(target_name));
        try {
            write_file(output_path.string(), output);
            std::cout << color::green << "  → " << color::reset << output_path.string() << "\n";
        } catch (const std::exception& e) {
            std::cout << color::red << "  ✗ " << e.what() << color::reset << "\n";
            return 1;
        }
    }

    if (!context_str.empty()) {
        AnimatedSpinner ctx_spinner("Generating AI context...", AnimatedSpinner::Moon);
        ctx_spinner.start();

        lex::ContextOptions ctx_opts;
        ctx_opts.include_graph = true;
        ctx_opts.include_statistics = true;
        ctx_opts.include_summaries = true;

        auto ctx_result = lex::generate_context_from_file(input_file, ctx_opts);
        if (!ctx_result.success) {
            ctx_spinner.fail("Context generation failed: " + ctx_result.error);
            return 1;
        }

        auto write_context = [&](lex::ContextFormat fmt) {
            std::string ext = lex::context_extension(fmt);
            std::string out;
            switch (fmt) {
                case lex::ContextFormat::JSON: out = lex::format_context_json(ctx_result, ctx_opts); break;
                case lex::ContextFormat::Markdown: out = lex::format_context_markdown(ctx_result, ctx_opts); break;
                case lex::ContextFormat::Minimal: out = lex::format_context_minimal(ctx_result, ctx_opts); break;
            }
            fs::path p = fs::path(output_dir) / (base_name + ext);
            write_file(p.string(), out);
            std::cout << color::green << "  → " << color::reset << p.string() << "\n";
        };

        if (context_str == "json") write_context(lex::ContextFormat::JSON);
        else if (context_str == "md") write_context(lex::ContextFormat::Markdown);
        else if (context_str == "minimal") write_context(lex::ContextFormat::Minimal);
        else if (context_str == "all") {
            write_context(lex::ContextFormat::JSON);
            write_context(lex::ContextFormat::Markdown);
            write_context(lex::ContextFormat::Minimal);
        }

        ctx_spinner.success("Context generated");
    }

    if (!query_str.empty()) {
        AnimatedSpinner query_spinner("Querying...", AnimatedSpinner::Clock);
        query_spinner.start();

        lex::ContextOptions ctx_opts;
        ctx_opts.include_graph = true;
        auto qr = lex::query_file(input_file, query_str, ctx_opts);

        if (!qr.success) {
            query_spinner.fail(qr.error);
            return 1;
        }

        query_spinner.stop();
        std::cout << "\n" << color::cyan << "Answer:" << color::reset << " " << qr.answer << "\n";
        return 0;
    }

    return 0;
}
