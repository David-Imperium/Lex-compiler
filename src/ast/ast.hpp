#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <variant>
#include <cstdint>
#include <optional>

namespace lex {

// ============================================================================
// Type System
// ============================================================================

enum class LexType {
    UNKNOWN,        // Type not yet inferred
    INTEGER,        // int64_t
    FLOAT,          // double
    STRING,         // std::string
    BOOLEAN,        // bool
    REFERENCE,      // Reference to another definition (e.g., era name)
    RESOURCE_MAP,   // { Gold: 50, Wood: 20 }
    REFERENCE_LIST, // [TechA, TechB]
    OBJECT,         // { key: value, ... } - nested object
    ARRAY,          // [value1, value2, ...] - array of values
    COLOR,          // #RRGGBB
    VOID            // No type (for statements)
};

std::string type_to_string(LexType type);

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
    LexType inferred_type = LexType::UNKNOWN;  // Type inference result

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

    // Type inference
    LexType infer_type() const;

    // Clone method (deep copy)
    std::unique_ptr<Expression> clone() const;

    // Convenience factory methods
    static std::unique_ptr<Expression> make_integer(int64_t val);
    static std::unique_ptr<Expression> make_float(double val);
    static std::unique_ptr<Expression> make_string(const std::string& val);
    static std::unique_ptr<Expression> make_bool(bool val);
    static std::unique_ptr<Expression> make_null();
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

// Forward declaration for PropertyValue (needed by ObjectValue)
class PropertyValue;

// ObjectValue - nested object with properties
// Example: stats: { attack: 10, defense: 5 }
class ObjectValue : public ASTNode {
public:
    // Store as vector of name-value pairs to preserve order
    std::vector<std::pair<std::string, std::unique_ptr<PropertyValue>>> properties;
    
    // Helper methods
    const PropertyValue* get(const std::string& name) const;
    bool has(const std::string& name) const;
    
    std::unique_ptr<ObjectValue> clone() const;
};

// ArrayValue - array of values
// Example: tags: ["warrior", "melee"]
class ArrayValue : public ASTNode {
public:
    std::vector<std::unique_ptr<PropertyValue>> values;
    
    std::unique_ptr<ArrayValue> clone() const;
};

// ============================================================================
// Properties (key-value pairs in definitions)
// ============================================================================

// Property name for objects (key-value pair within an object)
class PropertyName : public ASTNode {
public:
    std::string name;
    std::unique_ptr<PropertyValue> value;
};

class PropertyValue : public ASTNode {
public:
    enum class Type { EXPRESSION, RESOURCE_MAP, REFERENCE_LIST, OBJECT, ARRAY };
    Type type = Type::EXPRESSION;

    std::unique_ptr<Expression> expression;
    std::unique_ptr<ResourceMap> resource_map;
    std::unique_ptr<ReferenceList> reference_list;
    std::unique_ptr<ObjectValue> object;
    std::unique_ptr<ArrayValue> array;

    std::unique_ptr<PropertyValue> clone() const;
};

class Property : public ASTNode {
public:
    std::string name;
    std::unique_ptr<PropertyValue> value;

    std::unique_ptr<Property> clone() const;
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
// Visibility (module system)
// ============================================================================

enum class Visibility {
    PUBLIC,     // Visible to everyone (modders + developer)
    INTERNAL,   // Visible only to developer (Lex Engine/Core)
    PRIVATE     // Visible only in current file
};

// ============================================================================
// Definitions (top-level constructs)
// ============================================================================

class Definition : public ASTNode {
public:
    std::string identifier;  // The name of the definition
    std::string definition_type;  // "era", "structure", "unit", etc.
    Visibility visibility = Visibility::PUBLIC;  // Default: public
    std::vector<std::unique_ptr<Property>> properties;
    std::vector<std::unique_ptr<Condition>> conditions;

    // Typed property accessors
    std::optional<int64_t> get_int_property(const std::string& name) const;
    std::optional<double> get_float_property(const std::string& name) const;
    std::optional<std::string> get_string_property(const std::string& name) const;
    std::optional<bool> get_bool_property(const std::string& name) const;
    const ResourceMap* get_resource_map_property(const std::string& name) const;
    const ReferenceList* get_reference_list_property(const std::string& name) const;
    const Expression* get_expression_property(const std::string& name) const;

    // Property existence check
    bool has_property(const std::string& name) const;
};

// ============================================================================
// Module Declaration
// ============================================================================

class ModuleDeclaration : public ASTNode {
public:
    std::string name;  // e.g., "engine.internal", "game.public"
};

// ============================================================================
// Use Statement (module import)
// ============================================================================

class UseStatement : public ASTNode {
public:
    std::string path;  // File path: "characters.lex"
    std::string alias; // Optional alias (for future: use "file.lex" as alias)
};

// ============================================================================
// AST Root (entire file)
// ============================================================================

class ASTFile : public ASTNode {
public:
    std::string module_name;  // e.g., "engine.internal", "game.public"
    std::vector<std::unique_ptr<UseStatement>> imports;
    std::vector<std::unique_ptr<Definition>> definitions;
};

} // namespace lex
