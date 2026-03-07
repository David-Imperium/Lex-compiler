// =============================================================================
// Aurelius RPC Protocol - Type Definitions
// =============================================================================
// 
// Syntax: Type? for optional, Type[] for arrays
// =============================================================================

// -----------------------------------------------------------------------------
// SESSION MANAGEMENT
// -----------------------------------------------------------------------------

struct SessionNewRequest {
    name: string
    cwd: string
}

struct Session {
    session_identifier: string
    name: string
    created_at: string
    cwd: string
    message_count: int
}

struct SessionListResponse {
    sessions: Session[]
}

struct SessionLoadRequest {
    session_identifier: string
}

struct SessionDeleteRequest {
    session_identifier: string
}

struct SuccessResponse {
    success: bool
}

method SessionNew {
    input: SessionNewRequest
    output: Session
}

method SessionList {
    input: Empty
    output: SessionListResponse
}

method SessionLoad {
    input: SessionLoadRequest
    output: Session
}

method SessionDelete {
    input: SessionDeleteRequest
    output: SuccessResponse
}

// -----------------------------------------------------------------------------
// CHAT MESSAGING
// -----------------------------------------------------------------------------

struct Attachment {
    type: string
    path: string
    content: string
}

struct ChatSendMessageRequest {
    session_identifier: string
    content: string
    attachments: Attachment[]
}

struct MessagePart {
    type: string
    text: string
    content: string
}

struct Message {
    identifier: string
    role: string
    parts: MessagePart[]
    created_at: string
}

struct ChatSendMessageResponse {
    message: Message
    session_identifier: string
}

struct ChatAbortRequest {
    message_id: string
}

struct ChatEditMessageRequest {
    session_identifier: string
    message_id: string
    new_content: string
}

method ChatSendMessage {
    input: ChatSendMessageRequest
    output: ChatSendMessageResponse
}

method ChatAbort {
    input: ChatAbortRequest
    output: SuccessResponse
}

method ChatEditMessage {
    input: ChatEditMessageRequest
    output: SuccessResponse
}

// -----------------------------------------------------------------------------
// FILE OPERATIONS
// -----------------------------------------------------------------------------

struct FileReadRequest {
    path: string
    encoding: string
}

struct FileReadResponse {
    content: string
    encoding: string
    size: int
}

struct FileWriteRequest {
    path: string
    content: string
    encoding: string
}

struct FileListRequest {
    path: string
    recursive: bool
    include_hidden: bool
}

struct FileInfo {
    name: string
    path: string
    type: string
    size: int
    modified: string
}

struct FileListResponse {
    files: FileInfo[]
}

struct FileDeleteRequest {
    path: string
}

method FileRead {
    input: FileReadRequest
    output: FileReadResponse
}

method FileWrite {
    input: FileWriteRequest
    output: SuccessResponse
}

method FileList {
    input: FileListRequest
    output: FileListResponse
}

method FileDelete {
    input: FileDeleteRequest
    output: SuccessResponse
}

// -----------------------------------------------------------------------------
// LSP (Language Server Protocol)
// -----------------------------------------------------------------------------

struct LspStartRequest {
    language: string
    cwd: string
}

struct LspStartResponse {
    success: bool
    language: string
    server_name: string
}

struct LspDiagnosticsRequest {
    uri: string
}

struct Diagnostic {
    range: Range
    severity: int
    message: string
    source: string
    code: string
}

struct Range {
    start: Position
    end: Position
}

struct Position {
    line: int
    character: int
}

struct LspDiagnosticsResponse {
    diagnostics: Diagnostic[]
}

struct LspPositionRequest {
    file_path: string
    line: int
    character: int
}

struct HoverInfo {
    contents: string
    range: Range?
}

struct LspHoverResponse {
    hover: HoverInfo?
}

struct CompletionItem {
    label: string
    kind: int
    detail: string
    documentation: string
    insert_text: string
}

struct LspCompletionsResponse {
    completions: CompletionItem[]
}

struct Location {
    uri: string
    range: Range
}

struct LspLocationsResponse {
    locations: Location[]
}

struct CallHierarchyItem {
    name: string
    kind: int
    uri: string
    range: Range
    selection_range: Range
}

struct CallHierarchyResponse {
    item: CallHierarchyItem
    incoming: CallHierarchyItem[]
    outgoing: CallHierarchyItem[]
}

struct TypeHierarchyResponse {
    item: CallHierarchyItem
    supertypes: CallHierarchyItem[]
    subtypes: CallHierarchyItem[]
}

struct LspRenameRequest {
    file_path: string
    line: int
    character: int
    new_name: string
}

struct TextEdit {
    range: Range
    new_text: string
}

struct LspRenameResponse {
    changes: TextEdit[]
}

struct LspWorkspaceSymbolsRequest {
    query: string
    file_type: string
}

struct SymbolInfo {
    name: string
    kind: int
    container_name: string
    location: Location
}

struct LspWorkspaceSymbolsResponse {
    symbols: SymbolInfo[]
}

method LspStart {
    input: LspStartRequest
    output: LspStartResponse
}

method LspDiagnostics {
    input: LspDiagnosticsRequest
    output: LspDiagnosticsResponse
}

method LspHover {
    input: LspPositionRequest
    output: LspHoverResponse
}

method LspCompletions {
    input: LspPositionRequest
    output: LspCompletionsResponse
}

method LspDefinition {
    input: LspPositionRequest
    output: LspLocationsResponse
}

method LspReferences {
    input: LspPositionRequest
    output: LspLocationsResponse
}

method LspCallHierarchy {
    input: LspPositionRequest
    output: CallHierarchyResponse
}

method LspTypeHierarchy {
    input: LspPositionRequest
    output: TypeHierarchyResponse
}

method LspRename {
    input: LspRenameRequest
    output: LspRenameResponse
}

method LspWorkspaceSymbols {
    input: LspWorkspaceSymbolsRequest
    output: LspWorkspaceSymbolsResponse
}

// -----------------------------------------------------------------------------
// MCP (Model Context Protocol)
// -----------------------------------------------------------------------------

struct McpServer {
    name: string
    status: string
    tools_count: int
}

struct McpListServersResponse {
    servers: McpServer[]
}

struct McpListToolsRequest {
    server_name: string
}

struct McpTool {
    name: string
    description: string
    input_schema: object
}

struct McpListToolsResponse {
    tools: McpTool[]
}

method McpListServers {
    input: Empty
    output: McpListServersResponse
}

method McpListTools {
    input: McpListToolsRequest
    output: McpListToolsResponse
}

// -----------------------------------------------------------------------------
// SETTINGS
// -----------------------------------------------------------------------------

struct SettingsResponse {
    model: string
    thinking_enabled: bool
    cwd: string
    theme: string
}

struct SettingsSetRequest {
    key: string
    value: object
}

method SettingsGet {
    input: Empty
    output: SettingsResponse
}

method SettingsSet {
    input: SettingsSetRequest
    output: SuccessResponse
}

// -----------------------------------------------------------------------------
// MODELS
// -----------------------------------------------------------------------------

struct Model {
    id: string
    name: string
    provider: string
    context_window: int
    supports_streaming: bool
    supports_vision: bool
}

struct ModelsListResponse {
    models: Model[]
}

method ModelsList {
    input: Empty
    output: ModelsListResponse
}

// -----------------------------------------------------------------------------
// AGENT
// -----------------------------------------------------------------------------

struct AgentRunRequest {
    session_id: string
    prompt: string
    attachments: Attachment[]
}

struct ContentPart {
    type: string
    text: string
}

struct AgentResult {
    response: object
    session_id: string
}

struct AgentSummarizeRequest {
    session_id: string
}

method AgentRun {
    input: AgentRunRequest
    output: AgentResult
}

method AgentSummarize {
    input: AgentSummarizeRequest
    output: SuccessResponse
}

// -----------------------------------------------------------------------------
// EMPTY TYPE (for methods with no input)
// -----------------------------------------------------------------------------

struct Empty {
}
