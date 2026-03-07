import type { Translations } from '../types';

export const en: Translations = {
    nav: {
        home: 'Home',
        docs: 'Docs',
        playground: 'Playground',
        github: 'GitHub',
        sponsor: 'Sponsor',
    },
    hero: {
        tagline: '// The Native Databionic Compiler',
        description: 'Stop writing endless boilerplate or unreadable JSON for your game databases. Write elegant, strictly-typed `.lex` definitions and compile them instantly to C#, Lua, or GDScript.',
        download: '[ DOWNLOAD_V1.0.0 ]',
        docs: '[ VIEW_DOCUMENTATION ]',
        status: 'SYS.ONLINE :: v1.0.0',
    },
    problem: {
        badTitle: 'The Problem: Messy JSON/Dictionaries',
        goodTitle: 'The Solution: Lex Schema',
    },
    teaser: {
        title: 'Live_Transformer_Uplink',
        subtitle: 'Experience the compilation speed in real-time.',
        hint: 'Press <Space> to pause/resume compilation',
    },
    stats: {
        backends: 'Backends',
        assertions: 'Assertions',
        hooks: 'AI Hooks',
        foss: 'FOSS',
    },
    install: {
        title: 'System_Install',
        win: 'WIN32',
        unix: 'UNIX',
        src: 'SRC_BUILD',
    },
    backends: {
        title: 'Module_Targets',
        subtitle: 'Compile architecture -> output mapping',
        descLua: 'Any Lua native host',
        descJson: 'Universal abstract DB',
        descGdscript: 'Godot 4.x Resources',
        descCsharp: 'Unity ScriptableObject',
        descLove2d: 'LÖVE Framework',
        descDefold: 'Defold Engine scripts',
    },
    features: {
        title: 'Core_Features',
        agnosticDesc: 'Describe game content logically. Say goodbye to boilerplate code and unreadable dictionaries.',
        speedDesc: 'Compiled in pure C++. Zero runtime deps. It parses thousands of nodes before you can blink.',
        safeDesc: 'Catches typos, broken references, and logic errors at compile time, not at play time.',
        aiDesc: 'Built-in utilities to translate `.lex` files into agentic memory representations via queries.',
        customDesc: 'Tower defense, RPG, visual novels. Use `--types` to define exactly the node types you need.',
        rawDesc: 'No pip, no npm, no nightmare setups. One executable to rule your entire content pipeline.',
    },
    cta: {
        title: 'Initialize Connection?',
        subtitle: 'status = "awaiting command"',
        github: '[ // INIT _ GITHUB ]',
        download: '[ DOWNLOAD_V1.0.0 ]',
        sponsor: '[ ❤️ SPONSOR ]',
        roadmap: 'sys.roadmap()',
    },
    footer: {
        sysStatus: 'SYS.STATUS',
        tagline1: 'Stop writing boilerplate.',
        tagline2: 'Start describing worlds.',
        resources: 'Resources',
        tutorial: 'Tutorial',
        apiRef: 'API Reference',
        project: 'Project',
        releases: 'Releases',
        roadmap: 'Roadmap',
        issues: 'Issue Tracker',
        license: 'MIT License',
        community: 'Community',
        repo: 'GitHub Repository',
        sponsor: '❤ Sponsor David',
        rights: 'David (Imperium Game Engine). All rights reserved.',
    },
    tiers: {
        title: 'Access_Tiers',
        subtitle: 'Choose your level of access',
        earlyBird: 'EARLY BIRD',
        base: {
            name: 'Base',
            price: 'FREE',
            period: '',
            desc: 'For modders and hobbyists',
            features: [
                '6 backends (Lua, JSON, Godot, Unity, LÖVE2D, Defold)',
                'Module system',
                'Visibility filtering',
                'Schema-driven validation',
                'AI context generator',
                'MIT License',
            ],
            cta: 'Get Started',
        },
        core: {
            name: 'Core',
            price: '$49',
            period: 'lifetime',
            desc: 'For indie developers',
            features: [
                'Everything in Base',
                '+9 premium backends',
                'Unreal Engine',
                'GameMaker (coming)',
                'Rust, C++, Python (coming)',
                'Multi-file projects',
                'Advanced expressions',
                'Early bird pricing',
            ],
            cta: 'Get Core',
        },
    },
    faq: {
        title: 'FAQ',
        items: [
            {
                q: 'Why use Lex instead of writing JSON/Lua directly?',
                a: 'You write once, compile to any engine. Change your engine? Recompile. No manual translation. No copy-paste errors. Your .lex file is the single source of truth.'
            },
            {
                q: 'Why not just use a spreadsheet?',
                a: 'Spreadsheets are great for data, but offer no type validation, no references between tables, no visibility filtering, and no code generation. They are also hard to version control. Lex gives you all of that + compiles to real code.'
            },
            {
                q: 'How is Lex different from other DSLs?',
                a: 'No lock-in: Output is standard code, not a proprietary format. Multi-target: One file compiles to 6+ engines. Schema-agnostic: Define your own types, not hardcoded to any game. Offline: No cloud, no API, runs locally.'
            },
            {
                q: 'Is Lex only for games?',
                a: 'Primarily, but not exclusively. Any project that needs declarative data definitions, multiple output formats, type validation, and cross-references can use Lex (e.g., configuration files, content management, API schemas).'
            },
            {
                q: 'What if my engine isn\'t supported?',
                a: 'Free backends like lua and json work everywhere. Core backends are adding more engines. You can also implement a custom backend in C++.'
            },
            {
                q: 'Is Lex production-ready?',
                a: 'Yes. Lex (free) is stable with 35+ tests and used in production projects. Core backends are newer but follow the same quality standards.'
            },
            {
                q: 'Can I use Base commercially?',
                a: 'Yes! The MIT license allows any use — personal, commercial, open source. No attribution required.'
            },
            {
                q: 'Does every team member need a license?',
                a: 'Yes, each developer needs their own license for the Core tier. Contact us for team pricing (5+ seats).'
            },
            {
                q: 'Does the lifetime license expire?',
                a: 'Never. Pay once, use forever. This includes all future updates to Core backends.'
            },
            {
                q: 'Do I need internet to use Lex?',
                a: 'No. Lex works completely offline. License verification is local.'
            }
        ]
    }
};
