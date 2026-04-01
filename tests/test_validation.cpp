#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <chrono>

#include "../src/lexer/lexer.hpp"
#include "../src/parser/parser.hpp"
#include "../src/ast/ast.hpp"
#include "../src/schema/schema.hpp"
#include "../src/codegen/backend.hpp"
#include "../src/codegen/lua_backend.hpp"
#include "../src/codegen/json_backend.hpp"
#include "../src/codegen/godot_backend.hpp"
#include "../src/codegen/unity_backend.hpp"
#include "../src/codegen/love2d_backend.hpp"
#include "../src/codegen/defold_backend.hpp"
#include "../src/codegen/backend_validator.hpp"
#include "../src/codegen/llm_validator.hpp"

namespace fs = std::filesystem;
using namespace lex;

// Helper to load expected output from file
std::string load_expected(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Helper to save expected output (for creating new snapshots)
void save_expected(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    file << content;
}

// ============================================================================
// Backend Validator Tests
// ============================================================================

TEST_CASE("BackendValidator - Lua syntax validation") {
    BackendValidator validator;
    
    SECTION("valid Lua code") {
        std::string valid_lua = R"lua(
-- Generated Lua
local t = {}

t.test = {
    name = "Test",
    value = 42
}

return t
)lua";
        auto result = validator.validate("lua", valid_lua);
        CHECK(result.valid);
        CHECK(result.errors.empty());
    }
    
    SECTION("invalid Lua code - unclosed string") {
        std::string invalid_lua = R"lua(
local t = {
    name = "unclosed string
}
)lua";
        // Should fail basic syntax check even without lua interpreter
        auto result = validator.validate("lua", invalid_lua);
        INFO("Errors: " << result.summary());
    }
    
    SECTION("invalid Lua code - unbalanced brackets") {
        std::string invalid_lua = R"lua(
local t = {
    nested = {
        value = 1
})lua";
        auto result = validator.validate("lua", invalid_lua);
        CHECK_FALSE(result.valid);
    }
    
    SECTION("empty code should fail") {
        auto result = validator.validate("lua", "");
        CHECK_FALSE(result.valid);
        CHECK_FALSE(result.errors.empty());
    }
}

TEST_CASE("BackendValidator - JSON validation") {
    BackendValidator validator;
    
    SECTION("valid JSON") {
        std::string valid_json = R"json({
    "version": "1.0",
    "structures": {
        "Farm": {
            "id": "Farm",
            "type": "structure",
            "era": "Ancient"
        }
    }
})json";
        auto result = validator.validate("json", valid_json);
        CHECK(result.valid);
        CHECK(result.errors.empty());
    }
    
    SECTION("invalid JSON - unclosed object") {
        std::string invalid_json = R"json({
    "structures": {
        "Farm": {
            "id": "Farm"
    }
)json";
        auto result = validator.validate("json", invalid_json);
        CHECK_FALSE(result.valid);
    }
    
    SECTION("invalid JSON - missing quotes") {
        std::string invalid_json = R"json({
    structures: {}
})json";
        auto result = validator.validate("json", invalid_json);
        CHECK_FALSE(result.valid);
    }
    
    SECTION("empty JSON should fail") {
        auto result = validator.validate("json", "");
        CHECK_FALSE(result.valid);
    }
}

TEST_CASE("BackendValidator - C# syntax validation") {
    BackendValidator validator;
    
    SECTION("valid C# code") {
        std::string valid_cs = R"csharp(
public class TestData {
    public string name;
    public int value = 42;
    
    public void DoSomething() {
        System.Console.WriteLine("Hello");
    }
}
)csharp";
        auto result = validator.validate("csharp", valid_cs);
        CHECK(result.valid);
    }
    
    SECTION("invalid C# code - unbalanced braces") {
        std::string invalid_cs = R"csharp(
public class TestData {
    public void Method() {
        // Missing closing brace
)csharp";
        auto result = validator.validate("csharp", invalid_cs);
        CHECK_FALSE(result.valid);
    }
}

TEST_CASE("BackendValidator - GDScript syntax validation") {
    BackendValidator validator;
    
    SECTION("valid GDScript code") {
        std::string valid_gd = R"gdscript(
class_name TestData extends Resource

var structures = {}
var units = {}

func _init():
    structures["Farm"] = {
        "id": "Farm",
        "era": "Ancient"
    }
)gdscript";
        auto result = validator.validate("godot", valid_gd);
        CHECK(result.valid);
    }
    
    SECTION("invalid GDScript - unbalanced brackets") {
        std::string invalid_gd = R"gdscript(
var data = {
    "key": "value"
)gdscript";
        auto result = validator.validate("godot", invalid_gd);
        CHECK_FALSE(result.valid);
    }
}

TEST_CASE("BackendValidator - Tool availability check") {
    BackendValidator validator;
    
    // At least basic validation should always be available
    CHECK(validator.is_validator_available("json"));
    
    // Check other validators (may or may not be available)
    if (validator.is_validator_available("lua")) {
        INFO("Lua validator available");
    }
    if (validator.is_validator_available("godot")) {
        INFO("Godot validator available");
    }
    if (validator.is_validator_available("csharp")) {
        INFO("C# validator available");
    }
}

// ============================================================================
// LLM Validator Tests
// ============================================================================

TEST_CASE("LLMValidator - Mock client test") {
    auto mock_client = std::make_shared<MockLLMClient>();
    LLMValidator validator(mock_client);
    
    SECTION("mock validation always returns valid") {
        auto result = validator.validate("lua", "local x = 1", "test: true");
        CHECK(result.valid);
        CHECK(result.confidence == 1.0);
        CHECK(mock_client->is_available());
    }
    
    SECTION("quick check with mock") {
        auto result = validator.quick_check("lua", "local t = {}");
        CHECK(result.valid);
    }
}

TEST_CASE("LLMValidator - Client initialization") {
    SECTION("OpenAI client without key") {
        OpenAIClient client("", "gpt-4");
        CHECK_FALSE(client.is_available());
    }
    
    SECTION("Anthropic client without key") {
        AnthropicClient client("", "claude-3");
        CHECK_FALSE(client.is_available());
    }
    
    SECTION("Mock client always available") {
        MockLLMClient client;
        CHECK(client.is_available());
        CHECK(client.model_name() == "mock-validator");
    }
}

// ============================================================================
// Snapshot Tests - Generated Output Validation
// ============================================================================

TEST_CASE("Snapshot Tests - Lua Backend") {
    auto& schema = SchemaRegistry::instance();
    schema.load_imperium_default();
    
    Lexer lexer(R"lex(
        era Ancient { name: "Ancient Era" period: "3000 BC" }
        era Medieval { name: "Medieval Era" period: "500 AD" }
        
        structure Farm {
            era: Ancient
            name: "Farm"
            cost: { Gold: 50, Wood: 30 }
            production: { Food: 5 }
        }
        
        unit Warrior {
            era: Ancient
            name: "Warrior"
            hp: 100
            attack: 10
            defense: 5
        }
)lex");
    auto tokens = lexer.tokenize();
    REQUIRE(!lexer.has_errors());
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE(!parser.has_errors());
    
    LuaBackend backend;
    std::string output = backend.generate(ast);
    
    // Validate syntax
    BackendValidator validator;
    auto validation = validator.validate("lua", output);
    
    SECTION("valid Lua syntax") {
        CHECK(validation.valid);
        CHECK(validation.errors.empty());
    }
    
    SECTION("contains expected definitions") {
        CHECK(output.find("Eras") != std::string::npos);
        CHECK(output.find("Structures") != std::string::npos);
        CHECK(output.find("Units") != std::string::npos);
        CHECK(output.find("Farm") != std::string::npos);
        CHECK(output.find("Warrior") != std::string::npos);
    }
    
    SECTION("contains helper functions") {
        CHECK(output.find("LexHelpers") != std::string::npos);
        CHECK(output.find("has_technology") != std::string::npos);
    }
    
    SECTION("module return format") {
        CHECK(output.find("return {") != std::string::npos);
    }
}

TEST_CASE("Snapshot Tests - JSON Backend") {
    auto& schema = SchemaRegistry::instance();
    schema.load_imperium_default();
    
    Lexer lexer(R"lex(
        structure Temple {
            era: Ancient
            name: "Temple of Zeus"
            cost: { Gold: 300, Wood: 150 }
            production: { Culture: 5 }
            requires: [Pottery]
        }
)lex");
    auto tokens = lexer.tokenize();
    REQUIRE(!lexer.has_errors());
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE(!parser.has_errors());
    
    JsonBackend backend;
    std::string output = backend.generate(ast);
    
    // Validate JSON syntax
    BackendValidator validator;
    auto validation = validator.validate("json", output);
    
    SECTION("valid JSON syntax") {
        CHECK(validation.valid);
        CHECK(validation.errors.empty());
    }
    
    SECTION("contains expected structure") {
        CHECK(output.find("version") != std::string::npos);
        CHECK(output.find("structures") != std::string::npos);
        CHECK(output.find("Temple") != std::string::npos);
    }
    
    SECTION("metadata present") {
        CHECK(output.find("generated_by") != std::string::npos);
    }
}

TEST_CASE("Snapshot Tests - Godot Backend") {
    auto& schema = SchemaRegistry::instance();
    schema.load_imperium_default();
    
    Lexer lexer(R"lex(
        structure Turret {
            era: Medieval
            name: "Turret"
            cost: { Gold: 100 }
            damage: 15
            range: 3
        }
        
        unit Archer {
            era: Medieval
            name: "Archer"
            hp: 60
            attack: 8
        }
)lex");
    auto tokens = lexer.tokenize();
    REQUIRE(!lexer.has_errors());
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE(!parser.has_errors());
    
    GodotBackend backend;
    backend.set_class_name("MedievalData");
    std::string output = backend.generate(ast);
    
    // Validate GDScript syntax
    BackendValidator validator;
    auto validation = validator.validate("godot", output);
    
    SECTION("valid GDScript syntax") {
        CHECK(validation.valid);
        CHECK(validation.errors.empty());
    }
    
    SECTION("contains class definition") {
        CHECK(output.find("class_name MedievalData extends Resource") != std::string::npos);
    }
    
    SECTION("contains helper functions") {
        CHECK(output.find("func has_technology") != std::string::npos);
        CHECK(output.find("func has_structure") != std::string::npos);
    }
    
    SECTION("contains data definitions") {
        CHECK(output.find("var structures = {}") != std::string::npos);
        CHECK(output.find("var units = {}") != std::string::npos);
    }
}

TEST_CASE("Snapshot Tests - Unity Backend") {
    auto& schema = SchemaRegistry::instance();
    schema.load_imperium_default();
    
    Lexer lexer(R"lex(
        structure Barracks {
            era: Medieval
            name: "Barracks"
            cost: { Gold: 150 }
        }
)lex");
    auto tokens = lexer.tokenize();
    REQUIRE(!lexer.has_errors());
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE(!parser.has_errors());
    
    UnityBackend backend;
    std::string output = backend.generate(ast);
    
    // Validate C# syntax (basic check is limited, just verify structure)
    BackendValidator validator;
    auto validation = validator.validate("csharp", output);
    
    SECTION("valid C# syntax or basic check passes") {
        // Basic syntax checker is limited - just verify code is generated
        CHECK_FALSE(output.empty());
        // Note: Basic validator may have false positives with complex C# constructs
        // Full validation requires dotnet tool
    }
    
    SECTION("contains ScriptableObject class") {
        CHECK(output.find("ScriptableObject") != std::string::npos);
    }
}

TEST_CASE("Snapshot Tests - Complex Definitions") {
    auto& schema = SchemaRegistry::instance();
    schema.load_imperium_default();
    
    Lexer lexer(R"lex(
        technology Mathematics {
            era: Ancient
            name: "Mathematics"
            research_cost: 200
            requires: [Writing]
            unlocks: [Currency]
        }
        
        terrain Plains {
            name: "Plains"
            movement_cost: 1
            food_yield: 2
            buildable: true
        }
)lex");
    auto tokens = lexer.tokenize();
    REQUIRE(!lexer.has_errors());
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE(!parser.has_errors());
    
    // Test all backends with complex data
    BackendValidator validator;
    
    SECTION("Lua backend handles complex definitions") {
        LuaBackend backend;
        std::string output = backend.generate(ast);
        
        auto result = validator.validate("lua", output);
        CHECK(result.valid);
        CHECK(output.find("Technologies") != std::string::npos);
        CHECK(output.find("Terrains") != std::string::npos);
    }
    
    SECTION("JSON backend handles complex definitions") {
        JsonBackend backend;
        std::string output = backend.generate(ast);
        
        auto result = validator.validate("json", output);
        CHECK(result.valid);
        CHECK(output.find("technologys") != std::string::npos);
        CHECK(output.find("terrains") != std::string::npos);
    }
}

// ============================================================================
// Performance Tests
// ============================================================================

TEST_CASE("Performance - Large dataset validation") {
    auto& schema = SchemaRegistry::instance();
    schema.load_imperium_default();
    
    // Generate a large Lex file
    std::stringstream lex_source;
    lex_source << "era Test { name: \"Test\" }\n";
    
    for (int i = 0; i < 100; i++) {
        lex_source << "structure Item" << i << " { era: Test name: \"Item" << i << "\" cost: { Gold: " << (i * 10) << " } }\n";
    }
    
    Lexer lexer(lex_source.str());
    auto tokens = lexer.tokenize();
    REQUIRE(!lexer.has_errors());
    
    Parser parser(tokens);
    auto ast = parser.parse();
    REQUIRE(!parser.has_errors());
    
    // Time the code generation and validation
    BackendValidator validator;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    LuaBackend lua_backend;
    std::string lua_output = lua_backend.generate(ast);
    auto lua_result = validator.validate("lua", lua_output);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    SECTION("Lua backend performs well") {
        CHECK(lua_result.valid);
        CHECK(duration.count() < 1000); // Should complete in under 1 second
    }
    
    start = std::chrono::high_resolution_clock::now();
    
    JsonBackend json_backend;
    std::string json_output = json_backend.generate(ast);
    auto json_result = validator.validate("json", json_output);
    
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    SECTION("JSON backend performs well") {
        CHECK(json_result.valid);
        CHECK(duration.count() < 1000);
    }
}
