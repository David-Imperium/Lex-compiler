import type { Translations } from '../types';

export const it: Translations = {
    nav: {
        home: 'Home',
        docs: 'Docs',
        playground: 'Playground',
        github: 'GitHub',
        sponsor: 'Sponsor',
    },
    hero: {
        tagline: '// Il Compilatore Databionico Nativo',
        description: 'Basta scrivere boilerplate infiniti o JSON illeggibili per i database dei tuoi giochi. Scrivi definizioni `.lex` eleganti e strettamente tipizzate e compilale all\'istante in C#, Lua o GDScript.',
        download: '[ SCARICA_V1.0.0 ]',
        docs: '[ LEGGI_LA_DOCUMENTAZIONE ]',
        status: 'SISTEMA.ONLINE :: v1.0.0',
    },
    problem: {
        badTitle: 'Il Problema: JSON / Dizionari disordinati',
        goodTitle: 'La Soluzione: Lo Schema Lex',
    },
    teaser: {
        title: 'Uplink_Transformer_Live',
        subtitle: 'Sperimenta la velocità di compilazione in tempo reale.',
        hint: 'Premi <Spazio> per mettere in pausa/riprendere',
    },
    stats: {
        backends: 'Backends',
        assertions: 'Asserzioni',
        hooks: 'Hook AI',
        foss: 'Open Source',
    },
    install: {
        title: 'Installazione_Sistema',
        win: 'WIN32',
        unix: 'UNIX',
        src: 'SORGENTE',
    },
    backends: {
        title: 'Target_dei_Moduli',
        subtitle: 'Mappatura Architettura -> Output',
        descLua: 'Qualsiasi host nativo Lua',
        descJson: 'Database astratto universale',
        descGdscript: 'Risorse Godot 4.x',
        descCsharp: 'ScriptableObject di Unity',
        descLove2d: 'Framework LÖVE',
        descDefold: 'Script motore Defold',
    },
    features: {
        title: 'Feature_Principali',
        agnosticDesc: 'Descrivi i contenuti del gioco in modo logico. Dì addio al codice boilerplate e ai dizionari illeggibili.',
        speedDesc: 'Compilato in puro C++. Zero dipendenze a runtime. Analizza migliaia di nodi prima che tu possa sbattere le palpebre.',
        safeDesc: 'Rileva errori di battitura, riferimenti interrotti ed errori logici in fase di compilazione, non mentre giochi.',
        aiDesc: 'Utility integrate per tradurre i file `.lex` in rappresentazioni testuali per gli agenti AI tramite query.',
        customDesc: 'Tower defense, GDR, visual novel. Usa `--types` per definire esattamente i tipi di nodi di cui hai bisogno.',
        rawDesc: 'Niente pip, niente npm, niente setup da incubo. Un solo eseguibile per dominare l\'intera pipeline dei contenuti.',
    },
    cta: {
        title: 'Inizializzare_Connessione?',
        subtitle: 'stato = "in attesa di comandi"',
        github: '[ // INIT _ GITHUB ]',
        download: '[ SCARICA_V1.0.0 ]',
        sponsor: '[ ❤️ SPONSOR ]',
        roadmap: 'sys.roadmap()',
    },
    footer: {
        sysStatus: 'STATO.SISTEMA',
        tagline1: 'Basta codice boilerplate.',
        tagline2: 'Inizia a descrivere mondi.',
        resources: 'Risorse',
        tutorial: 'Tutorial',
        apiRef: 'Riferimento API',
        project: 'Progetto',
        releases: 'Rilasci',
        roadmap: 'Roadmap',
        issues: 'Segnalazioni (Issues)',
        license: 'Licenza MIT',
        community: 'Community',
        repo: 'Repository GitHub',
        sponsor: '❤ Sponsorizza David',
        rights: 'David (Motore di Gioco Imperium). Tutti i diritti riservati.',
    },
    tiers: {
        title: 'Livelli_di_Accesso',
        subtitle: 'Scegli il tuo livello di accesso',
        earlyBird: 'EARLY BIRD',
        base: {
            name: 'Base',
            price: 'GRATIS',
            period: '',
            desc: 'Per modder e hobbisti',
            features: [
                '6 backends (Lua, JSON, Godot, Unity, LÖVE2D, Defold)',
                'Sistema a Moduli',
                'Filtro di visibilità',
                'Validazione guidata dallo schema',
                'Generatore di contesto AI',
                'Licenza MIT',
            ],
            cta: 'Inizia Ora',
        },
        core: {
            name: 'Core',
            price: '$49',
            period: 'a vita',
            desc: 'Per sviluppatori indie',
            features: [
                'Tutto ciò che è in Base',
                '+9 backend premium',
                'Unreal Engine',
                'GameMaker (in arrivo)',
                'Rust, C++, Python (in arrivo)',
                'Progetti multi-file',
                'Espressioni avanzate',
                'Prezzo lancio anticipato',
            ],
            cta: 'Ottieni Core',
        },
    },
    faq: {
        title: 'FAQ',
        items: [
            {
                q: 'Perché usare Lex invece di scrivere JSON/Lua direttamente?',
                a: 'Scrivi una volta, compili per qualsiasi motore. Cambi motore? Ricompili. Nessuna traduzione manuale. Nessun errore di copia e incolla. Il tuo file .lex è l\'unica fonte di verità.'
            },
            {
                q: 'Perché non usare semplicemente un foglio di calcolo?',
                a: 'I fogli di calcolo sono ottimi per i dati, ma non offrono validazione dei tipi, riferimenti tra tabelle, filtri di visibilità o generazione di codice. Sono anche difficili da tracciare con Git. Lex ti offre tutto questo e compila in codice reale.'
            },
            {
                q: 'In cosa Lex è diverso da altri DSL?',
                a: 'Nessun lock-in: l\'output è codice standard, non un formato proprietario. Multi-target: un file compila per più di 6 motori. Agnostico sugli schemi: definisci i tuoi tipi, non è hardcoded per nessun gioco in particolare. Offline: nessuna API cloud, funziona in locale.'
            },
            {
                q: 'Lex è solo per i videogiochi?',
                a: 'Principalmente, ma non esclusivamente. Qualsiasi progetto che necessita di definizioni di dati dichiarative, formati di output multipli, validazioni dei tipi e riferimenti incrociati può usare Lex (es. file di configurazione, CMS, schemi API).'
            },
            {
                q: 'E se il mio motore non è supportato?',
                a: 'I backend gratuiti come lua e json funzionano ovunque. I backend Core stanno per aggiungere altri motori. Puoi anche implementare un backend personalizzato in C++.'
            },
            {
                q: 'Lex è pronto per la produzione?',
                a: 'Sì. Lex (gratuito) è stabile con oltre 35 test e usato in progetti in produzione. I backend Core sono più recenti ma seguono gli stessi standard di qualità.'
            },
            {
                q: 'Posso usare la versione Base a scopo commerciale?',
                a: 'Sì! La licenza MIT consente qualsiasi uso: personale, commerciale, open source. Nessuna attribuzione richiesta.'
            },
            {
                q: 'Ogni membro del team ha bisogno di una licenza?',
                a: 'Sì, ogni sviluppatore ha bisogno della propria licenza per il livello Core. Contattaci per prezzi aziendali (5+ postazioni).'
            },
            {
                q: 'La licenza a vita ha una scadenza?',
                a: 'Mai. Paghi una volta, la usi per sempre. Ciò include tutti i futuri aggiornamenti dei backend Core.'
            },
            {
                q: 'Serve internet per usare Lex?',
                a: 'No. Lex funziona completamente offline. La verifica della licenza è locale.'
            }
        ]
    }
};
