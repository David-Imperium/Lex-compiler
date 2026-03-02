# Lex Testing Strategy

**Version:** 0.1.0
**Last Updated:** 2026-03-02
**Status:** Draft

---

## Obiettivo

Validare che Lex produca output corretto senza toccare l'engine Imperium direttamente.

---

## Principio Fondamentale

> Non testiamo se l'engine funziona. Testiamo se Lex produce ciò che l'engine si aspetta.

---

## Strategia a 3 Livelli

### 1. Unit Tests (già esistenti)
Test di componenti isolati: lexer, parser, validator.

```
Lexer → "era Ancient { }" → tokens corretti
Parser → tokens → AST valido
Validator → AST → nessun errore semantico
```

**Stato:** 17 test con Catch2, funzionanti.

---

### 2. Snapshot Tests (da implementare)
Confrontiamo l'output generato con versioni di riferimento salvate.

```
examples/imperium_minimal.lex
    → lexc -t lua → output/imperium_minimal.lua
    → confronta con tests/snapshots/imperium_minimal.lua
```

**Vantaggi:**
- Rileva cambiamenti inaspettati nell'output
- Documenta il formato di output atteso
- Facile da mantenere (aggiorna snapshot quando cambiamento è intenzionale)

**Implementazione:**
```cpp
// tests/snapshot_tests.cpp

TEST_CASE("Snapshot - Lua Backend") {
    std::string source = read_file("examples/imperium_minimal.lex");
    
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    Parser parser(tokens);
    auto ast = parser.parse_file();
    
    LuaBackend backend;
    std::string output = backend.generate(*ast);
    
    std::string expected = read_file("tests/snapshots/imperium_minimal.lua");
    
    REQUIRE(output == expected);
}
```

**Comando per aggiornare snapshot:**
```bash
lexc examples/imperium_minimal.lex -t lua -o tests/snapshots/ --update-snapshots
```

---

### 3. Contract Tests (da implementare)
Verificano che l'output rispetti il "contratto" con l'engine.

Il contratto è: "se l'engine riceve questo JSON, deve poterlo caricare senza errori".

**Per Lua:**
```cpp
// tests/contract/lua_contract.cpp

TEST_CASE("Contract - Lua syntax valid") {
    std::string lua_output = generate_lua("examples/imperium_minimal.lex");
    
    // Usiamo sol2 o un parser Lua per validare la sintassi
    // SENZA eseguire il codice
    REQUIRE(is_valid_lua_syntax(lua_output));
}
```

**Per JSON:**
```cpp
// tests/contract/json_contract.cpp

TEST_CASE("Contract - JSON schema valid") {
    std::string json_output = generate_json("examples/imperium_minimal.lex");
    
    // Parse JSON
    auto json = nlohmann::json::parse(json_output);
    
    // Verifica schema
    REQUIRE(json.contains("structures"));
    REQUIRE(json.contains("units"));
    REQUIRE(json.contains("eras"));
    
    // Verifica che ogni struttura abbia i campi obbligatori
    for (const auto& structure : json["structures"]) {
        REQUIRE(structure.contains("id"));
        REQUIRE(structure.contains("name"));
        REQUIRE(structure.contains("era"));
    }
}
```

---

### 4. Integration Tests Simulati (da implementare)

Simuliamo ciò che l'engine farebbe con l'output, senza usare l'engine vero.

```cpp
// tests/integration/simulated_engine.cpp

class MockImperiumEngine {
public:
    bool load_structures(const std::string& lua_code) {
        // Simula il parsing che l'engine farebbe
        // Estrae le strutture definite
        
        auto structures = extract_definitions(lua_code, "Structure");
        
        for (const auto& s : structures) {
            if (!validate_structure(s)) {
                errors_.push_back("Invalid structure: " + s.id);
                return false;
            }
        }
        
        loaded_structures_ = structures;
        return true;
    }
    
    int count_structures() const { return loaded_structures_.size(); }
    
private:
    std::vector<StructureDef> loaded_structures_;
    std::vector<std::string> errors_;
};

TEST_CASE("Integration - Mock engine loads structures") {
    std::string lua = generate_lua("examples/imperium_minimal.lex");
    
    MockImperiumEngine engine;
    REQUIRE(engine.load_structures(lua));
    REQUIRE(engine.count_structures() == 5); // Farm, Barracks, Library, SteamFactory, ResearchLab
}
```

---

## Test fixtures

File `.lex` di test in `tests/fixtures/`:

```
tests/fixtures/
├── minimal_structure.lex      # Una singola struttura
├── all_resource_types.lex     # Tutti i tipi di risorsa
├── conditions_simple.lex      # Condizioni semplici
├── conditions_complex.lex     # Condizioni con and/or/not
├── cross_references.lex       # Riferimenti tra definizioni
├── error_missing_era.lex      # Errore: era non definita
├── error_duplicate_id.lex     # Errore: ID duplicato
└── edge_cases.lex             # Casi limite
```

---

## Schema JSON per Validazione

```json
// tests/schemas/structure.schema.json
{
  "$schema": "http://json-schema.org/draft-07/schema#",
  "type": "object",
  "required": ["id", "name", "era"],
  "properties": {
    "id": { "type": "string", "pattern": "^[A-Z][a-zA-Z0-9]*$" },
    "name": { "type": "string" },
    "era": { "type": "string" },
    "description": { "type": "string" },
    "cost": {
      "type": "object",
      "additionalProperties": { "type": "integer", "minimum": 0 }
    },
    "production": {
      "type": "object",
      "additionalProperties": { "type": "integer" }
    },
    "maintenance": {
      "type": "object",
      "additionalProperties": { "type": "integer", "minimum": 0 }
    }
  }
}
```

---

## Pipeline CI

```yaml
# .github/workflows/test.yml (futuro)

name: Test

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
      - uses: actions/checkout@v4
      
      - name: Build
        run: |
          cmake -B build -S . -DBUILD_TESTS=ON
          cmake --build build
      
      - name: Unit Tests
        run: ctest --test-dir build --output-on-failure
      
      - name: Snapshot Tests
        run: ./build/tests/snapshot_tests
        
      - name: Contract Tests
        run: ./build/tests/contract_tests
        
      - name: Check Snapshots Changed
        run: |
          git diff --exit-code tests/snapshots/
          if [ $? -ne 0 ]; then
            echo "Snapshots changed! Run locally and verify."
            exit 1
          fi
```

---

## Comandi di Test

```bash
# Tutti i test unit
cmake --build build
ctest --test-dir build

# Solo snapshot tests
./build/tests/snapshot_tests

# Aggiorna snapshot (quando cambiamento è intenzionale)
./build/tests/snapshot_tests --update

# Genera output per ispezione manuale
lexc examples/imperium_minimal.lex -t lua,json -o tests/output/

# Valida JSON contro schema
python tests/validate_json_schema.py tests/output/imperium_minimal.json
```

---

## Prossimi Passi

1. **Creare directory structure**
   ```
   tests/
   ├── fixtures/          # File .lex di test
   ├── snapshots/         # Output di riferimento
   ├── schemas/           # JSON schemas
   └── contract/          # Contract tests
   ```

2. **Implementare snapshot tests base**
   - `snapshot_tests.cpp`
   - Snapshot iniziali da `imperium_minimal.lex`

3. **Implementare contract tests JSON**
   - Validazione schema
   - Verifica campi obbligatori

4. **Implementare Lua syntax validator**
   - Parser Lua standalone (o regex-based per semplicità)

5. **Aggiornare CMakeLists.txt**
   - Nuovi eseguibili di test
   - Dipendenze (nlohmann/json se serve)

---

## Dipendenze Necessarie

| Libreria | Uso | Già presente |
|----------|-----|--------------|
| Catch2 | Test framework | ✅ |
| nlohmann/json | JSON parsing per contract tests | ❌ (header-only) |

---

*Documento creato per pianificare testing isolato - 2026-03-02*
