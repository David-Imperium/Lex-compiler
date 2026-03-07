#pragma once

#include "pass.hpp"
#include <vector>
#include <memory>
#include <functional>

namespace lex {

// ============================================================================
// Compiler Pipeline - Orchestrates passes
// ============================================================================

class CompilerPipeline {
public:
    CompilerPipeline() = default;
    
    // Add a pass to the pipeline (executed in order)
    void add_pass(std::unique_ptr<Pass> pass);
    
    // Add a pass with factory function
    template<typename T, typename... Args>
    void emplace_pass(Args&&... args) {
        add_pass(std::make_unique<T>(std::forward<Args>(args)...));
    }
    
    // Execute all passes
    CompileResult run(PassContext& ctx);
    
    // Get pass names for debugging
    std::vector<std::string> pass_names() const;
    
    // Create default pipeline (standard compilation)
    static CompilerPipeline create_default(const CompileOptions& options);
    
private:
    std::vector<std::unique_ptr<Pass>> passes_;
};

// ============================================================================
// Pipeline Builder - Fluent API
// ============================================================================

class PipelineBuilder {
public:
    PipelineBuilder& with_lexer();
    PipelineBuilder& with_parser();
    PipelineBuilder& with_validation(bool enabled = true);
    PipelineBuilder& with_typecheck(bool enabled = true);
    PipelineBuilder& with_visibility_filter(bool enabled = true);
    PipelineBuilder& with_codegen();
    PipelineBuilder& with_pass(std::unique_ptr<Pass> pass);
    
    CompilerPipeline build();
    
private:
    CompilerPipeline pipeline_;
};

} // namespace lex
