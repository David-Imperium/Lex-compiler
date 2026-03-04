// Modder Content Example
// This demonstrates how modders can use public modules

use "game_public.lex"

// Modder can extend public definitions
character CustomHero {
    name: "My Custom Hero"
    health: 200
    mana: 100
    level: 10
}

// Modder can create new items
item CustomWeapon {
    name: "Modded Sword"
    damage: 75
    rarity: "epic"
}
