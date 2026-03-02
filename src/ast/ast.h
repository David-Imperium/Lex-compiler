#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <variant>
#include <cstdint>

namespace lex {

// ============================================================================
// Base Classes
// ============================================================================

class ASTNode {
public:
    virtual ~ASTNode() = default;
    std::string location;
};

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
};

// ============================================================================
// Expressions (literals, references, operators, calls)
// ============================================================================

class Expression : public ASTNode {
public:
    enum class Type {
        INTEGER, FLOAT, STRING, BOOLEAN, COLOR, NULL_VAL, REFERENCE,
        BINARY, UNARY, CALL, MEMBER
    };

    // Binary operators
    enum class BinaryOp {
        ADD, SUB, MUL, DIV, MOD,           // Arithmetic: + - * / %
        EQ, NE, GT, LT, GE, LE,            // Comparison: == != > < >= <=
        AND, OR                            // Logical: and or
    };

    // Unary operators
    enum class UnaryOp { NOT, NEG };       // Logical: not, Arithmetic: -

    Type type = Type::NULL_VAL;

    // For literals and references
    std::variant<int64_t, double, std::string, bool> value;
    std::string reference;

    // For binary expressions (type == BINARY)
    BinaryOp binary_op = BinaryOp::ADD;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;

    // For unary expressions (type == UNARY)
    UnaryOp unary_op = UnaryOp::NOT;
    std::unique_ptr<Expression> operand;

    // For call expressions (type == CALL)
    std::string function_name;
    std::vector<std::unique_ptr<Expression>> arguments;

    // For member expressions (type == MEMBER)
    std::unique_ptr<Expression> object;
    std::string member_name;

    // Convenience factory methods
    static std::unique_ptr<Expression> make_integer(int64_t val);
    static std::unique_ptr<Expression> make_float(double val);
    static std::unique_ptr<Expression> make_string(const std::string& val);
    static std::unique_ptr<Expression> make_bool(bool val);
    static std::unique_ptr<Expression> make_reference(const std::string& name);
    static std::unique_ptr<Expression> make_binary(BinaryOp op,
                                                    std::unique_ptr<Expression> left,
                                                    std::unique_ptr<Expression> right);
    static std::unique_ptr<Expression> make_unary(UnaryOp op,
                                                   std::unique_ptr<Expression> operand);
    static std::unique_ptr<Expression> make_call(const std::string& name,
                                                  std::vector<std::unique_ptr<Expression>> args);
    static std::unique_ptr<Expression> make_member(std::unique_ptr<Expression> obj,
                                                    const std::string& member);
};

// ============================================================================
// Collections
// ============================================================================

class ResourceMap : public ASTNode {
public:
    // "Gold" -> 50, "Wood" -> 20
    std::map<std::string, int64_t> resources;
};

class ReferenceList : public ASTNode {
public:
    std::vector<std::string> references;
};

// ============================================================================
// Properties (key-value pairs in definitions)
// ============================================================================

class PropertyValue : public ASTNode {
public:
    enum class Type { EXPRESSION, RESOURCE_MAP, REFERENCE_LIST };
    Type type = Type::EXPRESSION;
    
    std::unique_ptr<Expression> expression;
    std::unique_ptr<ResourceMap> resource_map;
    std::unique_ptr<ReferenceList> reference_list;
};

class Property : public ASTNode {
public:
    std::string name;
    std::unique_ptr<PropertyValue> value;
};

// ============================================================================
// Conditions (when/if blocks)
// ============================================================================

class Condition : public ASTNode {
public:
    std::string trigger;  // "when", "if", "available_if", etc.
    std::unique_ptr<Expression> expression;  // Parsed condition expression
    std::vector<std::unique_ptr<Property>> properties;
};

// ============================================================================
// Definitions (top-level constructs)
// ============================================================================

class Definition : public ASTNode {
public:
    std::string identifier;  // The name of the definition
    std::string definition_type;  // "era", "structure", "unit", etc.
    std::vector<std::unique_ptr<Property>> properties;
    std::vector<std::unique_ptr<Condition>> conditions;
};

// ============================================================================
// Concrete Definitions (convenience accessors)
// ============================================================================

class EraDefinition : public Definition {
public:
    EraDefinition() { definition_type = "era"; }
    // Properties: name, period, music, dominant_color, atmosphere
};

class StructureDefinition : public Definition {
public:
    StructureDefinition() { definition_type = "structure"; }
    // Properties: era, cost, production, maintenance, requires, unlocks,
    //             description, icon, tooltip
};

class UnitDefinition : public Definition {
public:
    UnitDefinition() { definition_type = "unit"; }
    // Properties: era, type, cost, maintenance, attack, defense, movement, range,
    //             requires, description
};

class TechnologyDefinition : public Definition {
public:
    TechnologyDefinition() { definition_type = "technology"; }
    // Properties: era, research_cost, prerequisites, unlocks, description, quote
};

class ResourceDefinition : public Definition {
public:
    ResourceDefinition() { definition_type = "resource"; }
    // Properties: type, icon, color, description
};

class ChoiceDefinition : public Definition {
public:
    ChoiceDefinition() { definition_type = "choice"; }
    // Properties: era, text, options
};

class EndingDefinition : public Definition {
public:
    EndingDefinition() { definition_type = "ending"; }
    // Properties: type, text, requirements
};

class EventDefinition : public Definition {
public:
    EventDefinition() { definition_type = "event"; }
    // Properties: era, trigger, text, effect, archivist
};

class SecretDefinition : public Definition {
public:
    SecretDefinition() { definition_type = "secret"; }
    // Properties: trigger, activate, archivist
};

// ============================================================================
// AST Root (entire file)
// ============================================================================

class ASTFile : public ASTNode {
public:
    std::vector<std::unique_ptr<Definition>> definitions;
};

} // namespace lex
