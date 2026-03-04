#include "parser.h"
#include "../schema/schema.h"
#include <sstream>
#include <stdexcept>

namespace lex {

Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens) {}

// ============================================================================
// Core Parsing
// ============================================================================

std::vector<std::unique_ptr<Definition>> Parser::parse() {
    std::vector<std::unique_ptr<Definition>> definitions;

    while (pos_ < tokens_.size() && current().type != TokenType::END_OF_FILE) {
        auto def = parse_definition();
        if (def) {
            definitions.push_back(std::move(def));
        } else {
            // Skip to next definition on error
            skip_to_next_definition();
        }
    }

    return definitions;
}

Token& Parser::current() {
    return tokens_[pos_];
}

Token& Parser::peek(int offset) {
    size_t idx = pos_ + offset;
    return idx < tokens_.size() ? tokens_[idx] : tokens_.back();
}

bool Parser::check(TokenType type) {
    return current().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        pos_++;
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        return tokens_[pos_++];
    }
    error(message);
    return current();
}

void Parser::error(const std::string& message) {
    std::stringstream ss;
    ss << "Line " << current().location.line << ", Column " << current().location.column;
    ss << ": " << message;
    ss << " (found '" << current().lexeme << "')";
    errors_.push_back(ss.str());
}

void Parser::skip_to_next_definition() {
    // Skip tokens until we find a definition keyword or EOF
    while (pos_ < tokens_.size() && current().type != TokenType::END_OF_FILE) {
        if (is_definition_keyword(current().type)) {
            return;
        }
        pos_++;
    }
}

bool Parser::is_definition_keyword(TokenType type) {
    return type == TokenType::ERA ||
           type == TokenType::STRUCTURE ||
           type == TokenType::UNIT ||
           type == TokenType::TECHNOLOGY ||
           type == TokenType::RESOURCE ||
           type == TokenType::CHOICE ||
           type == TokenType::ENDING ||
           type == TokenType::EVENT ||
           type == TokenType::SECRET ||
           type == TokenType::TERRAIN;
}

// ============================================================================
// Definition Parsing
// ============================================================================

std::unique_ptr<Definition> Parser::parse_definition() {
    TokenType type = current().type;

    // Map token type to definition type string
    std::string type_name;

    switch (type) {
        case TokenType::ERA:         type_name = "era"; break;
        case TokenType::STRUCTURE:   type_name = "structure"; break;
        case TokenType::UNIT:        type_name = "unit"; break;
        case TokenType::TECHNOLOGY:  type_name = "technology"; break;
        case TokenType::RESOURCE:    type_name = "resource"; break;
        case TokenType::CHOICE:      type_name = "choice"; break;
        case TokenType::ENDING:      type_name = "ending"; break;
        case TokenType::EVENT:       type_name = "event"; break;
        case TokenType::SECRET:      type_name = "secret"; break;
        case TokenType::TERRAIN:     type_name = "terrain"; break;
        case TokenType::IDENTIFIER: {
            // Support for custom definition types from schema
            auto& schema = SchemaRegistry::instance();
            type_name = current().lexeme;
            if (schema.is_valid_definition(type_name)) {
                pos_++;  // Consume the identifier
                return parse_generic_definition(type_name);
            }
            error("Unknown definition type: " + type_name);
            return nullptr;
        }
        default:
            error("Expected definition keyword");
            return nullptr;
    }

    pos_++;  // Consume the keyword token
    return parse_generic_definition(type_name);
}

// ============================================================================
// Property Parsing
// ============================================================================

bool Parser::is_condition_keyword(TokenType type) {
    return type == TokenType::WHEN ||
           type == TokenType::IF ||
           type == TokenType::AVAILABLE_IF ||
           type == TokenType::SECRET_IF ||
           type == TokenType::ACTIVE_IF ||
           type == TokenType::BONUS_IF;
}

bool Parser::is_property_keyword(TokenType type) {
    // Most keywords can be property names
    return type == TokenType::IDENTIFIER ||
           type == TokenType::ERA ||
           type == TokenType::NAME ||
           type == TokenType::DESCRIPTION ||
           type == TokenType::COST ||
           type == TokenType::PRODUCTION ||
           type == TokenType::MAINTENANCE ||
           type == TokenType::REQUIRES ||
           type == TokenType::UNLOCKS ||
           type == TokenType::TYPE ||
           type == TokenType::CATEGORY ||
           type == TokenType::LEVEL ||
           type == TokenType::QUOTE ||
           type == TokenType::PERIOD ||
           type == TokenType::MUSIC ||
           type == TokenType::DOMINANT_COLOR ||
           type == TokenType::ATMOSPHERE ||
           type == TokenType::RESEARCH_COST ||
           type == TokenType::PREREQUISITES ||
           type == TokenType::ICON ||
           type == TokenType::COLOR ||
           type == TokenType::TOOLTIP ||
           type == TokenType::CONTEXT ||
           type == TokenType::REFERENCES ||
           type == TokenType::TEXT_CONTENT ||
           type == TokenType::EFFECT ||
           type == TokenType::ARCHIVIST ||
           type == TokenType::TRIGGER ||
           type == TokenType::CONDITION ||
           type == TokenType::ATTACK ||
           type == TokenType::DEFENSE ||
           type == TokenType::MOVEMENT ||
           type == TokenType::RANGE ||
           type == TokenType::POPULATION ||
           type == TokenType::HEALTH ||
           type == TokenType::HAPPINESS ||
           type == TokenType::STABILITY;
}

std::string Parser::property_token_to_string(TokenType type) {
    // Convert token type to property name string
    switch (type) {
        case TokenType::ERA: return "era";
        case TokenType::NAME: return "name";
        case TokenType::DESCRIPTION: return "description";
        case TokenType::COST: return "cost";
        case TokenType::PRODUCTION: return "production";
        case TokenType::MAINTENANCE: return "maintenance";
        case TokenType::REQUIRES: return "requires";
        case TokenType::UNLOCKS: return "unlocks";
        case TokenType::TYPE: return "type";
        case TokenType::CATEGORY: return "category";
        case TokenType::LEVEL: return "level";
        case TokenType::QUOTE: return "quote";
        case TokenType::PERIOD: return "period";
        case TokenType::MUSIC: return "music";
        case TokenType::DOMINANT_COLOR: return "dominant_color";
        case TokenType::ATMOSPHERE: return "atmosphere";
        case TokenType::RESEARCH_COST: return "research_cost";
        case TokenType::PREREQUISITES: return "prerequisites";
        case TokenType::ICON: return "icon";
        case TokenType::COLOR: return "color";
        case TokenType::TOOLTIP: return "tooltip";
        case TokenType::CONTEXT: return "context";
        case TokenType::REFERENCES: return "references";
        case TokenType::TEXT_CONTENT: return "text";
        case TokenType::EFFECT: return "effect";
        case TokenType::ARCHIVIST: return "archivist";
        case TokenType::TRIGGER: return "trigger";
        case TokenType::CONDITION: return "condition";
        case TokenType::ATTACK: return "attack";
        case TokenType::DEFENSE: return "defense";
        case TokenType::MOVEMENT: return "movement";
        case TokenType::RANGE: return "range";
        case TokenType::POPULATION: return "population";
        case TokenType::HEALTH: return "health";
        case TokenType::HAPPINESS: return "happiness";
        case TokenType::STABILITY: return "stability";
        default: return "";
    }
}

std::unique_ptr<Property> Parser::parse_property() {
    auto prop = std::make_unique<Property>();
    
    // Get property name
    std::string name;
    if (check(TokenType::IDENTIFIER)) {
        name = current().lexeme;
        pos_++;
    } else if (is_property_keyword(current().type)) {
        name = property_token_to_string(current().type);
        if (name.empty()) name = current().lexeme;
        pos_++;
    } else {
        error("Expected property name");
        return nullptr;
    }
    
    prop->name = name;
    
    consume(TokenType::COLON, "Expected ':' after property name");
    
    // Parse value (could be expression, resource map, or reference list)
    prop->value = parse_property_value();
    
    return prop;
}

std::unique_ptr<PropertyValue> Parser::parse_property_value() {
    auto value = std::make_unique<PropertyValue>();
    
    if (check(TokenType::LEFT_BRACKET)) {
        // Could be resource map [Gold x50, Wood x20] or reference list [TechA, TechB]
        return parse_bracket_value();
    } else if (check(TokenType::LEFT_BRACE)) {
        // Resource map with braces: { Gold: 50, Wood: 20 }
        return parse_brace_value();
    } else {
        // Simple expression
        value->type = PropertyValue::Type::EXPRESSION;
        value->expression = parse_expression();
        return value;
    }
}

std::unique_ptr<PropertyValue> Parser::parse_bracket_value() {
    auto value = std::make_unique<PropertyValue>();
    
    consume(TokenType::LEFT_BRACKET, "Expected '['");
    
    if (check(TokenType::RIGHT_BRACKET)) {
        // Empty list
        pos_++;
        value->type = PropertyValue::Type::REFERENCE_LIST;
        value->reference_list = std::make_unique<ReferenceList>();
        return value;
    }
    
    // Peek ahead to determine type
    // Resource map: [Identifier x Number, ...]
    // Reference list: [Identifier, Identifier, ...]
    
    bool is_resource_map = false;
    if (check(TokenType::IDENTIFIER)) {
        // Look ahead for 'x' pattern (Resource x Quantity)
        size_t lookahead = pos_ + 1;
        if (lookahead < tokens_.size() && 
            (tokens_[lookahead].type == TokenType::IDENTIFIER || 
             tokens_[lookahead].type == TokenType::STAR)) {
            // Could be "Resource x Quantity" or "x" as identifier
            // Check if it's the 'x' pattern
            if (tokens_[lookahead].lexeme == "x" || tokens_[lookahead].type == TokenType::STAR) {
                is_resource_map = true;
            }
        }
    }
    
    if (is_resource_map) {
        value->type = PropertyValue::Type::RESOURCE_MAP;
        value->resource_map = parse_resource_map_content();
    } else {
        value->type = PropertyValue::Type::REFERENCE_LIST;
        value->reference_list = parse_reference_list_content();
    }
    
    consume(TokenType::RIGHT_BRACKET, "Expected ']'");
    
    return value;
}

std::unique_ptr<PropertyValue> Parser::parse_brace_value() {
    auto value = std::make_unique<PropertyValue>();
    value->type = PropertyValue::Type::RESOURCE_MAP;
    
    consume(TokenType::LEFT_BRACE, "Expected '{'");
    
    auto map = std::make_unique<ResourceMap>();
    
    // Parse: Resource: Quantity, Resource: Quantity, ...
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected resource name");
            pos_++;  // Skip bad token to avoid infinite loop
            break;
        }
        
        std::string resource = current().lexeme;
        pos_++;
        
        // Expect colon
        if (!check(TokenType::COLON)) {
            error("Expected ':' after resource name");
            pos_++;
            break;
        }
        pos_++;  // Skip colon
        
        // Expect value - could be integer, string, or nested structure
        if (check(TokenType::INTEGER)) {
            int64_t quantity = std::get<int64_t>(current().value);
            pos_++;
            map->resources[resource] = quantity;
        } else if (check(TokenType::STRING)) {
            // String value - store as string resource with quantity 0
            pos_++;
            // Skip for now, could extend ResourceMap to support strings
        } else if (check(TokenType::LEFT_BRACKET)) {
            // Nested list - skip the entire bracket content
            int depth = 1;
            pos_++;  // consume '['
            while (depth > 0 && pos_ < tokens_.size()) {
                if (check(TokenType::LEFT_BRACKET)) depth++;
                else if (check(TokenType::RIGHT_BRACKET)) depth--;
                pos_++;
            }
        } else if (check(TokenType::LEFT_BRACE)) {
            // Nested object - skip the entire brace content
            int depth = 1;
            pos_++;  // consume '{'
            while (depth > 0 && pos_ < tokens_.size()) {
                if (check(TokenType::LEFT_BRACE)) depth++;
                else if (check(TokenType::RIGHT_BRACE)) depth--;
                pos_++;
            }
        } else {
            error("Expected value after ':'");
            pos_++;
            break;
        }
        
        // Optional comma
        if (check(TokenType::COMMA)) {
            pos_++;
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}'");
    
    value->resource_map = std::move(map);
    return value;
}

std::unique_ptr<ResourceMap> Parser::parse_resource_map_content() {
    auto map = std::make_unique<ResourceMap>();
    
    // Parse: Resource x Quantity, Resource x Quantity, ...
    while (!check(TokenType::RIGHT_BRACKET) && !check(TokenType::END_OF_FILE)) {
        if (!check(TokenType::IDENTIFIER)) {
            error("Expected resource name");
            break;
        }
        
        std::string resource = current().lexeme;
        pos_++;
        
        // Check for 'x' or '*'
        if (check(TokenType::IDENTIFIER) && current().lexeme == "x") {
            pos_++;  // Skip 'x'
        } else if (check(TokenType::STAR)) {
            pos_++;  // Skip '*'
        } else {
            error("Expected 'x' or '*' after resource name");
            break;
        }
        
        if (!check(TokenType::INTEGER)) {
            error("Expected quantity after 'x'");
            break;
        }
        
        int64_t quantity = std::get<int64_t>(current().value);
        pos_++;
        
        map->resources[resource] = quantity;
        
        // Optional comma
        if (check(TokenType::COMMA)) {
            pos_++;
        }
    }
    
    return map;
}

std::unique_ptr<ReferenceList> Parser::parse_reference_list_content() {
    auto list = std::make_unique<ReferenceList>();
    
    // Parse: Reference, Reference, ... (identifiers or strings)
    while (!check(TokenType::RIGHT_BRACKET) && !check(TokenType::END_OF_FILE)) {
        if (check(TokenType::IDENTIFIER)) {
            list->references.push_back(current().lexeme);
            pos_++;
        } else if (check(TokenType::STRING)) {
            list->references.push_back(std::get<std::string>(current().value));
            pos_++;
        } else {
            error("Expected reference name or string");
            pos_++;  // Skip bad token to avoid infinite loop
            break;
        }
        
        // Optional comma
        if (check(TokenType::COMMA)) {
            pos_++;
        }
    }
    
    return list;
}

// ============================================================================
// Expression Parsing (Pratt Parser with Precedence)
// ============================================================================

// Operator precedence (higher = binds tighter)
int Parser::get_operator_precedence(TokenType type) {
    switch (type) {
        // Lowest precedence
        case TokenType::OR:
            return 1;
        case TokenType::AND:
            return 2;
        case TokenType::EQ:
        case TokenType::NE:
            return 3;
        case TokenType::GT:
        case TokenType::LT:
        case TokenType::GE:
        case TokenType::LE:
            return 4;
        case TokenType::PLUS:
        case TokenType::MINUS:
            return 5;
        case TokenType::STAR:
        case TokenType::SLASH:
        case TokenType::PERCENT:
            return 6;
        // Highest precedence
        default:
            return 0;  // Not a binary operator
    }
}

Expression::BinaryOp Parser::token_to_binary_op(TokenType type) {
    switch (type) {
        case TokenType::PLUS:    return Expression::BinaryOp::ADD;
        case TokenType::MINUS:   return Expression::BinaryOp::SUB;
        case TokenType::STAR:    return Expression::BinaryOp::MUL;
        case TokenType::SLASH:   return Expression::BinaryOp::DIV;
        case TokenType::PERCENT: return Expression::BinaryOp::MOD;
        case TokenType::EQ:      return Expression::BinaryOp::EQ;
        case TokenType::NE:      return Expression::BinaryOp::NE;
        case TokenType::GT:      return Expression::BinaryOp::GT;
        case TokenType::LT:      return Expression::BinaryOp::LT;
        case TokenType::GE:      return Expression::BinaryOp::GE;
        case TokenType::LE:      return Expression::BinaryOp::LE;
        case TokenType::AND:     return Expression::BinaryOp::AND;
        case TokenType::OR:      return Expression::BinaryOp::OR;
        default:                 return Expression::BinaryOp::ADD;  // Should never happen
    }
}

Expression::UnaryOp Parser::token_to_unary_op(TokenType type) {
    switch (type) {
        case TokenType::NOT:   return Expression::UnaryOp::NOT;
        case TokenType::MINUS: return Expression::UnaryOp::NEG;
        default:               return Expression::UnaryOp::NOT;  // Should never happen
    }
}

std::unique_ptr<Expression> Parser::parse_expression(int min_precedence) {
    // Parse left-hand side (unary or primary)
    auto left = parse_unary();
    if (!left) {
        return left;
    }

    // Handle postfix (member access, function calls)
    left = parse_postfix(std::move(left));

    // Parse binary operators with precedence climbing
    while (true) {
        TokenType op_type = current().type;
        int precedence = get_operator_precedence(op_type);

        // Stop if not a binary operator (precedence == 0) or precedence too low
        if (precedence == 0) {
            break;
        }
        if (precedence < min_precedence) {
            break;
        }

        // Consume the operator
        pos_++;

        // Parse right-hand side with higher precedence
        auto right = parse_expression(precedence + 1);
        if (!right) {
            error("Expected expression after operator");
            break;
        }

        // Build binary expression
        left = Expression::make_binary(token_to_binary_op(op_type),
                                        std::move(left),
                                        std::move(right));
    }

    return left;
}

std::unique_ptr<Expression> Parser::parse_unary() {
    // Check for unary operators: not, -
    if (check(TokenType::NOT)) {
        pos_++;
        auto operand = parse_unary();  // Right-associative
        if (!operand) {
            error("Expected expression after 'not'");
            return Expression::make_bool(false);
        }
        return Expression::make_unary(Expression::UnaryOp::NOT, std::move(operand));
    }

    if (check(TokenType::MINUS)) {
        pos_++;
        auto operand = parse_unary();  // Right-associative
        if (!operand) {
            error("Expected expression after '-'");
            return Expression::make_integer(0);
        }
        return Expression::make_unary(Expression::UnaryOp::NEG, std::move(operand));
    }

    return parse_primary();
}

std::unique_ptr<Expression> Parser::parse_postfix(std::unique_ptr<Expression> expr) {
    // Handle member access: expr.member
    while (check(TokenType::DOT)) {
        pos_++;  // consume '.'

        // Member name can be an identifier or a property keyword (e.g., city.population)
        std::string member;
        if (check(TokenType::IDENTIFIER)) {
            member = current().lexeme;
            pos_++;
        } else if (is_property_keyword(current().type)) {
            member = property_token_to_string(current().type);
            if (member.empty()) member = current().lexeme;
            pos_++;
        } else {
            error("Expected member name after '.'");
            break;
        }

        expr = Expression::make_member(std::move(expr), member);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::parse_member_or_call(const std::string& identifier) {
    auto expr = Expression::make_reference(identifier);

    // Handle member access
    expr = parse_postfix(std::move(expr));

    // Handle function call
    if (check(TokenType::LEFT_PAREN)) {
        pos_++;  // consume '('

        std::vector<std::unique_ptr<Expression>> args;

        // Parse arguments
        if (!check(TokenType::RIGHT_PAREN)) {
            do {
                auto arg = parse_expression();
                if (arg) {
                    args.push_back(std::move(arg));
                }
            } while (match(TokenType::COMMA));
        }

        consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments");

        // Convert to call expression
        // If it was a member access, the function_name should be the full path
        std::string func_name;
        if (expr->type == Expression::Type::MEMBER) {
            // For now, flatten member access to function name
            // e.g., civ.population -> we store "population" and object separately
            // But for a call like obj.method(), we need to handle it properly
            func_name = expr->member_name;
            // Keep the object for potential method call semantics
        } else {
            func_name = expr->reference;
        }

        return Expression::make_call(func_name, std::move(args));
    }

    return expr;
}

std::unique_ptr<Expression> Parser::parse_primary() {
    auto expr = std::make_unique<Expression>();

    if (check(TokenType::INTEGER)) {
        expr->type = Expression::Type::INTEGER;
        expr->value = std::get<int64_t>(current().value);
        pos_++;
        return expr;
    }

    if (check(TokenType::FLOAT)) {
        expr->type = Expression::Type::FLOAT;
        expr->value = std::get<double>(current().value);
        pos_++;
        return expr;
    }

    if (check(TokenType::STRING)) {
        expr->type = Expression::Type::STRING;
        expr->value = std::get<std::string>(current().value);
        pos_++;
        return expr;
    }

    if (check(TokenType::HEX_COLOR)) {
        expr->type = Expression::Type::COLOR;
        expr->value = current().lexeme;
        pos_++;
        return expr;
    }

    if (check(TokenType::TRUE)) {
        expr->type = Expression::Type::BOOLEAN;
        expr->value = true;
        pos_++;
        return expr;
    }

    if (check(TokenType::FALSE)) {
        expr->type = Expression::Type::BOOLEAN;
        expr->value = false;
        pos_++;
        return expr;
    }

    if (check(TokenType::NULL_LIT)) {
        expr->type = Expression::Type::NULL_VAL;
        pos_++;
        return expr;
    }

    if (check(TokenType::IDENTIFIER)) {
        std::string name = current().lexeme;
        pos_++;
        return parse_member_or_call(name);
    }

    // Property keywords can also be used as identifiers in expressions
    // (e.g., "population" in "population > 100")
    if (is_property_keyword(current().type)) {
        std::string name = property_token_to_string(current().type);
        if (name.empty()) name = current().lexeme;
        pos_++;
        return parse_member_or_call(name);
    }

    // Grouped expression: (expr)
    if (check(TokenType::LEFT_PAREN)) {
        pos_++;  // consume '('
        auto grouped = parse_expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return grouped;
    }

    error("Expected expression");
    expr->type = Expression::Type::NULL_VAL;
    return expr;
}

// ============================================================================
// Condition Parsing
// ============================================================================

std::unique_ptr<Condition> Parser::parse_condition() {
    auto cond = std::make_unique<Condition>();

    // Get the trigger type
    if (check(TokenType::WHEN)) {
        cond->trigger = "when";
    } else if (check(TokenType::IF)) {
        cond->trigger = "if";
    } else if (check(TokenType::AVAILABLE_IF)) {
        cond->trigger = "available_if";
    } else if (check(TokenType::SECRET_IF)) {
        cond->trigger = "secret_if";
    } else if (check(TokenType::ACTIVE_IF)) {
        cond->trigger = "active_if";
    } else if (check(TokenType::BONUS_IF)) {
        cond->trigger = "bonus_if";
    } else {
        error("Expected condition keyword");
        return nullptr;
    }
    pos_++;  // Consume trigger

    // Check for "keyword: { ... }" format (property-style condition)
    // vs "keyword expr { ... }" format (expression condition)
    if (check(TokenType::COLON)) {
        // Property-style: available_if: { technologies: [X] }
        pos_++;  // Consume ':'
        
        consume(TokenType::LEFT_BRACE, "Expected '{' after ':' in condition");

        while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
            auto prop = parse_property();
            if (prop) cond->properties.push_back(std::move(prop));
        }

        consume(TokenType::RIGHT_BRACE, "Expected '}' after condition block");

        // Create a simple "true" expression as placeholder
        cond->expression = Expression::make_bool(true);
    } else {
        // Expression-style: when expr { ... }
        cond->expression = parse_expression();

        if (!cond->expression) {
            error("Expected condition expression");
            return nullptr;
        }

        // Parse block
        consume(TokenType::LEFT_BRACE, "Expected '{' after condition");

        while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
            auto prop = parse_property();
            if (prop) cond->properties.push_back(std::move(prop));
        }

        consume(TokenType::RIGHT_BRACE, "Expected '}' after condition block");
    }

    return cond;
}

// ============================================================================
// Generic Definition Parsing (for custom types from --types)
// ============================================================================

std::unique_ptr<Definition> Parser::parse_generic_definition(const std::string& type_name) {
    auto def = std::make_unique<Definition>();
    def->definition_type = type_name;
    
    Token name = consume(TokenType::IDENTIFIER, "Expected definition name");
    def->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after definition name");
    
    // Parse properties (same as any definition)
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (is_condition_keyword(current().type)) {
            auto cond = parse_condition();
            if (cond) def->conditions.push_back(std::move(cond));
        } else {
            auto prop = parse_property();
            if (prop) {
                def->properties.push_back(std::move(prop));
            }
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after definition");
    
    return def;
}

} // namespace lex
