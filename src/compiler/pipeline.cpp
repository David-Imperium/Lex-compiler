#include "pipeline.hpp"
#include <algorithm>

namespace lex {

// ============================================================================
// CompilerPipeline Implementation
// ============================================================================

void CompilerPipeline::add_pass(std::unique_ptr<Pass> pass) {
    passes_.push_back(std::move(pass));
}

CompileResult CompilerPipeline::run(PassContext& ctx) {
    CompileResult result;
    
    for (auto& pass : passes_) {
        // Skip optional passes if we already have errors
        if (ctx.has_errors() && pass->is_optional()) {
            continue;
        }
        
        if (!pass->run(ctx)) {
            // Pass failed, stop pipeline
            break;
        }
    }
    
    // Transfer results
    result.success = !ctx.has_errors();
    result.outputs = ctx.outputs;
    result.errors = ctx.errors;
    result.warnings = ctx.warnings;
    
    return result;
}

std::vector<std::string> CompilerPipeline::pass_names() const {
    std::vector<std::string> names;
    for (const auto& pass : passes_) {
        names.push_back(pass->name());
    }
    return names;
}

CompilerPipeline CompilerPipeline::create_default(const CompileOptions& options) {
    CompilerPipeline pipeline;
    
    // Standard pipeline order
    pipeline.add_pass(std::make_unique<LexerPass>());
    pipeline.add_pass(std::make_unique<ParserPass>());
    pipeline.add_pass(std::make_unique<ValidationPass>());
    pipeline.add_pass(std::make_unique<TypeCheckPass>());
    pipeline.add_pass(std::make_unique<VisibilityPass>());
    pipeline.add_pass(std::make_unique<CodegenPass>());
    
    return pipeline;
}

// ============================================================================
// PipelineBuilder Implementation
// ============================================================================

PipelineBuilder& PipelineBuilder::with_lexer() {
    pipeline_.add_pass(std::make_unique<LexerPass>());
    return *this;
}

PipelineBuilder& PipelineBuilder::with_parser() {
    pipeline_.add_pass(std::make_unique<ParserPass>());
    return *this;
}

PipelineBuilder& PipelineBuilder::with_validation(bool enabled) {
    if (enabled) {
        pipeline_.add_pass(std::make_unique<ValidationPass>());
    }
    return *this;
}

PipelineBuilder& PipelineBuilder::with_typecheck(bool enabled) {
    if (enabled) {
        pipeline_.add_pass(std::make_unique<TypeCheckPass>());
    }
    return *this;
}

PipelineBuilder& PipelineBuilder::with_visibility_filter(bool enabled) {
    if (enabled) {
        pipeline_.add_pass(std::make_unique<VisibilityPass>());
    }
    return *this;
}

PipelineBuilder& PipelineBuilder::with_codegen() {
    pipeline_.add_pass(std::make_unique<CodegenPass>());
    return *this;
}

PipelineBuilder& PipelineBuilder::with_pass(std::unique_ptr<Pass> pass) {
    pipeline_.add_pass(std::move(pass));
    return *this;
}

CompilerPipeline PipelineBuilder::build() {
    return std::move(pipeline_);
}

} // namespace lex
