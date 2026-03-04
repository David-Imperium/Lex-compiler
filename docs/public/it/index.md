# Documentazione Lex

Un linguaggio domain-specific per logica di gioco e scripting.

## Panoramica

Lex è progettato per essere:
- **Semplice**: Sintassi pulita ispirata ai linguaggi moderni
- **Sicuro**: Validazione integrata che rileva errori prima del runtime
- **Flessibile**: Multipli backend di output (Lua, JSON, Godot)

## Documentazione

### Specifica
- [Riferimento Linguaggio](./spec/language.md) - Sintassi, tipi, keyword
- [Regole di Validazione](./spec/validation.md) - Cosa controlla lex_engine
- [Backend di Output](./spec/outputs.md) - Target di compilazione supportati

### Architettura
- [Layer del Sistema](./architecture/layers.md) - Base, Core, Neural

### Integrazioni
- [Imperium Engine](./integrations/imperium-engine.md) - Bridge FFI C++

### Guide
- [Iniziare](./guides/tutorial.md) - Tutorial rapido

## Esempio Rapido

```lex
fn main() {
    let x: int = 42;
    let nome: string = "Ciao";
    
    if x > 10 {
        spawn("worker_unit", x, 0, 0);
    }
}
```

## Repository

- **Compilatore**: `src/` - Lexer, parser, generatore codice
- **Engine Bridge**: `lex_engine/` - FFI per integrazione Imperium
- **Test**: `tests/` - Suite di test
