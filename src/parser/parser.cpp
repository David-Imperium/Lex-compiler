#include "parser.h"
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
           type == TokenType::SECRET;
}

// ============================================================================
// Definition Parsing
// ============================================================================

std::unique_ptr<Definition> Parser::parse_definition() {
    TokenType type = current().type;
    
    switch (type) {
        case TokenType::ERA:
            return parse_era();
        case TokenType::STRUCTURE:
            return parse_structure();
        case TokenType::UNIT:
            return parse_unit();
        case TokenType::TECHNOLOGY:
            return parse_technology();
        case TokenType::RESOURCE:
            return parse_resource();
        case TokenType::CHOICE:
            return parse_choice();
        case TokenType::ENDING:
            return parse_ending();
        case TokenType::EVENT:
            return parse_event();
        case TokenType::SECRET:
            return parse_secret();
        default:
            error("Expected definition keyword");
            return nullptr;
    }
}

std::unique_ptr<EraDefinition> Parser::parse_era() {
    auto era = std::make_unique<EraDefinition>();
    
    consume(TokenType::ERA, "Expected 'era'");
    
    Token name = consume(TokenType::IDENTIFIER, "Expected era name");
    era->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after era name");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (is_condition_keyword(current().type)) {
            auto cond = parse_condition();
            if (cond) era->conditions.push_back(std::move(cond));
        } else {
            auto prop = parse_property();
            if (prop) era->properties.push_back(std::move(prop));
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after era definition");
    
    return era;
}

std::unique_ptr<StructureDefinition> Parser::parse_structure() {
    auto structure = std::make_unique<StructureDefinition>();
    
    consume(TokenType::STRUCTURE, "Expected 'structure'");
    
    Token name = consume(TokenType::IDENTIFIER, "Expected structure name");
    structure->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after structure name");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (is_condition_keyword(current().type)) {
            auto cond = parse_condition();
            if (cond) structure->conditions.push_back(std::move(cond));
        } else {
            auto prop = parse_property();
            if (prop) structure->properties.push_back(std::move(prop));
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after structure definition");
    
    return structure;
}

std::unique_ptr<UnitDefinition> Parser::parse_unit() {
    auto unit = std::make_unique<UnitDefinition>();
    
    consume(TokenType::UNIT, "Expected 'unit'");
    
    Token name = consume(TokenType::IDENTIFIER, "Expected unit name");
    unit->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after unit name");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (is_condition_keyword(current().type)) {
            auto cond = parse_condition();
            if (cond) unit->conditions.push_back(std::move(cond));
        } else {
            auto prop = parse_property();
            if (prop) unit->properties.push_back(std::move(prop));
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after unit definition");
    
    return unit;
}

std::unique_ptr<TechnologyDefinition> Parser::parse_technology() {
    auto tech = std::make_unique<TechnologyDefinition>();
    
    consume(TokenType::TECHNOLOGY, "Expected 'technology'");
    
    Token name = consume(TokenType::IDENTIFIER, "Expected technology name");
    tech->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after technology name");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (is_condition_keyword(current().type)) {
            auto cond = parse_condition();
            if (cond) tech->conditions.push_back(std::move(cond));
        } else {
            auto prop = parse_property();
            if (prop) tech->properties.push_back(std::move(prop));
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after technology definition");
    
    return tech;
}

std::unique_ptr<ResourceDefinition> Parser::parse_resource() {
    auto resource = std::make_unique<ResourceDefinition>();
    
    consume(TokenType::RESOURCE, "Expected 'resource'");
    
    Token name = consume(TokenType::IDENTIFIER, "Expected resource name");
    resource->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after resource name");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        auto prop = parse_property();
        if (prop) resource->properties.push_back(std::move(prop));
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after resource definition");
    
    return resource;
}

std::unique_ptr<ChoiceDefinition> Parser::parse_choice() {
    auto choice = std::make_unique<ChoiceDefinition>();
    
    consume(TokenType::CHOICE, "Expected 'choice'");
    
    Token name = consume(TokenType::IDENTIFIER, "Expected choice name");
    choice->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after choice name");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (is_condition_keyword(current().type)) {
            auto cond = parse_condition();
            if (cond) choice->conditions.push_back(std::move(cond));
        } else {
            auto prop = parse_property();
            if (prop) choice->properties.push_back(std::move(prop));
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after choice definition");
    
    return choice;
}

std::unique_ptr<EndingDefinition> Parser::parse_ending() {
    auto ending = std::make_unique<EndingDefinition>();
    
    consume(TokenType::ENDING, "Expected 'ending'");
    
    Token name = consume(TokenType::IDENTIFIER, "Expected ending name");
    ending->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after ending name");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        auto prop = parse_property();
        if (prop) ending->properties.push_back(std::move(prop));
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after ending definition");
    
    return ending;
}

std::unique_ptr<EventDefinition> Parser::parse_event() {
    auto event = std::make_unique<EventDefinition>();
    
    consume(TokenType::EVENT, "Expected 'event'");
    
    Token name = consume(TokenType::IDENTIFIER, "Expected event name");
    event->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after event name");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (is_condition_keyword(current().type)) {
            auto cond = parse_condition();
            if (cond) event->conditions.push_back(std::move(cond));
        } else {
            auto prop = parse_property();
            if (prop) event->properties.push_back(std::move(prop));
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after event definition");
    
    return event;
}

std::unique_ptr<SecretDefinition> Parser::parse_secret() {
    auto secret = std::make_unique<SecretDefinition>();
    
    consume(TokenType::SECRET, "Expected 'secret'");
    
    Token name = consume(TokenType::IDENTIFIER, "Expected secret name");
    secret->identifier = name.lexeme;
    
    consume(TokenType::LEFT_BRACE, "Expected '{' after secret name");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        if (is_condition_keyword(current().type)) {
            auto cond = parse_condition();
            if (cond) secret->conditions.push_back(std::move(cond));
        } else {
            auto prop = parse_property();
            if (prop) secret->properties.push_back(std::move(prop));
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after secret definition");
    
    return secret;
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
        
        // Expect number
        if (!check(TokenType::INTEGER)) {
            error("Expected quantity after ':'");
            pos_++;
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
// Expression Parsing
// ============================================================================

std::unique_ptr<Expression> Parser::parse_expression() {
    return parse_primary();
}

std::unique_ptr<Expression> Parser::parse_primary() {
    auto expr = std::make_unique<Expression>();
    
    if (check(TokenType::INTEGER)) {
        expr->type = Expression::Type::INTEGER;
        expr->value = std::get<int64_t>(current().value);
        pos_++;
    } else if (check(TokenType::FLOAT)) {
        expr->type = Expression::Type::FLOAT;
        expr->value = std::get<double>(current().value);
        pos_++;
    } else if (check(TokenType::STRING)) {
        expr->type = Expression::Type::STRING;
        expr->value = std::get<std::string>(current().value);
        pos_++;
    } else if (check(TokenType::HEX_COLOR)) {
        expr->type = Expression::Type::COLOR;
        expr->value = current().lexeme;
        pos_++;
    } else if (check(TokenType::TRUE)) {
        expr->type = Expression::Type::BOOLEAN;
        expr->value = true;
        pos_++;
    } else if (check(TokenType::FALSE)) {
        expr->type = Expression::Type::BOOLEAN;
        expr->value = false;
        pos_++;
    } else if (check(TokenType::NULL_LIT)) {
        expr->type = Expression::Type::NULL_VAL;
        pos_++;
    } else if (check(TokenType::IDENTIFIER)) {
        expr->type = Expression::Type::REFERENCE;
        expr->reference = current().lexeme;
        pos_++;
    } else {
        error("Expected expression");
        expr->type = Expression::Type::NULL_VAL;
    }
    
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
    
    // Parse condition expression (simplified - just collect until '{')
    std::stringstream expr_stream;
    while (!check(TokenType::LEFT_BRACE) && !check(TokenType::END_OF_FILE)) {
        expr_stream << current().lexeme << " ";
        pos_++;
    }
    cond->expression = expr_stream.str();
    
    // Parse block
    consume(TokenType::LEFT_BRACE, "Expected '{' after condition");
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::END_OF_FILE)) {
        auto prop = parse_property();
        if (prop) cond->properties.push_back(std::move(prop));
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after condition block");
    
    return cond;
}

} // namespace lex
