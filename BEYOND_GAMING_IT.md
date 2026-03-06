# Lex: Oltre i Videogiochi

## La Potenza Nascosta di un Transpiler Dichiarativo

Lex è nato nel mondo dello sviluppo di videogiochi, ma la sua architettura rivela qualcosa di molto più potente: **un compilatore universale da dichiarativo a codice**.

### L'Intuizione Fondamentale

La generazione di codice tradizionale richiede di imparare un nuovo DSL per ogni target:
- OpenAPI/Swagger per le API REST
- Protocol Buffers per RPC
- Prisma per i database
- Kubernetes YAML per l'infrastruttura

**E se potessi scrivere il tuo modello di dominio una volta sola, in un formato semplice e leggibile, e generare codice per TUTTI questi target?**

```lex
// Definisci il tuo dominio UNA VOLTA
struct Utente {
    id: int
    nome: string
    email: string
    creato_il: datetime
}

method GetUtente {
    input: int
    output: Utente
}

method CreaUtente {
    input: Utente
    output: Utente
}
```

### Generazione Multi-Target

```bash
# Una definizione, molti output
lexc api.lex -t python,go,typescript,sql,openapi,grpc
```

Questo singolo comando genera:
- **Python** dataclass e handler FastAPI
- **Go** struct e metodi
- **TypeScript** interfacce per il frontend
- **SQL** schema e migrazioni
- **OpenAPI** spec per la documentazione
- **gRPC** definizioni protobuf

### L'Architettura

```
                    ┌─────────────────┐
                    │   Tuo file      │
                    │   .lex          │
                    └────────┬────────┘
                             │
                    ┌────────▼────────┐
                    │    Parser Lex   │
                    │    (AST)        │
                    └────────┬────────┘
                             │
        ┌────────────────────┼────────────────────┐
        │                    │                    │
   ┌────▼────┐         ┌────▼────┐         ┌────▼────┐
   │ Python  │         │   Go    │         │   SQL   │
   │ Backend │         │ Backend │         │ Backend │
   └────┬────┘         └────┬────┘         └────┬────┘
        │                    │                    │
   ┌────▼────┐         ┌────▼────┐         ┌────▼────┐
   │  .py    │         │   .go   │         │  .sql   │
   └─────────┘         └─────────┘         └─────────┘
```

### Cosa Rende Possibile Tutto Questo

1. **Parser Agnostico allo Schema**: Lex analizza le definizioni senza sapere cosa rappresentano
2. **Backend Pluggabili**: Ogni backend è un generatore di codice autonomo
3. **Flag `--types`**: Definisci i tuoi tipi di dominio (`struct`, `method`, `table`, `message`...)
4. **Nessuna Dipendenza Runtime**: Il codice generato è standalone, nessuna libreria Lex richiesta

### Casi d'Uso nel Mondo Reale

#### Sviluppo API
Definisci la tua API una volta, genera codice server, SDK client e documentazione simultaneamente.

#### Schemi Database
Definisci le entità una volta, genera DDL SQL, modelli ORM e script di migrazione.

#### Infrastructure as Code
Definisci la tua infrastruttura in modo dichiarativo, genera Terraform, manifest Kubernetes o CloudFormation.

#### Protocol Buffers / gRPC
Genera definizioni protobuf E il codice di implementazione dalla stessa sorgente.

#### Macchine a Stati
Definisci stati e transizioni, genera implementazione in qualsiasi linguaggio.

### La Visione

**Scrivi una volta, genera ovunque.**

L'obiettivo non è sostituire strumenti specializzati come OpenAPI o Prisma. È avere un'unica **fonte di verità** per il tuo modello di dominio, con la capacità di generare per qualsiasi target tu abbia bisogno.

### Stato Attuale

| Target | Stato | Note |
|--------|-------|------|
| Lua | ✅ Stabile | Modding giochi |
| JSON | ✅ Stabile | Serializzazione dati |
| Godot (GDScript) | ✅ Stabile | Godot Engine |
| Unity (C#) | ✅ Stabile | Unity Engine |
| Love2D (Lua) | ✅ Stabile | Framework LÖVE |
| Defold (Lua) | ✅ Stabile | Defold Engine |
| RPC (Python/Go) | ✅ Nuovo | Protocolli JSON-RPC |

### Backend Futuri (Benvenuti Contributi)

- **OpenAPI/Swagger** - Documentazione API REST
- **TypeScript** - Interfacce frontend
- **Prisma/SQL** - Schemi database
- **gRPC/Protobuf** - Protocolli RPC
- **Kubernetes** - Infrastructure as code
- **GraphQL** - Generazione schema
- **Rust** - Programmazione di sistema
- **Python** - Dataclass, modelli Pydantic

### Come Contribuire con un Backend

```cpp
// src/codegen/tuo_backend.cpp
class TuoBackend : public Backend {
public:
    std::string name() const override { return "tuo_target"; }
    std::string file_extension() const override { return ".ext"; }
    std::string generate(const AST& ast) override {
        // La tua logica di generazione
    }
};

// Registralo
registry.register_factory("tuo_target", []() {
    return std::make_unique<TuoBackend>();
});
```

### Perché È Importante

1. **Consistenza**: Una fonte di verità, nessun problema di sincronizzazione tra API/server/client
2. **Velocità**: Genera boilerplate, concentrati sulla logica di business
3. **Flessibilità**: Cambia target senza riscrivere le definizioni
4. **Collaborazione**: Anche i non-sviluppatori possono leggere e scrivere file `.lex`
5. **Estensibilità**: Crea i tuoi backend per esigenze custom

---

## In Sostanza

**Lex non è solo un transpiler di contenuti per giochi.** È un nuovo modo di pensare alla generazione di codice: definizioni dichiarative che trascendono qualsiasi singolo target o linguaggio.

Inizia con lo sviluppo di giochi. Resta per la transpilazione universale.

---

*Vuoi creare un nuovo backend? Guarda `src/codegen/` per esempi, o apri una issue per discutere la tua idea.*
