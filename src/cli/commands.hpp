#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <set>
#include <csignal>
#include <mutex>

#include "color.hpp"
#include "spinner.hpp"
#include "watcher.hpp"
#include "compiler.hpp"
#include "../lex/lex.hpp"
#include "../context/context.hpp"
#include "../context/query.hpp"
#include "../license/license.hpp"
#include "../codegen/backend_validator.hpp"

namespace lex::cli {

namespace fs = std::filesystem;

// Utility functions
inline void write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) throw std::runtime_error("Cannot write file: " + path);
    file << content;
}

inline std::vector<std::string> collect_lex_files(const std::string& path) {
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

inline std::vector<Target> parse_targets(const std::string& target_str) {
    std::vector<Target> targets;
    std::stringstream ss(target_str);
    std::string t;
    while (std::getline(ss, t, ',')) {
        if (t == "lua") targets.push_back(Target::Lua);
        else if (t == "json") targets.push_back(Target::JSON);
        else if (t == "gd" || t == "godot") targets.push_back(Target::Godot);
        else if (t == "cs" || t == "unity") targets.push_back(Target::Unity);
        else if (t == "love2d" || t == "love") targets.push_back(Target::Love2D);
        else if (t == "defold") targets.push_back(Target::Defold);
    }
    return targets;
}

inline std::vector<std::string> parse_target_names(const std::string& target_str) {
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

// Command result wrapper for CLI operations
struct CommandResult {
    int exit_code = 0;
    std::string error;
};

inline CommandResult run_watch_mode(
    const std::string& input_path,
    const std::string& output_dir,
    const std::vector<Target>& targets,
    const std::vector<std::string>& target_names,
    CompileOptions& options) {

    std::cout << color::logo();
    std::cout << color::cyan << "Watch Mode" << color::reset << "\n";
    std::cout << "  Input:  " << input_path << "\n";
    std::cout << "  Output: " << output_dir << "\n";
    std::cout << color::dim << "Press Ctrl+C to stop" << color::reset << "\n\n";

    std::vector<std::string> files = collect_lex_files(input_path);
    if (files.empty()) {
        std::cerr << color::red << "No .lex files found" << color::reset << "\n";
        return {1, "No .lex files found"};
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
            if (!ok) std::cout << color::red << "  X " << file << color::reset << "\n";
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
            fs::path inp_path(file);
            std::string base_name = inp_path.stem().string();

            for (const auto& [target_name, output] : result.outputs) {
                fs::path output_path = fs::path(output_dir) / (base_name + target_extension(target_name));
                try {
                    write_file(output_path.string(), output);
                    std::cout << color::green << "  -> " << color::reset << output_path.string() << "\n";
                } catch (const std::exception& e) {
                    std::cout << color::red << "  X " << e.what() << color::reset << "\n";
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
    return {0, ""};
}

inline CommandResult run_compile(
    const std::string& input_file,
    const std::string& output_dir,
    const std::vector<Target>& targets,
    const std::vector<std::string>& target_names,
    CompileOptions& options,
    bool verbose) {

    std::vector<std::string> files = collect_lex_files(input_file);
    if (files.empty()) {
        std::cerr << color::red << "No .lex files found" << color::reset << "\n";
        return {1, "No .lex files found"};
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
                    std::cout << color::red << "  X " << file << color::reset << "\n";
                }
            }
        }
        return {errors > 0 ? 1 : 0, ""};
    }

    // Single file compilation with animation
    AnimatedSpinner spinner("Compiling " + input_file, AnimatedSpinner::Braille);
    spinner.start();

    lex::CompileResult result = lex::compile_file(input_file, options);

    if (result.has_warnings()) {
        spinner.stop();
        std::cout << "\n";
        for (const auto& warn : result.warnings) {
            std::cout << color::yellow << "  ! " << warn.message;
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
            if (!err.suggestion.empty()) std::cout << "\n    " << color::cyan << "Suggestion: " << err.suggestion << color::reset;
            std::cout << "\n";
        }
        return {1, "Compilation failed"};
    }

    spinner.success("Compiled successfully");

    fs::path inp_path(input_file);
    std::string base_name = inp_path.stem().string();

    // Validate generated code if validation is enabled
    if (options.validate) {
        BackendValidator validator;
        bool has_validation_errors = false;

        for (const auto& [target_name, output] : result.outputs) {
            auto validation = validator.validate(target_name, output);
            if (!validation.valid) {
                has_validation_errors = true;
                std::cout << color::red << "  X Validation failed for " << target_name << color::reset << "\n";
                for (const auto& err : validation.errors) {
                    std::cout << color::red << "    - " << err << color::reset << "\n";
                }
            } else if (!validation.warnings.empty()) {
                std::cout << color::yellow << "  ! " << target_name << " validated with warnings" << color::reset << "\n";
                for (const auto& warn : validation.warnings) {
                    std::cout << color::yellow << "    - " << warn << color::reset << "\n";
                }
            } else if (verbose) {
                std::cout << color::green << "  ✓ " << target_name << " syntax validated" << color::reset << "\n";
            }
        }

        if (has_validation_errors) {
            return {1, "Code validation failed"};
        }
    }

    for (const auto& [target_name, output] : result.outputs) {
        fs::path output_path = fs::path(output_dir) / (base_name + target_extension(target_name));
        try {
            write_file(output_path.string(), output);
            std::cout << color::green << "  -> " << color::reset << output_path.string() << "\n";
        } catch (const std::exception& e) {
            std::cout << color::red << "  X " << e.what() << color::reset << "\n";
            return {1, e.what()};
        }
    }

    return {0, ""};
}

inline CommandResult run_context(
    const std::string& input_file,
    const std::string& output_dir,
    const std::string& context_str) {

    AnimatedSpinner ctx_spinner("Generating AI context...", AnimatedSpinner::Moon);
    ctx_spinner.start();

    ContextOptions ctx_opts;
    ctx_opts.include_graph = true;
    ctx_opts.include_statistics = true;
    ctx_opts.include_summaries = true;

    auto ctx_result = generate_context_from_file(input_file, ctx_opts);
    if (!ctx_result.success) {
        ctx_spinner.fail("Context generation failed: " + ctx_result.error);
        return {1, ctx_result.error};
    }

    auto write_context = [&](ContextFormat fmt) {
        std::string ext = context_extension(fmt);
        std::string out;
        switch (fmt) {
            case ContextFormat::JSON: out = format_context_json(ctx_result, ctx_opts); break;
            case ContextFormat::Markdown: out = format_context_markdown(ctx_result, ctx_opts); break;
            case ContextFormat::Minimal: out = format_context_minimal(ctx_result, ctx_opts); break;
        }
        fs::path inp_path(input_file);
        std::string base_name = inp_path.stem().string();
        fs::path p = fs::path(output_dir) / (base_name + ext);
        write_file(p.string(), out);
        std::cout << color::green << "  -> " << color::reset << p.string() << "\n";
    };

    if (context_str == "json") write_context(ContextFormat::JSON);
    else if (context_str == "md") write_context(ContextFormat::Markdown);
    else if (context_str == "minimal") write_context(ContextFormat::Minimal);
    else if (context_str == "all") {
        write_context(ContextFormat::JSON);
        write_context(ContextFormat::Markdown);
        write_context(ContextFormat::Minimal);
    }

    ctx_spinner.success("Context generated");
    return {0, ""};
}

inline CommandResult run_query(
    const std::string& input_file,
    const std::string& query_str) {

    AnimatedSpinner query_spinner("Querying...", AnimatedSpinner::Clock);
    query_spinner.start();

    ContextOptions ctx_opts;
    ctx_opts.include_graph = true;
    auto qr = query_file(input_file, query_str, ctx_opts);

    if (!qr.success) {
        query_spinner.fail(qr.error);
        return {1, qr.error};
    }

    query_spinner.stop();
    std::cout << "\n" << color::cyan << "Answer:" << color::reset << " " << qr.answer << "\n";
    return {0, ""};
}

} // namespace lex::cli
