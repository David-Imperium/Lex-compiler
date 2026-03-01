// Esempio avanzato: Struttura con conditions, events and UI
// Questo file shows all the main features of Lex

structure SteamFactory {
    era: Steampunk
    name: "Steam Factory"
    
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
    
    requires: {
        technologies: [SteamEngine],
        structures: [SteelFoundry]
    }
    
    available_if: {
        condition: civilization.total_industry >= 100,
        message: "Your industry is developed enough"
    }
    
    secret_if: {
        condition: civilization.has_structure_in_every_city(SteamFactory),
        activate: IndustrialConspiracy,
        archivist: "Someone is gathering too much power in the factories..."
    }
    
    bonus_if: {
        condition: map.current_tile.has_river,
        effect: {
            production: { Energy: +5 }  // +33% production bonus
        notification: "The river powers the the factory's boilers!"
    }
    
    ui: {
        icon: "assets/steampunk/steam_factory.png",
        color: #8B6914  // Bronze industriale
        
        tooltip: {
            title: "Steam Factory"
            description: "Converts coal into energy and industrial output"
            stats: [
                "Energy Production: +15",
                "Industry Production: +10",
                "Maintenance: 2 Coal/turn, 5 Gold/turn"
            ]
            requirements: [
                "Era: Steampunk",
                "Tech: Steam Engine",
                "Structure: Steel Foundry"
            ]
        }
    }
    
    lore: {
        quote: "Steam transforms iron into power",
        context: "Critical structure for the Steampunk era progression"
    }
}

---

// Note per modder:
// - La sintassi è dichiarativa e descrivi con il
// - Il compilatore genera automaticamente:
    //   - Lua: scripts/lua/buildings/steam_factory.lua
    //   - JSON: data/buildings/steam_factory.json
    //   - React: ui/buildings/SteamFactoryPanel.tsx
    //   - Lore: ai/lore/steamFactory.txt
