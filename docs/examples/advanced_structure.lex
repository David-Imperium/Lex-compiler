// Esempio avanzato: Struttura con proprieta multiple
// Questo file shows the main features of Lex currently supported

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

    description: "Converts coal into energy and industrial output"
}

// Note per modder:
// - La sintassi e dichiarativa: descrivi il cosa, non il come
// - Il compilatore genera automaticamente:
//   - Lua: scripts/lua/buildings/steam_factory.lua
//   - JSON: data/buildings/steam_factory.json
//   - React: ui/buildings/SteamFactoryPanel.tsx
//   - Lore: ai/lore/steamFactory.txt
