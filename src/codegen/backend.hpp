#pragma once

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include "../ast/ast.hpp"

namespace lex {

class Backend {
public:
    virtual ~Backend() = default;

    virtual std::string name() const = 0;
    virtual std::string file_extension() const = 0;
    virtual std::string generate(const std::vector<std::unique_ptr<Definition>>& ast) = 0;

    // Optional: configure backend with compile-time options
    virtual void configure(const std::string& source_name) {
        (void)source_name;  // Default: no-op
    }

protected:
    std::vector<std::string> errors_;
};

// Factory function type
using BackendFactory = std::function<std::unique_ptr<Backend>()>;

class BackendRegistry {
public:
    static BackendRegistry& instance();

    // Register a factory for creating backends
    void register_factory(const std::string& name, BackendFactory factory);
    
    // Create a new backend instance
    std::unique_ptr<Backend> create(const std::string& name);
    
    // List available backends
    std::vector<std::string> available_backends() const;

private:
    std::map<std::string, BackendFactory> factories_;
};

}
