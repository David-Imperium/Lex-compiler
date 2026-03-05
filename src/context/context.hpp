#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>

namespace lex {

// Forward declarations
class Definition;
class ASTFile;

// ============================================================================
// Context Output Formats
// ============================================================================

enum class ContextFormat {
    JSON,       // Full structured context
    Markdown,   // Human-readable documentation
    Minimal     // Compact for LLM prompts
};

// ============================================================================
// Context Options
// ============================================================================

struct ContextOptions {
    ContextFormat format = ContextFormat::JSON;
    
    // Include sections
    bool include_graph = true;        // Dependency graph
    bool include_statistics = true;   // Entity counts by type/era
    bool include_summaries = true;    // AI-generated summaries
    
    // Graph options
    bool include_reverse_refs = true; // "referenced_by" field
    
    // Statistics options
    std::vector<std::string> group_by_properties = {"era"};  // Group stats by these properties
    
    // Output options
    bool pretty_print = true;         // For JSON/Markdown formatting
    std::string indent = "  ";        // Indentation string
};

// ============================================================================
// Entity Context (single definition analysis)
// ============================================================================

struct EntityContext {
    std::string id;                              // "Farm"
    std::string type;                            // "structure"
    std::string summary;                         // AI-generated summary
    
    // Properties (extracted values)
    std::map<std::string, std::string> properties;  // All properties as strings
    
    // Reference analysis
    std::vector<std::string> references;        // What this entity references
    std::vector<std::string> referenced_by;     // What references this entity
    
    // Dependency analysis
    std::vector<std::string> dependencies;      // What this needs (requires, etc.)
    std::vector<std::string> dependents;        // What needs this
    
    // Raw data for custom processing
    const Definition* source = nullptr;         // Pointer to original definition
};

// ============================================================================
// Graph Context (dependency graph)
// ============================================================================

struct GraphEdge {
    std::string from;
    std::string to;
    std::string type;  // "requires", "unlocks", "produces", etc.
};

struct GraphContext {
    std::vector<std::string> nodes;
    std::vector<GraphEdge> edges;
};

// ============================================================================
// Statistics Context (aggregated data)
// ============================================================================

struct StatisticsContext {
    size_t total_entities = 0;
    std::map<std::string, size_t> by_type;      // "structure" -> 5
    std::map<std::string, size_t> by_property;  // "era:Ancient" -> 3
};

// ============================================================================
// Context Result (full output)
// ============================================================================

struct ContextResult {
    // Success status
    bool success = false;
    std::string error;
    
    // Schema info
    std::string schema_version = "0.4.1";
    std::vector<std::string> types;
    
    // Analyzed data
    std::vector<EntityContext> entities;
    GraphContext graph;
    StatisticsContext statistics;
    
    // Convenience methods
    bool has_entities() const { return !entities.empty(); }
    size_t entity_count() const { return entities.size(); }
    
    // Find entity by ID (case-insensitive)
    const EntityContext* find_entity(const std::string& id) const {
        std::string lower_id;
        for (char c : id) lower_id += std::tolower(static_cast<unsigned char>(c));
        
        for (const auto& e : entities) {
            std::string lower_eid;
            for (char c : e.id) lower_eid += std::tolower(static_cast<unsigned char>(c));
            
            if (lower_eid == lower_id) return &e;
        }
        return nullptr;
    }
    
    // Get entities by type
    std::vector<const EntityContext*> get_by_type(const std::string& type) const {
        std::vector<const EntityContext*> result;
        for (const auto& e : entities) {
            if (e.type == type) result.push_back(&e);
        }
        return result;
    }
};

// ============================================================================
// Context Generator API
// ============================================================================

// Generate context from AST
ContextResult generate_context(
    const std::vector<std::unique_ptr<Definition>>& definitions,
    const ContextOptions& options = {}
);

// Generate context from AST file
ContextResult generate_context(
    const ASTFile& ast,
    const ContextOptions& options = {}
);

// Format context as JSON
std::string format_context_json(const ContextResult& context, const ContextOptions& options = {});

// Format context as Markdown
std::string format_context_markdown(const ContextResult& context, const ContextOptions& options = {});

// Format context as Minimal (for LLM prompts)
std::string format_context_minimal(const ContextResult& context, const ContextOptions& options = {});

// Get file extension for context format
std::string context_extension(ContextFormat format);

} // namespace lex
