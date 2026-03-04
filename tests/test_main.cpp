#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../src/lexer/lexer.h"
#include "../src/lexer/keywords.h"
#include "../src/parser/parser.h"
#include "../src/ast/ast.h"
#include "../src/schema/schema.h"

#include <regex>
#include <string>
#include <iostream>

using namespace lex;

TEST_CASE("Lexer - Keywords") {
    SECTION("era keyword") {
        Lexer lexer("era");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens.size() == 2);
        REQUIRE(tokens[0].type == TokenType::ERA);
        REQUIRE(tokens[0].lexeme == "era");
        REQUIRE(tokens[1].type == TokenType::END_OF_FILE);
    }

    
    SECTION("structure keyword") {
        Lexer lexer("structure");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::STRUCTURE);
    }
    
    SECTION("multiple keywords") {
        Lexer lexer("era structure unit technology");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::ERA);
        REQUIRE(tokens[1].type == TokenType::STRUCTURE);
        REQUIRE(tokens[2].type == TokenType::UNIT);
        REQUIRE(tokens[3].type == TokenType::TECHNOLOGY);
    }
    
    SECTION("boolean keywords") {
        Lexer lexer("true false null");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::TRUE);
        REQUIRE(std::holds_alternative<bool>(tokens[0].value));
        REQUIRE(std::get<bool>(tokens[0].value) == true);
        
        REQUIRE(tokens[1].type == TokenType::FALSE);
        REQUIRE(std::holds_alternative<bool>(tokens[1].value));
        REQUIRE(std::get<bool>(tokens[1].value) == false);
        
        REQUIRE(tokens[2].type == TokenType::NULL_LIT);
    }
}
TEST_CASE("Lexer - Identifiers") {
    SECTION("simple identifier") {
        Lexer lexer("hello");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[0].lexeme == "hello");
    }
    
    SECTION("identifier with underscore") {
        Lexer lexer("hello_world");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[0].lexeme == "hello_world");
    }
    
    SECTION("identifier starting with underscore") {
        Lexer lexer("_private");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[0].lexeme == "_private");
    }
    
    SECTION("identifier with numbers") {
        Lexer lexer("var123");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[0].lexeme == "var123");
    }
}
TEST_CASE("Lexer - Integers") {
    SECTION("simple integer") {
        Lexer lexer("123");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::INTEGER);
        REQUIRE(tokens[0].lexeme == "123");
        REQUIRE(std::holds_alternative<int64_t>(tokens[0].value));
        REQUIRE(std::get<int64_t>(tokens[0].value) == 123);
    }
    
    SECTION("zero") {
        Lexer lexer("0");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::INTEGER);
        REQUIRE(std::get<int64_t>(tokens[0].value) == 0);
    }
    
    SECTION("large integer") {
        Lexer lexer("9223372036854775807");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::INTEGER);
    }
}
TEST_CASE("Lexer - Floats") {
    SECTION("simple float") {
        Lexer lexer("3.14");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::FLOAT);
        REQUIRE(tokens[0].lexeme == "3.14");
        REQUIRE(std::holds_alternative<double>(tokens[0].value));
        REQUIRE_THAT(std::get<double>(tokens[0].value), Catch::Matchers::WithinAbs(3.14, 0.001));
    }
    
    SECTION("float with many decimals") {
        Lexer lexer("45.6789");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::FLOAT);
        REQUIRE_THAT(std::get<double>(tokens[0].value), Catch::Matchers::WithinAbs(45.6789, 0.0001));
    }
    
    SECTION("integer not confused with float") {
        Lexer lexer("42");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::INTEGER);
    }
}
TEST_CASE("Lexer - Strings") {
    SECTION("simple string") {
        Lexer lexer("\"hello\"");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::STRING);
        REQUIRE(tokens[0].lexeme == "hello");
        REQUIRE(std::holds_alternative<std::string>(tokens[0].value));
        REQUIRE(std::get<std::string>(tokens[0].value) == "hello");
    }
    
    SECTION("string with spaces") {
        Lexer lexer("\"hello world\"");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::STRING);
        REQUIRE(tokens[0].lexeme == "hello world");
    }
    
    SECTION("string with escape sequences") {
        Lexer lexer("\"line1\\nline2\"");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::STRING);
        REQUIRE(std::get<std::string>(tokens[0].value) == "line1\nline2");
    }
    
    SECTION("string with tab escape") {
        Lexer lexer("\"col1\\tcol2\"");
        auto tokens = lexer.tokenize();
        REQUIRE(std::get<std::string>(tokens[0].value) == "col1\tcol2");
    }
    
    SECTION("string with quote escape") {
        Lexer lexer("\"say \\\"hello\\\"\"");
        auto tokens = lexer.tokenize();
        REQUIRE(std::get<std::string>(tokens[0].value) == "say \"hello\"");
    }
    
    SECTION("string with backslash escape") {
        Lexer lexer("\"path\\\\to\\\\file\"");
        auto tokens = lexer.tokenize();
        REQUIRE(std::get<std::string>(tokens[0].value) == "path\\to\\file");
    }
    
    SECTION("empty string") {
        Lexer lexer("\"\"");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::STRING);
        REQUIRE(tokens[0].lexeme.empty());
    }
}
TEST_CASE("Lexer - Hex Colors") {
    SECTION("valid hex color") {
        Lexer lexer("#FF00FF");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::HEX_COLOR);
        REQUIRE(tokens[0].lexeme == "#FF00FF");
        REQUIRE(std::holds_alternative<std::string>(tokens[0].value));
        REQUIRE(std::get<std::string>(tokens[0].value) == "#FF00FF");
    }
    
    SECTION("hex color lowercase") {
        Lexer lexer("#aabbcc");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::HEX_COLOR);
        REQUIRE(tokens[0].lexeme == "#aabbcc");
    }
    
    SECTION("hex color mixed case") {
        Lexer lexer("#AbCdEf");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::HEX_COLOR);
        REQUIRE(tokens[0].lexeme == "#AbCdEf");
    }
    
    SECTION("hex color black") {
        Lexer lexer("#000000");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::HEX_COLOR);
        REQUIRE(tokens[0].lexeme == "#000000");
    }
    
    SECTION("hex color white") {
        Lexer lexer("#FFFFFF");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::HEX_COLOR);
        REQUIRE(tokens[0].lexeme == "#FFFFFF");
    }
    
    SECTION("invalid hex color - too short") {
        Lexer lexer("#FFF");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::ERROR);
        REQUIRE(lexer.has_errors());
    }
}
TEST_CASE("Lexer - Operators") {
    SECTION("equality") {
        Lexer lexer("==");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::EQ);
        REQUIRE(tokens[0].lexeme == "==");
    }
    
    SECTION("inequality") {
        Lexer lexer("!=");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::NE);
        REQUIRE(tokens[0].lexeme == "!=");
    }
    
    SECTION("greater than or equal") {
        Lexer lexer(">=");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::GE);
        REQUIRE(tokens[0].lexeme == ">=");
    }
    
    SECTION("less than or equal") {
        Lexer lexer("<=");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::LE);
        REQUIRE(tokens[0].lexeme == "<=");
    }
    
    SECTION("plus assign") {
        Lexer lexer("+=");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::PLUS_ASSIGN);
        REQUIRE(tokens[0].lexeme == "+=");
    }
    
    SECTION("minus assign") {
        Lexer lexer("-=");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::MINUS_ASSIGN);
        REQUIRE(tokens[0].lexeme == "-=");
    }
    
    SECTION("arrow") {
        Lexer lexer("->");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::ARROW);
        REQUIRE(tokens[0].lexeme == "->");
    }
    
    SECTION("range") {
        Lexer lexer("..");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::RANGE);
        REQUIRE(tokens[0].lexeme == "..");
    }
    
    SECTION("single char operators") {
        Lexer lexer("+ - * / % = > <");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::PLUS);
        REQUIRE(tokens[1].type == TokenType::MINUS);
        REQUIRE(tokens[2].type == TokenType::STAR);
        REQUIRE(tokens[3].type == TokenType::SLASH);
        REQUIRE(tokens[4].type == TokenType::PERCENT);
        REQUIRE(tokens[5].type == TokenType::ASSIGN);
        REQUIRE(tokens[6].type == TokenType::GT);
        REQUIRE(tokens[7].type == TokenType::LT);
    }
}
TEST_CASE("Lexer - Punctuation") {
    SECTION("braces") {
        Lexer lexer("{ }");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::LEFT_BRACE);
        REQUIRE(tokens[1].type == TokenType::RIGHT_BRACE);
    }
    
    SECTION("brackets") {
        Lexer lexer("[ ]");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::LEFT_BRACKET);
        REQUIRE(tokens[1].type == TokenType::RIGHT_BRACKET);
    }
    
    SECTION("parentheses") {
        Lexer lexer("( )");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::LEFT_PAREN);
        REQUIRE(tokens[1].type == TokenType::RIGHT_PAREN);
    }
    
    SECTION("other punctuation") {
        Lexer lexer(": , . ;");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::COLON);
        REQUIRE(tokens[1].type == TokenType::COMMA);
        REQUIRE(tokens[2].type == TokenType::DOT);
        REQUIRE(tokens[3].type == TokenType::SEMICOLON);
    }
}
TEST_CASE("Lexer - Comments") {
    SECTION("single line comment") {
        Lexer lexer("x // comment\ny");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[0].lexeme == "x");
        REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[1].lexeme == "y");
    }
    
    SECTION("multi line comment") {
        Lexer lexer("x /* comment */ y");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0].lexeme == "x");
        REQUIRE(tokens[1].lexeme == "y");
    }
    
    SECTION("multi line comment across lines") {
        Lexer lexer("x /* line1\nline2 */ y");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens.size() == 3);
        REQUIRE(tokens[0].lexeme == "x");
        REQUIRE(tokens[1].lexeme == "y");
    }
}
TEST_CASE("Lexer - Multiple Tokens") {
    SECTION("simple expression") {
        Lexer lexer("x = 42");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens.size() == 4);
        REQUIRE(tokens[0].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[1].type == TokenType::ASSIGN);
        REQUIRE(tokens[2].type == TokenType::INTEGER);
        REQUIRE(tokens[3].type == TokenType::END_OF_FILE);
    }
    
    SECTION("function-like syntax") {
        Lexer lexer("name: \"Test\"");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::NAME);
        REQUIRE(tokens[1].type == TokenType::COLON);
        REQUIRE(tokens[2].type == TokenType::STRING);
        REQUIRE(tokens[2].lexeme == "Test");
    }
    
    SECTION("complex statement") {
        Lexer lexer("if (x == 10) { y = y + 1 }");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::IF);
        REQUIRE(tokens[1].type == TokenType::LEFT_PAREN);
        REQUIRE(tokens[2].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[3].type == TokenType::EQ);
        REQUIRE(tokens[4].type == TokenType::INTEGER);
        REQUIRE(tokens[5].type == TokenType::RIGHT_PAREN);
        REQUIRE(tokens[6].type == TokenType::LEFT_BRACE);
    }
    
    SECTION("structure definition") {
        Lexer lexer("structure Knight { attack: 10 }");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::STRUCTURE);
        REQUIRE(tokens[1].type == TokenType::IDENTIFIER);
        REQUIRE(tokens[1].lexeme == "Knight");
        REQUIRE(tokens[2].type == TokenType::LEFT_BRACE);
        REQUIRE(tokens[3].type == TokenType::ATTACK);
        REQUIRE(tokens[4].type == TokenType::COLON);
        REQUIRE(tokens[5].type == TokenType::INTEGER);
        REQUIRE(std::get<int64_t>(tokens[5].value) == 10);
    }
}
TEST_CASE("Lexer - Location Tracking") {
    SECTION("line tracking") {
        Lexer lexer("x\ny\nz");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].location.line == 1);
        REQUIRE(tokens[1].location.line == 2);
        REQUIRE(tokens[2].location.line == 3);
    }
    
    SECTION("column tracking") {
        Lexer lexer("x y z");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].location.column == 1);
        REQUIRE(tokens[1].location.column == 3);
        REQUIRE(tokens[2].location.column == 5);
    }
}
TEST_CASE("Lexer - Edge Cases") {
    SECTION("empty input") {
        Lexer lexer("");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0].type == TokenType::END_OF_FILE);
    }
    
    SECTION("only whitespace") {
        Lexer lexer("   \t\n  ");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens.size() == 1);
        REQUIRE(tokens[0].type == TokenType::END_OF_FILE);
    }
    
    SECTION("unterminated string") {
        Lexer lexer("\"hello");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::ERROR);
        REQUIRE(lexer.has_errors());
    }
    
    SECTION("unexpected character") {
        Lexer lexer("@");
        auto tokens = lexer.tokenize();
        REQUIRE(tokens[0].type == TokenType::ERROR);
        REQUIRE(lexer.has_errors());
    }
}
TEST_CASE("Lexer - Next and Peek") {
    SECTION("next_token advances") {
        Lexer lexer("a b c");
        Token t1 = lexer.next_token();
        Token t2 = lexer.next_token();
        REQUIRE(t1.lexeme == "a");
        REQUIRE(t2.lexeme == "b");
    }
    
    SECTION("peek_token does not advance") {
        Lexer lexer("a b");
        Token t1 = lexer.peek_token();
        Token t2 = lexer.peek_token();
        REQUIRE(t1.lexeme == "a");
        REQUIRE(t2.lexeme == "a");
    }
    
    SECTION("peek then next") {
        Lexer lexer("a b");
        Token t1 = lexer.peek_token();
        Token t2 = lexer.next_token();
        Token t3 = lexer.peek_token();
        REQUIRE(t1.lexeme == "a");
        REQUIRE(t2.lexeme == "a");
        REQUIRE(t3.lexeme == "b");
    }
}

// ============================================================================
// Parser Tests
// ============================================================================

TEST_CASE("Parser - Basic Structure") {
    SECTION("simple structure definition") {
        Lexer lexer("structure Farm { era: Ancient }");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());
        
        Parser parser(tokens);
        auto ast = parser.parse();
        
        REQUIRE(!parser.has_errors());
        REQUIRE(ast.size() == 1);
        REQUIRE(ast[0]->definition_type == "structure");
        REQUIRE(ast[0]->identifier == "Farm");
    }
}

TEST_CASE("Parser - Different Definition Types") {
    SECTION("era definition") {
        Lexer lexer(R"(era Ancient { period: "3000 BC" })");
        auto tokens = lexer.tokenize();
        
        Parser parser(tokens);
        auto ast = parser.parse();
        
        REQUIRE(!parser.has_errors());
        REQUIRE(ast.size() == 1);
        REQUIRE(ast[0]->definition_type == "era");
        REQUIRE(ast[0]->identifier == "Ancient");
    }
    
    SECTION("unit definition") {
        Lexer lexer(R"(unit Knight { era: Medieval attack: 10 })");
        auto tokens = lexer.tokenize();
        
        Parser parser(tokens);
        auto ast = parser.parse();
        
        REQUIRE(!parser.has_errors());
        REQUIRE(ast.size() == 1);
        REQUIRE(ast[0]->definition_type == "unit");
    }
}

TEST_CASE("Parser - Multiple Definitions") {
    SECTION("two structures") {
        Lexer lexer(R"(
            structure Farm { era: Ancient }
            structure Mine { era: Medieval }
        )");
        auto tokens = lexer.tokenize();
        
        Parser parser(tokens);
        auto ast = parser.parse();
        
        REQUIRE(!parser.has_errors());
        REQUIRE(ast.size() == 2);
        REQUIRE(ast[0]->identifier == "Farm");
        REQUIRE(ast[1]->identifier == "Mine");
    }
}

TEST_CASE("Parser - Error Recovery") {
    SECTION("missing brace") {
        Lexer lexer("structure Farm { era: Ancient");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(parser.has_errors());
    }
}

// ============================================================================
// Expression Parser Tests
// ============================================================================

TEST_CASE("Expression Parser - Arithmetic") {
    SECTION("simple addition in property") {
        Lexer lexer(R"(structure Test { cost: 10 + 5 })");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());
        REQUIRE(ast.size() == 1);

        // Check that we parsed a property with a binary expression
        auto& props = ast[0]->properties;
        REQUIRE(props.size() == 1);
        REQUIRE(props[0]->name == "cost");

        auto& expr = props[0]->value->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);
        REQUIRE(expr->binary_op == Expression::BinaryOp::ADD);
    }

    SECTION("operator precedence - mul before add") {
        Lexer lexer(R"(structure Test { cost: 10 + 5 * 2 })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->properties[0]->value->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);
        REQUIRE(expr->binary_op == Expression::BinaryOp::ADD);

        // Left should be 10, right should be (5 * 2)
        REQUIRE(expr->left->type == Expression::Type::INTEGER);
        REQUIRE(std::get<int64_t>(expr->left->value) == 10);

        REQUIRE(expr->right->type == Expression::Type::BINARY);
        REQUIRE(expr->right->binary_op == Expression::BinaryOp::MUL);
    }

    SECTION("parentheses override precedence") {
        Lexer lexer(R"(structure Test { cost: (10 + 5) * 2 })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->properties[0]->value->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);
        REQUIRE(expr->binary_op == Expression::BinaryOp::MUL);

        // Left should be (10 + 5), right should be 2
        REQUIRE(expr->left->type == Expression::Type::BINARY);
        REQUIRE(expr->left->binary_op == Expression::BinaryOp::ADD);

        REQUIRE(expr->right->type == Expression::Type::INTEGER);
        REQUIRE(std::get<int64_t>(expr->right->value) == 2);
    }
}

TEST_CASE("Expression Parser - Comparisons") {
    SECTION("greater than") {
        Lexer lexer(R"(structure Test { available_if population > 100 {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());
        REQUIRE(ast[0]->conditions.size() == 1);

        auto& cond = ast[0]->conditions[0];
        REQUIRE(cond->trigger == "available_if");
        REQUIRE(cond->expression->type == Expression::Type::BINARY);
        REQUIRE(cond->expression->binary_op == Expression::BinaryOp::GT);
    }

    SECTION("equality") {
        Lexer lexer(R"(structure Test { available_if era == Steampunk {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);
        REQUIRE(expr->binary_op == Expression::BinaryOp::EQ);
    }

    SECTION("less than or equal") {
        Lexer lexer(R"(structure Test { available_if happiness <= 30 {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);
        REQUIRE(expr->binary_op == Expression::BinaryOp::LE);
    }
}

TEST_CASE("Expression Parser - Logical Operators") {
    SECTION("and operator") {
        Lexer lexer(R"(structure Test { available_if population > 100 and happiness > 50 {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        if (parser.has_errors()) {
            for (const auto& err : parser.errors()) {
                UNSCOPED_INFO("Parser error: " << err);
            }
        }
        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);
        REQUIRE(expr->binary_op == Expression::BinaryOp::AND);
    }

    SECTION("or operator") {
        Lexer lexer(R"(structure Test { available_if has_tech or has_gold {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);
        REQUIRE(expr->binary_op == Expression::BinaryOp::OR);
    }

    SECTION("not operator") {
        Lexer lexer(R"(structure Test { available_if not is_raining {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::UNARY);
        REQUIRE(expr->unary_op == Expression::UnaryOp::NOT);
    }

    SECTION("complex logical expression") {
        Lexer lexer(R"(structure Test { available_if a > 5 and b < 10 or c == 0 {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        // OR should be at the top (lowest precedence)
        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);
        REQUIRE(expr->binary_op == Expression::BinaryOp::OR);

        // Left side should be AND
        REQUIRE(expr->left->type == Expression::Type::BINARY);
        REQUIRE(expr->left->binary_op == Expression::BinaryOp::AND);
    }
}

TEST_CASE("Expression Parser - Member Access") {
    SECTION("simple member access") {
        Lexer lexer(R"(structure Test { available_if city.population > 100 {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);
        REQUIRE(expr->binary_op == Expression::BinaryOp::GT);

        // Left should be member access
        REQUIRE(expr->left->type == Expression::Type::MEMBER);
        REQUIRE(expr->left->member_name == "population");
        REQUIRE(expr->left->object->type == Expression::Type::REFERENCE);
        REQUIRE(expr->left->object->reference == "city");
    }

    SECTION("chained member access") {
        Lexer lexer(R"(structure Test { available_if civilization.cities.count > 5 {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);

        // Navigate the chain: civilization.cities.count
        auto& left = expr->left;
        REQUIRE(left->type == Expression::Type::MEMBER);
        REQUIRE(left->member_name == "count");

        REQUIRE(left->object->type == Expression::Type::MEMBER);
        REQUIRE(left->object->member_name == "cities");

        REQUIRE(left->object->object->type == Expression::Type::REFERENCE);
        REQUIRE(left->object->object->reference == "civilization");
    }
}

TEST_CASE("Expression Parser - Function Calls") {
    SECTION("simple function call") {
        Lexer lexer(R"(structure Test { available_if has_technology(SteamEngine) {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::CALL);
        REQUIRE(expr->function_name == "has_technology");
        REQUIRE(expr->arguments.size() == 1);
        REQUIRE(expr->arguments[0]->type == Expression::Type::REFERENCE);
        REQUIRE(expr->arguments[0]->reference == "SteamEngine");
    }

    SECTION("function call with multiple arguments") {
        Lexer lexer(R"(structure Test { available_if count_units(Warrior, Knight) > 5 {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);

        // Left is the call
        auto& call = expr->left;
        REQUIRE(call->type == Expression::Type::CALL);
        REQUIRE(call->function_name == "count_units");
        REQUIRE(call->arguments.size() == 2);
    }

    SECTION("function call with expression argument") {
        Lexer lexer(R"(structure Test { available_if calculate(a + b) {} })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->conditions[0]->expression;
        REQUIRE(expr->type == Expression::Type::CALL);

        auto& arg = expr->arguments[0];
        REQUIRE(arg->type == Expression::Type::BINARY);
        REQUIRE(arg->binary_op == Expression::BinaryOp::ADD);
    }
}

TEST_CASE("Expression Parser - Unary Negation") {
    SECTION("negative number in property") {
        Lexer lexer(R"(structure Test { cost: -50 })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->properties[0]->value->expression;
        REQUIRE(expr->type == Expression::Type::UNARY);
        REQUIRE(expr->unary_op == Expression::UnaryOp::NEG);
        REQUIRE(expr->operand->type == Expression::Type::INTEGER);
        REQUIRE(std::get<int64_t>(expr->operand->value) == 50);
    }

    SECTION("negative in expression") {
        Lexer lexer(R"(structure Test { cost: 100 + -50 })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        REQUIRE(!parser.has_errors());

        auto& expr = ast[0]->properties[0]->value->expression;
        REQUIRE(expr->type == Expression::Type::BINARY);

        // Right side should be unary negation
        REQUIRE(expr->right->type == Expression::Type::UNARY);
        REQUIRE(expr->right->unary_op == Expression::UnaryOp::NEG);
    }
}

// ============================================================================
// Codegen Backend Tests
// ============================================================================

#include "../src/codegen/lua_backend.h"
#include "../src/codegen/json_backend.h"

TEST_CASE("Lua Backend - Basic Output") {
    SECTION("simple structure") {
        Lexer lexer(R"(structure Farm { era: Ancient cost: { Gold: 50 } })");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());

        Parser parser(tokens);
        auto ast = parser.parse();
        REQUIRE(!parser.has_errors());

        LuaBackend backend;
        std::string output = backend.generate(ast);

        // Check table name
        REQUIRE(output.find("Structures") != std::string::npos);
        REQUIRE(output.find("Structures.Farm") != std::string::npos);

        // Check properties
        REQUIRE(output.find("era = \"Ancient\"") != std::string::npos);
        REQUIRE(output.find("Gold = 50") != std::string::npos);
    }

    SECTION("multiple definition types") {
        Lexer lexer(R"(
            era Ancient { period: "3000 BC" }
            structure Farm { era: Ancient }
        )");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());

        Parser parser(tokens);
        auto ast = parser.parse();
        REQUIRE(!parser.has_errors());

        LuaBackend backend;
        std::string output = backend.generate(ast);

        // Both tables should be generated
        REQUIRE(output.find("Eras") != std::string::npos);
        REQUIRE(output.find("Structures") != std::string::npos);
        REQUIRE(output.find("Eras.Ancient") != std::string::npos);
        REQUIRE(output.find("Structures.Farm") != std::string::npos);
    }

    SECTION("module return") {
        Lexer lexer(R"(structure Farm { era: Ancient })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        LuaBackend backend;
        std::string output = backend.generate(ast);

        // Should return module for require()
        REQUIRE(output.find("return {") != std::string::npos);
        REQUIRE(output.find("LexHelpers") != std::string::npos);
    }
}

TEST_CASE("JSON Backend - Basic Output") {
    SECTION("simple structure") {
        Lexer lexer(R"(structure Farm { era: Ancient cost: { Gold: 50 } })");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());

        Parser parser(tokens);
        auto ast = parser.parse();
        REQUIRE(!parser.has_errors());

        JsonBackend backend;
        std::string output = backend.generate(ast);

        // Check JSON structure
        REQUIRE(output.find("\"structures\":") != std::string::npos);
        REQUIRE(output.find("\"Farm\":") != std::string::npos);
        REQUIRE(output.find("\"id\": \"Farm\"") != std::string::npos);
        REQUIRE(output.find("\"type\": \"structure\"") != std::string::npos);
        REQUIRE(output.find("\"era\": \"Ancient\"") != std::string::npos);
        REQUIRE(output.find("\"cost\":") != std::string::npos);
        REQUIRE(output.find("\"Gold\": 50") != std::string::npos);
    }

    SECTION("version info") {
        Lexer lexer(R"(structure Farm { era: Ancient })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        JsonBackend backend;
        std::string output = backend.generate(ast);

        REQUIRE(output.find("\"version\": \"1.0\"") != std::string::npos);
        REQUIRE(output.find("\"generated_by\": \"Lex Compiler") != std::string::npos);
    }

    SECTION("multiple definition types") {
        Lexer lexer(R"(
            era Ancient { period: "3000 BC" }
            structure Farm { era: Ancient }
        )");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();

        JsonBackend backend;
        std::string output = backend.generate(ast);

        // Both types should be present
        REQUIRE(output.find("\"eras\":") != std::string::npos);
        REQUIRE(output.find("\"structures\":") != std::string::npos);
    }
}

TEST_CASE("Backend - Dynamic Schema") {
    SECTION("custom type via generic") {
        // Test that custom types work with generic generator
        Lexer lexer(R"(structure Farm { era: Ancient custom_prop: "test" })");
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        auto ast = parser.parse();
        REQUIRE(!parser.has_errors());

        // Both backends should handle unknown properties
        LuaBackend lua_backend;
        std::string lua_output = lua_backend.generate(ast);
        REQUIRE(lua_output.find("custom_prop") != std::string::npos);

        JsonBackend json_backend;
        std::string json_output = json_backend.generate(ast);
        REQUIRE(json_output.find("custom_prop") != std::string::npos);
    }
}

TEST_CASE("Agnostic Schema - Custom Types") {
    // Test that Lex works with any schema, not just Imperium
    SECTION("RPG schema types") {
        // Load custom schema
        auto& schema = SchemaRegistry::instance();
        schema.clear();
        schema.load_from_cli("character,item,quest");

        // Parse custom types
        Lexer lexer(R"(
            character Hero { health: 100 mana: 50 }
            item Sword { damage: 25 rarity: "legendary" }
            quest SaveVillage { reward: { Gold: 500 } }
        )");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());

        Parser parser(tokens);
        auto ast = parser.parse();
        REQUIRE(!parser.has_errors());
        REQUIRE(ast.size() == 3);

        // Check types
        REQUIRE(ast[0]->definition_type == "character");
        REQUIRE(ast[0]->identifier == "Hero");
        REQUIRE(ast[1]->definition_type == "item");
        REQUIRE(ast[1]->identifier == "Sword");
        REQUIRE(ast[2]->definition_type == "quest");
        REQUIRE(ast[2]->identifier == "SaveVillage");

        // Generate JSON
        JsonBackend json_backend;
        std::string json_output = json_backend.generate(ast);

        REQUIRE(json_output.find("\"characters\":") != std::string::npos);
        REQUIRE(json_output.find("\"items\":") != std::string::npos);
        REQUIRE(json_output.find("\"quests\":") != std::string::npos);
        REQUIRE(json_output.find("\"health\": 100") != std::string::npos);
        REQUIRE(json_output.find("\"damage\": 25") != std::string::npos);

        // Generate Lua
        LuaBackend lua_backend;
        std::string lua_output = lua_backend.generate(ast);

        REQUIRE(lua_output.find("Characters") != std::string::npos);
        REQUIRE(lua_output.find("Items") != std::string::npos);
        REQUIRE(lua_output.find("Quests") != std::string::npos);
        REQUIRE(lua_output.find("health = 100") != std::string::npos);

        // Restore default schema
        schema.load_imperium_default();
    }
}

TEST_CASE("Module System - Visibility") {
    SECTION("public and internal keywords") {
        Lexer lexer(R"(
            public structure Hero { era: Ancient }
            internal GameState { turn: 1 }
            private helper Helper { value: 42 }
        )");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());

        // Check keywords are recognized
        REQUIRE(tokens[0].type == TokenType::PUBLIC);
        REQUIRE(tokens[1].type == TokenType::STRUCTURE);
        REQUIRE(tokens[2].lexeme == "Hero");
    }

    SECTION("module declaration") {
        Lexer lexer(R"(module engine.internal)");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());

        REQUIRE(tokens[0].type == TokenType::MODULE);
        REQUIRE(tokens[1].lexeme == "engine");
        REQUIRE(tokens[2].type == TokenType::DOT);
        REQUIRE(tokens[3].lexeme == "internal");
    }

    SECTION("use statement") {
        Lexer lexer(R"(use "characters.lex")");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());

        REQUIRE(tokens[0].type == TokenType::USE);
        REQUIRE(tokens[1].type == TokenType::STRING);
    }

    SECTION("full file with modules") {
        // Load schema first
        auto& schema = SchemaRegistry::instance();
        schema.load_imperium_default();

        Lexer lexer(R"(
            module game.public
            use "items.lex"

            public structure Hero { era: Ancient }
        )");
        auto tokens = lexer.tokenize();
        REQUIRE(!lexer.has_errors());

        Parser parser(tokens);
        auto file = parser.parse_file();
        if (parser.has_errors()) {
            for (const auto& err : parser.errors()) {
                std::cout << "Parser error: " << err << std::endl;
            }
        }
        REQUIRE(!parser.has_errors());

        REQUIRE(file.module_name == "game.public");
        REQUIRE(file.imports.size() == 1);
        REQUIRE(file.imports[0]->path == "items.lex");
        REQUIRE(file.definitions.size() == 1);
        REQUIRE(file.definitions[0]->identifier == "Hero");
        REQUIRE(file.definitions[0]->visibility == Visibility::PUBLIC);
    }
}

