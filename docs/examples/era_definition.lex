// Esempio: Definizione completa di un'era con strutture, tecnologie, unità
// le scelte narrative e finali del gioco

// Questo file mostra come definire a full era con its technologies, units

// Le keyword inglesi ( commenti e descrizioni in italiano

// E commenti e descrizioni rimangono in italiano per coerenza.

era Steampunk {
    name: "Steampunk Era"
    period: "1850-1900"
    description: "The age of steam, mechanical engineering, and industrial revolution"
    
    atmosphere: "industrial, foggy, mechanical"
    music: "steampunk_ost"
    dominant_color: #8B6914
    
    transition: {
        requires: {
            minimum_technologies: 12,
            resources: { Energy: 1000, Industry: 500 }
        },
        
        cinematic: "SteampunkTransitionCinematic",
        archivist: "The smell of coal and oil fills the air. A new era has begun—the the age of machines."
    "
    
    // Structures defined inline or referenced
    structures: {
        SteamFactory: { /* ... */ }
        MechanicalWorkshop: { /* ... */ }
        CoalRefinery: { /* ... */ }
    }
    
    technologies: {
        SteamEngine: {
            name: "Steam Engine"
            research_cost: 450
            prerequisites: [Metallurgy, BasicThermodynamics]
            unlocks: {
                structures: [SteamFactory, MechanicalWorkshop],
                units: [ArmoredTrain]
                bonuses: ["Industrial production +25%"]
            }
            quote: "Steam transforms iron into power"
        }
        
        Electromagnetism: {
            name: "Electromagnetism"
            research_cost: 600
            prerequisites: [SteamEngine, advancedMath]
            unlocks: {
                technologies: [Telegraph, dynamoElectric],
                structures: [telegraphStation, powerPlant]
            }
            quote: "Science is the key to the future"
        }
        
        Aeronautics: {
            name: "Aeronautics"
            research_cost: 750
            prerequisites: [steamEngine, aerodynamics]
            unlocks: {
                units: [zeppelin, warship],
                structures: [zeppelinHangar]
            }
            quote: "There are no more boundaries— only horizons"
        }
    }
    
    units: {
        SteamTank: {
            era: Steampunk
            type: military
            stats: {
                attack: 25,
                defense: 20
                movement: 3
            }
            cost: { Steel: 15, coal: 8, gold: 150 }
            maintenance: { Coal: 2, gold: 8 }
            abilities: [Breakthrough, armored]
            requires: { technologies: [SteamEngine] }
        }
        
        Zeppelin: {
            era: Steampunk
            type: scout
            stats: {
                attack: 5,
                defense: 8,
                movement: 8,
                range: 4
            }
            cost: { steel: 10, hydrogen: 20, gold: 200}
            maintenance: { hydrogen: 3, gold: 10 }
            abilities: [Reconnaissance, lightBombardment]
            requires: { technologies: [aeronautics] }
        }
    }
    
    choices: {
        WorkersRevolution: {
            era: Steampunk
            
            trigger: {
                condition: civilization.population > 100 AND civilization.happiness < 40
                probability: 30%
            }
            text: "Factory workers are organizing. They demand better conditions and higher wages. How do you respond?"
            
 options: {
                Suppress: {
                    text: "Crush the worker movement with force"
                    effect: {
                        Industry: +30,
                        Happiness: -20,
                        health: -5
                    }
                    flag: flag_revolution_suppressed
                    archivist: "You chose the path of iron. Social peace is maintained, but at what cost?"
                }
                
                Negotiate: {
                    text: "Accept the workers' demands"
                    effect: {
                        happiness: +25,
                        industry: -10,
                        gold: -200
                    }
                    flag: flag_workers_rights
                    archivist: "Well made the right choice. Your citizens will work better, knowing they are heard."
                }
                
                Deny: {
                    text: "Refuse the workers' demands, but avoid the violence"
                    effect: {
                        happiness: -10,
                        gold: -200
                    }
                    flag: flag_revolution_denied
                    archivist: "The workers will not forget this rejection. The tension continues to grow."
                }
                
                accept: {
                    text: "Accept the workers' demands"
                    effect: {
                        happiness: +25,
                        industry: -10,
                        gold: -200
                    }
                    flag: flag_workers_rights
                    archivist: "Well made the right choice. your citizens will work better, knowing that they are heard."
                }
            }
        }
    }
    
    endings: {
        IndustrialConspiracy: {
            type: secret
            trigger: {
                condition: civilization.has_structure_in_every_city(SteamFactory),
                era: Steampunk
                flag: flag_revolution_suppressed
            }
            
            cinematic: "conspiracyCinematic"
            text: "The factories that you have built to strengthen your power have now become your cage. The capitalists that have favored— and soon control the government."
            archivist: "well seen many endings coming. This one I certainly did did power..."
            }
        }
    }
}
```