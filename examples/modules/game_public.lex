// Game Public Module
// This module is visible to everyone (developer + modders)

module game.public

// Public character definition - modders can use and extend this
public character Hero {
    name: "Arthur"
    health: 100
    mana: 50
    level: 1
}

// Public item definition
public item Sword {
    name: "Excalibur"
    damage: 50
    rarity: "legendary"
}

// Private helper - only visible in this file
private helper DamageCalculator {
    base_multiplier: 1.5
}
