#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>
#include "../ast/ast.h"

namespace lex {

enum class ErrorSeverity {
    ERROR,
    WARNING
};

class SemanticError {
public:
    std::string code;
    std::string message;
    std::string location;
    ErrorSeverity severity = ErrorSeverity::ERROR;
};

class Validator {
public:
    bool validate(const std::vector<std::unique_ptr<Definition>>& definitions);

    const std::vector<SemanticError>& errors() const { return errors_; }
    const std::vector<SemanticError>& warnings() const { return warnings_; }
    bool has_errors() const { return !errors_.empty(); }
    bool has_warnings() const { return !warnings_.empty(); }

private:
    std::vector<SemanticError> errors_;
    std::vector<SemanticError> warnings_;
    
    // Symbol tables by type
    std::set<std::string> eras_;
    std::set<std::string> structures_;
    std::set<std::string> units_;
    std::set<std::string> technologies_;
    std::set<std::string> resources_;
    std::set<std::string> all_symbols_;

    void add_error(const std::string& code, const std::string& message, const std::string& location);
    void add_warning(const std::string& code, const std::string& message, const std::string& location);
    
    void register_symbols(const std::vector<std::unique_ptr<Definition>>& definitions);
    void validate_references(const std::vector<std::unique_ptr<Definition>>& definitions);
    void validate_required_properties(const std::vector<std::unique_ptr<Definition>>& definitions);
    void validate_resource_maps(const std::vector<std::unique_ptr<Definition>>& definitions);
};

}
