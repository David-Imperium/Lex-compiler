#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <set>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/validator.h"
#include "codegen/lua_backend.h"
#include "codegen/json_backend.h"

namespace fs = std::filesystem;

void print_usage(const char* program) {
    std::cerr << "Lex Compiler v0.1.0\n\n";
    std::cerr << "Usage: " << program << " <input.lex> [options]\n\n";
    std::cerr << "Options:\n";
    std::cerr << "  -o, --output <dir>   Output directory (default: same as input)\n";
    std::cerr << "  -t, --target <fmt>   Output format(s): lua, json (comma-separated)\n";
    std::cerr << "  --no-validate        Skip semantic validation\n";
    std::cerr << "  -h, --help           Show this help\n";
}

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot write file: " + path);
    }
    file << content;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string input_file;
    std::string output_dir;
    std::string target = "lua";
    bool validate = true;

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
            target = argv[++i];
        } else if (arg == "--no-validate") {
            validate = false;
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
    }

    // Read input
    std::string source;
    try {
        source = read_file(input_file);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    // Lexical analysis
    lex::Lexer lexer(source, input_file);
    auto tokens = lexer.tokenize();
    if (lexer.has_errors()) {
        std::cerr << "Lexer errors:\n";
        for (const auto& err : lexer.errors()) {
            std::cerr << "  " << err << "\n";
        }
        return 1;
    }

    // Parsing
    lex::Parser parser(tokens);
    auto ast = parser.parse();
    if (parser.has_errors()) {
        std::cerr << "Parser errors:\n";
        for (const auto& err : parser.errors()) {
            std::cerr << "  " << err << "\n";
        }
        return 1;
    }

    // Semantic validation
    if (validate) {
        lex::Validator validator;
        validator.validate(ast);
        
        // Show warnings first
        if (validator.has_warnings()) {
            std::cerr << "Warnings:\n";
            for (const auto& warn : validator.warnings()) {
                std::cerr << "  [" << warn.code << "] " << warn.message << "\n";
            }
        }
        
        // Then errors (fatal)
        if (validator.has_errors()) {
            std::cerr << "Validation errors:\n";
            for (const auto& err : validator.errors()) {
                std::cerr << "  [" << err.code << "] " << err.message << "\n";
            }
            return 1;
        }
    }

    // Parse targets (comma-separated)
    std::set<std::string> targets;
    std::stringstream ts(target);
    std::string t;
    while (std::getline(ts, t, ',')) {
        // Trim whitespace
        t.erase(0, t.find_first_not_of(" \t"));
        t.erase(t.find_last_not_of(" \t") + 1);
        targets.insert(t);
    }

    // Determine output path base
    fs::path input_path(input_file);
    std::string base_name = input_path.stem().string();

    // Generate for each target
    for (const auto& fmt : targets) {
        std::string output;
        std::string extension;

        if (fmt == "lua") {
            lex::LuaBackend backend;
            output = backend.generate(ast);
            extension = ".lua";
        } else if (fmt == "json") {
            lex::JsonBackend backend;
            output = backend.generate(ast);
            extension = ".json";
        } else {
            std::cerr << "Error: Unknown target format: " << fmt << "\n";
            std::cerr << "Supported targets: lua, json\n";
            return 1;
        }

        fs::path output_path = fs::path(output_dir) / (base_name + extension);

        try {
            write_file(output_path.string(), output);
            std::cout << "Generated: " << output_path.string() << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
    }

    return 0;
}
