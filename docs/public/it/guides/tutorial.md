# Tutorial: Iniziare con Lex

## Installazione

### Da Sorgente

```bash
git clone https://github.com/yourorg/lex.git
cd lex
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

Il binario del compilatore sarà in `build/Release/lexc.exe`.

## Il Tuo Primo Programma

Crea `ciao.lex`:

```lex
fn main() {
    let messaggio: string = "Ciao, Lex!";
    print(messaggio);
}
```

Compilalo:

```bash
lexc ciao.lex -o ciao.lua
```

Output (`ciao.lua`):

```lua
function main()
    local messaggio = "Ciao, Lex!"
    print(messaggio)
end
```

## Variabili e Tipi

```lex
fn esempio() {
    // Numeri
    let eta: int = 25;
    let prezzo: float = 19.99;
    
    // Booleano
    let attivo: bool = true;
    
    // Stringa
    let nome: string = "Giocatore1";
}
```

## Funzioni

```lex
fn saluta(nome: string): string {
    return "Ciao, " + nome;
}

fn somma(a: int, b: int): int {
    return a + b;
}

fn main() {
    let saluto: string = saluta("Mondo");
    let totale: int = somma(10, 20);
}
```

## Controllo del Flusso

```lex
fn controllaEta(eta: int) {
    if eta >= 18 {
        print("Adulto");
    } else {
        print("Minorenne");
    }
}

fn contoAllaRovescia() {
    let i: int = 10;
    while i > 0 {
        print(i);
        i = i - 1;
    }
}
```

## Integrazione con il Gioco

```lex
// Spawn unità
fn spawnLavoratori() {
    spawn("worker", 0, 0, 0);
    spawn("worker", 10, 0, 0);
}

// Gestione eventi
on gioco_avviato {
    spawnLavoratori();
}

on unita_morta {
    emit("controlla_vittoria");
}
```

## Prossimi Passi

- Leggi il [Riferimento Linguaggio](../spec/language.md)
- Scopri la [Validazione](../spec/validation.md)
- Esplora l'[Integrazione Imperium](../integrations/imperium-engine.md)
