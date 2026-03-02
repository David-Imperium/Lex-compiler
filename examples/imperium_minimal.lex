// Imperium 2.0 - Minimal Game Data
// Generated for simulation testing
// Version: 0.1.0

// ============================================
// RESOURCES
// ============================================

resource Gold {
    name: "Gold"
    category: "currency"
    description: "Primary currency for construction and maintenance"
}

resource Wood {
    name: "Wood"
    category: "material"
    description: "Basic construction material"
}

resource Coal {
    name: "Coal"
    category: "fuel"
    description: "Fuel for steam machines and factories"
}

resource Steel {
    name: "Steel"
    category: "material"
    description: "Advanced construction material"
}

resource Energy {
    name: "Energy"
    category: "utility"
    description: "Power for advanced structures"
}

resource Industry {
    name: "Industry"
    category: "production"
    description: "Industrial output capacity"
}

resource Research {
    name: "Research"
    category: "knowledge"
    description: "Scientific research points"
}

// ============================================
// ERAS
// ============================================

era Ancient {
    name: "Ancient Era"
    period: "3000 BCE - 500 CE"
    description: "The dawn of civilization. Basic agriculture, simple tools, and the first cities."
    atmosphere: "pastoral, sunny, peaceful"
    music: "ancient_ost"
    dominant_color: #D4A574
}

era Steampunk {
    name: "Steampunk Era"
    period: "1850 - 1920"
    description: "The age of steam, mechanical engineering, and industrial revolution."
    atmosphere: "industrial, foggy, mechanical"
    music: "steampunk_ost"
    dominant_color: #8B6914
}

// ============================================
// STRUCTURES - Ancient Era
// ============================================

structure Farm {
    era: Ancient
    name: "Farm"
    description: "Produces food and basic resources for your civilization."
    
    cost: {
        Gold: 30,
        Wood: 15
    }
    
    production: {
        Gold: 5
    }
    
    maintenance: {
        Gold: 1
    }
}

structure Barracks {
    era: Ancient
    name: "Barracks"
    description: "Trains basic military units."
    
    cost: {
        Gold: 50,
        Wood: 25
    }
    
    maintenance: {
        Gold: 3
    }
}

structure Library {
    era: Ancient
    name: "Library"
    description: "Generates research points for technological advancement."
    
    cost: {
        Gold: 80,
        Wood: 40
    }
    
    production: {
        Research: 10
    }
    
    maintenance: {
        Gold: 5
    }
}

// ============================================
// STRUCTURES - Steampunk Era
// ============================================

structure SteamFactory {
    era: Steampunk
    name: "Steam Factory"
    description: "Converts coal into energy and industrial output."
    
    cost: {
        Coal: 8,
        Steel: 5,
        Gold: 50
    }
    
    production: {
        Energy: 15,
        Industry: 10
    }
    
    maintenance: {
        Coal: 2,
        Gold: 5
    }
}

structure ResearchLab {
    era: Steampunk
    name: "Research Laboratory"
    description: "Advanced research facility for modern technologies."
    
    cost: {
        Steel: 10,
        Gold: 100,
        Energy: 5
    }
    
    production: {
        Research: 25
    }
    
    maintenance: {
        Coal: 3,
        Gold: 10
    }
}

// ============================================
// UNITS - Ancient Era
// ============================================

unit Warrior {
    era: Ancient
    name: "Warrior"
    type: "infantry"
    description: "Basic infantry unit."
    
    attack: 5
    defense: 3
    movement: 2
    
    cost: {
        Gold: 20
    }
    
    maintenance: {
        Gold: 2
    }
}

unit Archer {
    era: Ancient
    name: "Archer"
    type: "ranged"
    description: "Ranged unit with good attack but weak defense."
    
    attack: 7
    defense: 1
    movement: 2
    
    cost: {
        Gold: 25,
        Wood: 5
    }
    
    maintenance: {
        Gold: 2
    }
}

// ============================================
// UNITS - Steampunk Era
// ============================================

unit SteamTank {
    era: Steampunk
    name: "Steam Tank"
    type: "armored"
    description: "Heavily armored steam-powered vehicle."
    
    attack: 15
    defense: 12
    movement: 1
    
    cost: {
        Steel: 15,
        Coal: 10,
        Gold: 80
    }
    
    maintenance: {
        Coal: 3,
        Gold: 8
    }
}

// ============================================
// TECHNOLOGIES
// ============================================

technology SteamEngine {
    era: Steampunk
    name: "Steam Engine"
    description: "Harness the power of steam for industry and transportation."
    
    research_cost: 100
    
    unlocks: [SteamFactory, SteamTank]
}

technology IndustrialChemistry {
    era: Steampunk
    name: "Industrial Chemistry"
    description: "Advanced chemical processes for industry."
    
    research_cost: 150
    prerequisites: [SteamEngine]
    
    unlocks: [ResearchLab]
}
