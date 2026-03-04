# Specifica del Linguaggio

## Tipi Base

| Tipo | Descrizione | Esempio |
|------|-------------|---------|
| `int` | Numeri interi | `42`, `-10` |
| `float` | Virgola mobile | `3.14`, `-0.5` |
| `bool` | Booleano | `true`, `false` |
| `string` | Stringa di testo | `"Ciao"` |

## Dichiarazione Variabili

```lex
let nome: tipo = valore;
```

Esempi:
```lex
let eta: int = 25;
let pi: float = 3.14159;
let attivo: bool = true;
let messaggio: string = "Benvenuto";
```

## Funzioni

### Dichiarazione
```lex
fn nomeFunzione(param1: tipo1, param2: tipo2): tipoRitorno {
    // corpo
}
```

### Esempio
```lex
fn somma(a: int, b: int): int {
    return a + b;
}

fn saluta(nome: string): string {
    return "Ciao, " + nome;
}
```

## Controllo del Flusso

### If/Else
```lex
if condizione {
    // ...
} else if altraCondizione {
    // ...
} else {
    // ...
}
```

### Ciclo While
```lex
while condizione {
    // ...
}
```

### Ciclo For
```lex
for let i: int = 0; i < 10; i = i + 1 {
    // ...
}
```

## Keyword Specifiche per Giochi

| Keyword | Descrizione |
|---------|-------------|
| `spawn` | Crea un'unità |
| `destroy` | Rimuovi entità |
| `move` | Muovi entità |
| `wait` | Metti in pausa |
| `emit` | Invia evento |
| `on` | Gestore eventi |

### Esempio
```lex
fn spawnLavoratore(x: int, y: int) {
    spawn("worker_unit", x, y, 0);
}

on unit_morta {
    emit("richiesta_respawn");
}
```

## Commenti

```lex
// Commento su una riga

/*
 Commento
 su più righe
*/
```

## Operatori

| Tipo | Operatori |
|------|-----------|
| Aritmetici | `+`, `-`, `*`, `/`, `%` |
| Confronto | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Logici | `and`, `or`, `not` |
| Assegnamento | `=`, `+=`, `-=`, `*=`, `/=` |
