// =============================================================================
// Lex Showcase Example for Godot Forum
// Define game data once, generate for multiple engines
// =============================================================================

// A simple RPG example with items, enemies, and abilities

resource Item {
    label: "Item"
    name: "Potion"
    description: "Restores health"
    rarity: 1
    value: 50
    stackable: true
}

resource Weapon {
    label: "Weapon"
    name: "Iron Sword"
    damage: 25
    speed: 1.5
    range: 2.0
    durability: 100
}

resource Enemy {
    label: "Enemy"
    name: "Goblin"
    hp: 50
    damage: 10
    speed: 3.0
    xp_reward: 25
}

resource Player {
    label: "Player"
    name: "Hero"
    hp: 100
    max_hp: 100
    mp: 50
    max_mp: 50
    level: 1
    experience: 0
    gold: 0
}

resource Ability {
    label: "Ability"
    name: "Fireball"
    description: "Launches a fireball"
    mp_cost: 15
    cooldown: 2.5
    damage: 40
}
