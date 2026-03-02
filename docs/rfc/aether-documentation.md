# Aether — Il Layer AI di Imperium

> *"Aether non parla. Osserva, ricorda, e sussurra a chi sa ascoltare."*

Aether è il layer dedicato alla gestione dell'intelligenza artificiale in Imperium. Si posiziona tra **Lex** e il modello LLM locale, gestendo due agenti distinti con scopi completamente diversi: l'**Archivista**, il companion narrativo del giocatore, e la **God AI**, il sistema di osservazione intelligente rivolto allo sviluppatore.

---

## Indice

1. [Cos'è Aether](#cosè-aether)
2. [Architettura](#architettura)
3. [I Due Agenti](#i-due-agenti)
   - [Archivista](#archivista)
   - [God AI](#god-ai)
4. [Integrazione con Lex](#integrazione-con-lex)
5. [Configurazione](#configurazione)
6. [Il Sistema di Report](#il-sistema-di-report)
7. [Memoria e Contesto](#memoria-e-contesto)
8. [Visione](#visione)
9. [Reference Completo](#reference-completo)
10. [Errori Comuni](#errori-comuni)

---

## Cos'è Aether

Aether nasce da un principio semplice: **Lex non dovrebbe sapere come funziona un modello AI, solo cosa vuole da esso.**

Senza Aether, integrare un LLM in Imperium significherebbe scrivere prompt a mano, gestire manualmente il contesto, ottimizzare i token, e collegare il modello a ogni singolo evento del gioco. Un lavoro ripetitivo, fragile, e difficile da mantenere.

Aether risolve tutto questo in un layer dedicato. Lex dichiara le intenzioni, Aether si occupa dell'esecuzione.

```
┌─────────────────────────────────────┐
│              Lex                    │  ← logica di gioco e intenzioni
└─────────────────┬───────────────────┘
                  │
┌─────────────────▼───────────────────┐
│             Aether                  │  ← gestione AI
│                                     │
│   ┌─────────────┐  ┌─────────────┐  │
│   │  Archivista │  │   God AI    │  │
│   └─────────────┘  └─────────────┘  │
└─────────────────┬───────────────────┘
                  │
┌─────────────────▼───────────────────┐
│           Ollama + LLM              │  ← modello locale
└─────────────────────────────────────┘
```

### Cosa fa Aether

- Gestisce la comunicazione con Ollama
- Costruisce il contesto automaticamente dallo stato di gioco
- Ottimizza i token per modelli leggeri
- Gestisce la memoria selettiva degli agenti
- Coordina la visione per l'Archivista
- Genera e salva i report della God AI
- Isola il resto del sistema dai dettagli implementativi del modello

### Cosa NON fa Aether

- Non modifica mai lo stato del gioco direttamente
- Non scrive variabili, non spawna eventi, non altera dati
- Non è visibile al giocatore in alcun modo
- Non dipende da API esterne o servizi cloud

---

## Architettura

Aether è composto da quattro componenti interni:

**Context Builder** — raccoglie e filtra lo stato del gioco prima di passarlo al modello. Decide cosa è rilevante in base all'agente e alla situazione corrente. È il componente più critico perché la qualità del contesto determina la qualità delle risposte.

**Token Manager** — ottimizza il contesto per rientrare nei limiti del modello leggero. Applica priorità, tronca il secondario, e comprime la storia quando necessario.

**Memory Store** — mantiene la memoria persistente degli agenti tra un turno e l'altro. Gestisce cosa ricordare, cosa dimenticare, e cosa ricordare sempre indipendentemente dal tempo passato.

**Report Engine** — raccoglie le osservazioni della God AI e genera report strutturati e leggibili. Salva tutto in formato testo nella cartella `/logs`.

```
┌────────────────────────────────────────────────────┐
│                     Aether                         │
│                                                    │
│  ┌──────────────────┐    ┌──────────────────────┐  │
│  │  Context Builder │    │    Token Manager     │  │
│  └────────┬─────────┘    └──────────┬───────────┘  │
│           │                         │              │
│  ┌────────▼─────────────────────────▼───────────┐  │
│  │              Ollama Bridge                   │  │
│  └────────┬─────────────────────────┬───────────┘  │
│           │                         │              │
│  ┌────────▼─────────┐    ┌──────────▼───────────┐  │
│  │   Memory Store   │    │    Report Engine     │  │
│  └──────────────────┘    └──────────────────────┘  │
└────────────────────────────────────────────────────┘
```

---

## I Due Agenti

Aether gestisce due agenti con natura e scopo completamente diversi. Non sono intercambiabili — hanno modelli, contesti, e responsabilità separate.

---

### Archivista

L'Archivista è il companion narrativo del giocatore. Lo accompagna per tutta la partita, commenta le scelte, racconta la lore, e reagisce agli eventi del mondo. È l'unico agente visibile al giocatore.

#### Caratteristiche

**Ruolo:** Narratore e companion  
**Visibile al giocatore:** Sì  
**Modello consigliato:** Modello leggero con supporto visione  
**Priorità:** Qualità narrativa, coerenza emotiva, risposte brevi e evocative

#### Comportamento

L'Archivista non risponde mai con dati tecnici o numeri. Il suo output è sempre narrativo — frasi brevi, evocative, coerenti con l'atmosfera dell'era corrente. Sa tutto della storia della civiltà del giocatore e la usa attivamente nelle sue risposte.

Non ripete mai le stesse frasi nella stessa partita. Aether gestisce internamente un registro delle risposte già date per evitare ripetizioni.

#### Contesto che riceve

Il Context Builder assembla automaticamente queste informazioni prima di ogni risposta dell'Archivista:

**Contesto prioritario — sempre incluso:**
- Era corrente e atmosfera
- Ultima scelta fatta dal giocatore
- Segreti attivi in questa partita
- Evento o trigger che ha generato la risposta

**Contesto secondario — incluso se i token lo permettono:**
- Scelte degli ultimi 3 turni
- Tecnologia appena scoperta
- Struttura appena costruita
- Stato delle relazioni diplomatiche

**Mai incluso — troppo per un modello leggero:**
- Storia completa della partita
- Tutti i flag narrativi
- Dati economici dettagliati

#### Memoria

L'Archivista ricorda selettivamente. Alcune cose vengono conservate per tutta la partita, altre dimenticate dopo un certo numero di turni.

**Ricorda sempre:**
- Scelte canoniche (quelle che portano ai finali)
- Segreti sbloccati
- Ere completate
- Flag narrativi principali

**Dimentica dopo 10 turni:**
- Commenti su strutture specifiche
- Reazioni a eventi minori
- Dialoghi di routine

#### Visione

L'Archivista può vedere lo schermo di gioco e commentare quello che osserva visivamente. Questo gli permette di notare cose che un prompt statico non potrebbe mai fare — la disposizione delle città, la densità delle strutture, l'espansione del territorio.

La visione è opzionale e configurabile. Per macchine con hardware limitato può essere disattivata senza perdere le funzionalità narrative di base.

---

### God AI

La God AI è il sistema di osservazione intelligente di Imperium. Non interagisce mai con il giocatore — è uno strumento rivolto esclusivamente allo sviluppatore. Osserva, analizza, e riporta.

#### Caratteristiche

**Ruolo:** Osservatore e analista  
**Visibile al giocatore:** Mai  
**Modello consigliato:** Modello leggero analitico, visione non necessaria  
**Priorità:** Precisione, dati strutturati, report leggibili

#### Il principio fondamentale

La God AI **non modifica mai nulla**. Non ha permessi di scrittura sullo stato del gioco. Il suo unico output sono i report. Questa limitazione è intenzionale — un modello leggero non ha l'affidabilità necessaria per prendere decisioni autonome sul bilanciamento. Osserva e suggerisce, la decisione finale è sempre dello sviluppatore.

#### Cosa osserva

**Metriche di bilanciamento:**
- Oro e risorse accumulate per turno
- Velocità di espansione della civiltà
- Durata delle ere in turni
- Difficoltà percepita nel tempo
- Distribuzione delle risorse strategiche
- Frequenza delle scelte diplomatiche vs militari

**Anomalie tecniche:**
- Crash e eccezioni con contesto completo
- Valori fuori range nelle variabili di gioco
- Transizioni tra ere fallite o incomplete
- Flag narrativi in conflitto tra loro
- Comportamenti inattesi del motore fisico o del render

**Progressione narrativa:**
- Flag narrativi impostati e loro coerenza
- Percorso verso i finali canonici
- Segreti sbloccati o bloccati per scelte incompatibili

#### Quando interviene

La God AI non gira in continuo — questo sarebbe troppo oneroso per un modello leggero. Interviene in tre momenti precisi:

**A intervalli regolari** — ogni N turni configurabili genera un report periodico con lo stato generale della partita.

**Su anomalia rilevata** — quando una metrica supera le soglie configurate, genera immediatamente un report focalizzato sul problema specifico.

**A fine sessione** — quando il giocatore chiude la partita, genera un report riassuntivo dell'intera sessione.

La God AI non interviene mai durante una cinematica o una transizione tra ere — aspetta che il gioco sia in stato stabile.

---

## Integrazione con Lex

Da Lex, entrambi gli agenti si usano in modo dichiarativo e semplice. Aether gestisce tutto il resto invisibilmente.

```lex
// Configurazione dell'Archivista in Lex
agent Archivista {
    usa: Aether.Archivista
    
    quando giocatore.entra(era.Steampunk) {
        contesto_extra: [atmosfera.era, ultima_guerra]
        genera: commento_entrata_era
    }
    
    quando civiltà.scopre(tecnologia.MotoreVapore) {
        genera: commento_tecnologia
    }
    
    quando segreto_attivato(ConspirazioneIndustriale) {
        genera: commento_segreto
        tono: misterioso
    }
}

// Configurazione della God AI in Lex
agent GodAI {
    usa: Aether.GodAI
    
    ogni 10_turni {
        genera: report_bilanciamento
    }
    
    quando anomalia_rilevata {
        genera: report_anomalia
    }
    
    quando sessione.termina {
        genera: report_sessione_completa
    }
}
```

---

## Configurazione

Aether si configura tramite un file `aether.config` nella cartella principale di Imperium.

```aether
// Configurazione globale
aether {
    runtime: ollama
    endpoint: "http://localhost:11434"
    agenti: [Archivista, GodAI]
    log_livello: standard        // standard | verboso | silenzioso
    log_cartella: /logs/aether/
}

// Configurazione Archivista
agente Archivista {
    modello: "llava"
    limite_token: 512
    temperatura: 0.7             // più alta = più creativo
    
    memoria {
        tipo: selettiva
        dimentica_dopo: 10_turni
        ricorda_sempre: [scelte_canoniche, segreti_sbloccati, ere_completate]
    }
    
    visione {
        attiva: true
        risoluzione: bassa
        focus: area_di_gioco
    }
    
    cache {
        attiva: true
        durata: 5_turni          // riusa risposte simili per N turni
    }
}

// Configurazione God AI
agente GodAI {
    modello: "llama3.2"
    limite_token: 1024
    temperatura: 0.2             // più bassa = più preciso e deterministico
    formato_risposta: testo
    
    intervalli {
        report_periodico: ogni_10_turni
        mai_durante: [cinematica, transizione_era]
    }
    
    soglie {
        // Bilanciamento economico
        oro_per_turno > 500          → "Economia troppo generosa"
        risorse_strategiche < 2      → "Risorse troppo scarse"
        
        // Ritmo di gioco
        durata_era > 80_turni        → "Era troppo lunga"
        durata_era < 15_turni        → "Era troppo corta"
        
        // Difficoltà
        difficoltà_percepita < 2     → "Gioco troppo facile"
        difficoltà_percepita > 8     → "Gioco troppo difficile"
        
        // Espansione
        espansione_velocità > 3      → "Espansione troppo rapida"
        espansione_velocità < 0.5    → "Espansione bloccata"
    }
    
    report {
        cartella: /logs/godai/
        formato: markdown
        mantieni_ultimi: 50          // conserva gli ultimi N report
    }
}
```

---

## Il Sistema di Report

I report della God AI sono il prodotto principale di Aether lato sviluppatore. Sono scritti in linguaggio naturale, leggibili senza dover analizzare log tecnici.

### Struttura di un Report

Ogni report ha tre sezioni fisse:

**Bilanciamento** — metriche economiche e di gioco con segnalazioni e suggerimenti.

**Anomalie** — bug, crash, e comportamenti inattesi con contesto completo della situazione di gioco al momento dell'errore.

**Generale** — osservazioni narrative sulla sessione, comportamento del giocatore, e note sulla progressione narrativa.

### Esempio di Report Periodico

```
=== AETHER — GOD AI REPORT ===
Turno: 45 | Era: Steampunk | Data: 2025-03-01 14:32

──────────────────────────────
BILANCIAMENTO
──────────────────────────────

⚠ ATTENZIONE — Economia sbilanciata
  Oro accumulato per turno: 480 (soglia: 500)
  Tendenza crescente negli ultimi 8 turni.
  Suggerimento: Considera di aumentare il costo di
  mantenimento delle strutture industriali del 15-20%
  oppure introdurre un evento di crisi economica
  nell'era Steampunk avanzata.

⚠ ATTENZIONE — Era troppo lunga
  Era Steampunk attiva da 42 turni (ottimale: 25-30).
  Il giocatore ha rallentato la ricerca tecnologica
  privilegiando l'espansione economica.
  Suggerimento: Valuta se ridurre il costo della
  tecnologia che innesca la transizione, oppure
  aggiungere un evento narrativo che spinga avanti.

✓ Difficoltà nella norma (5.2/10)
✓ Espansione territoriale nella norma

──────────────────────────────
ANOMALIE
──────────────────────────────

✓ Nessuna anomalia rilevata in questa sessione.

──────────────────────────────
GENERALE
──────────────────────────────

Il giocatore ha adottato una strategia prevalentemente
economica — 8 strutture commerciali su 12 totali.
Nessuna struttura militare costruita finora.

Flag narrativi attivi: flag_alleanza_oscura
Questo flag potrebbe interferire con il percorso
verso il finale Aurora se il giocatore non completa
l'evento ConspirazioneIndustriale prima della
transizione all'era Moderna.

Sessione stabile. Nessuna criticità urgente.
```

### Esempio di Report di Crash

```
=== AETHER — GOD AI ANOMALIA RILEVATA ===
Turno: 67 | Era: Moderna | Data: 2025-03-01 16:14

──────────────────────────────
ANOMALIA — CRASH RILEVATO
──────────────────────────────

✗ CRITICO — Crash durante transizione era
  Errore: Transizione Moderna → Atompunk fallita

  Contesto al momento del crash:
  - Strutture attive: 14 (3 di superficie, 11 avanzate)
  - Flag attivi: flag_guerra_nucleare, flag_alleanza_oscura
  - Tensione globale: 92
  - Risorse in elaborazione: [Acciaio, Carbone, Uranio]
  - Ultimo evento: GuerraFredda.opzione_Guerra confermata

  Possibile causa:
  La transizione tenta di distruggere le strutture
  di superficie mentre Uranio è ancora in coda
  di elaborazione. Possibile race condition tra
  il sistema di distruzione strutture e il
  gestore delle risorse.

  Riproduzione probabile:
  Avvia la transizione con risorse strategiche
  ancora in elaborazione nel turno corrente.
```

---

## Memoria e Contesto

La gestione della memoria è uno degli aspetti più critici di Aether — i modelli leggeri hanno contesti limitati, quindi ogni token deve guadagnarsi il suo posto.

### Livelli di Priorità

Aether assegna automaticamente tre livelli di priorità ai dati di contesto:

**Priorità Alta — sempre inclusa:** informazioni essenziali per la risposta corrente. Era corrente, trigger che ha generato la risposta, flag narrativi principali.

**Priorità Media — inclusa se i token lo permettono:** informazioni utili ma non essenziali. Scelte recenti, tecnologie scoperte di recente, stato diplomatico.

**Priorità Bassa — inclusa solo se lo spazio lo consente:** informazioni di background. Storia economica, statistiche di espansione, eventi minori passati.

### Compressione del Contesto

Quando il contesto supera il limite di token, Aether applica la compressione in questo ordine:

1. Rimuove le informazioni a priorità bassa
2. Comprime la storia recente in un sommario breve
3. Mantiene intatte le informazioni a priorità alta
4. Se ancora necessario, riduce il sommario al minimo indispensabile

### Cache delle Risposte

Aether evita di chiamare il modello per situazioni molto simili a quelle recenti. Se il giocatore entra in una città che ha già visitato 2 turni fa senza che nulla sia cambiato, Aether riusa la risposta precedente con piccole variazioni invece di generarne una nuova.

---

## Visione

La visione permette all'Archivista di commentare lo stato visivo del gioco — non solo i dati, ma quello che il giocatore vede sullo schermo.

### Come funziona

Aether cattura uno screenshot dell'area di gioco e lo passa al modello insieme al contesto testuale. Il modello può quindi notare cose come la densità delle strutture in una città, la forma del territorio espanso, o la posizione delle unità militari.

### Casi d'uso

```lex
// L'Archivista commenta l'aspetto visivo della città
quando giocatore.apre_vista_città {
    usa_visione: true
    genera: commento_visivo_città
}

// Commenta il territorio dopo una grande espansione
quando civiltà.espande(nuovi_territori >= 5) {
    usa_visione: true
    genera: commento_espansione
}
```

### Configurazione Hardware

La visione ha un costo computazionale. Per macchine con hardware limitato:

```aether
visione {
    attiva: true
    risoluzione: bassa           // bassa | media — mai alta per modelli leggeri
    focus: area_di_gioco         // cattura solo l'area rilevante, non tutto lo schermo
    max_per_turno: 2             // massimo N chiamate con visione per turno
    fallback_senza_visione: true // se la visione fallisce, risponde senza
}
```

---

## Reference Completo

### Keyword di Aether

| Keyword | Descrizione |
|---|---|
| `aether` | Blocco di configurazione globale |
| `agente` | Definisce un agente AI |
| `modello` | Specifica il modello Ollama da usare |
| `ruolo` | Tipo di agente: `narratore` o `osservatore` |
| `limite_token` | Massimo token per chiamata |
| `temperatura` | Creatività del modello (0.0 - 1.0) |
| `memoria` | Configurazione della memoria persistente |
| `visione` | Configurazione del modulo visivo |
| `cache` | Configurazione del caching delle risposte |
| `soglie` | Soglie di allerta per la God AI |
| `report` | Configurazione dei report |
| `intervalli` | Frequenza di intervento della God AI |
| `ogni` | Trigger temporale |
| `quando` | Trigger condizionale |
| `mai_durante` | Esclusioni temporali |
| `genera` | Tipo di output richiesto |
| `usa_visione` | Attiva la visione per questa chiamata |
| `contesto_extra` | Aggiunge contesto specifico alla chiamata |

### Tipi di Output

| Tipo | Agente | Descrizione |
|---|---|---|
| `commento_entrata_era` | Archivista | Commento narrativo all'ingresso in una nuova era |
| `commento_tecnologia` | Archivista | Reazione alla scoperta di una tecnologia |
| `commento_segreto` | Archivista | Reazione all'attivazione di un segreto |
| `commento_visivo_città` | Archivista | Commento visivo sulla città |
| `commento_espansione` | Archivista | Commento sull'espansione territoriale |
| `dialogo` | Archivista | Dialogo generico contestuale |
| `report_bilanciamento` | God AI | Report sulle metriche di bilanciamento |
| `report_anomalia` | God AI | Report su crash o comportamenti anomali |
| `report_sessione_completa` | God AI | Report riassuntivo della sessione |

### Valori di Temperatura

| Valore | Comportamento | Uso consigliato |
|---|---|---|
| 0.1 - 0.3 | Molto deterministico, preciso | God AI, dati strutturati |
| 0.5 - 0.7 | Bilanciato | Archivista in situazioni neutre |
| 0.8 - 1.0 | Creativo, variabile | Archivista in momenti drammatici |

---

## Errori Comuni

**AETHER_ERR_001 — Ollama non raggiungibile**
```
Aether non riesce a connettersi a Ollama.
Verifica che Ollama sia in esecuzione su localhost:11434
oppure aggiorna l'endpoint in aether.config.
```

**AETHER_ERR_002 — Modello non trovato**
```
Il modello "llava" non è installato in Ollama.
Esegui: ollama pull llava
oppure specifica un modello diverso in aether.config.
```

**AETHER_ERR_003 — Limite token superato**
```
Il contesto generato supera il limite configurato.
Aether ha applicato la compressione automatica.
Se il problema persiste, riduci contesto_secondario
oppure aumenta limite_token in aether.config.
```

**AETHER_ERR_004 — Visione non supportata**
```
Il modello configurato non supporta la visione.
Disattiva visione.attiva oppure usa un modello
con supporto multimodale (es. llava).
```

**AETHER_ERR_005 — Cartella report non trovata**
```
La cartella /logs/godai/ non esiste o non è accessibile.
Aether la crea automaticamente al primo avvio.
Se l'errore persiste, verifica i permessi della cartella.
```

---

## Note Finali

Aether è progettato per essere invisibile quando funziona bene. Il giocatore non sa che esiste — vede solo un Archivista che sembra conoscere davvero la sua storia. Lo sviluppatore non deve gestire prompt o contesti manualmente — apre i report e trova già tutto analizzato.

Questo è l'obiettivo di Aether: **togliere complessità, non aggiungerne.**

---

*Documentazione Aether v0.1 — Imperium Game Engine*
*Aether è un componente interno di Imperium e non è destinato all'uso esterno.*
