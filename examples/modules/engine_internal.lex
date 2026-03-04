// Lex Engine Internal Module
// This module is only visible to the engine developer, not to modders

module engine.internal

// Internal game state - modders cannot access this
internal GameState {
    turn: 1
    difficulty: "normal"
    max_players: 4
}

// Internal resource manager
internal ResourceManager {
    max_pool: 10000
    gc_interval: 60
}
