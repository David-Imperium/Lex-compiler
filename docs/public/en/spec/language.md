# Language Specification

## Basic Types

| Type | Description | Example |
|------|-------------|---------|
| `int` | Integer numbers | `42`, `-10` |
| `float` | Floating point | `3.14`, `-0.5` |
| `bool` | Boolean | `true`, `false` |
| `string` | Text string | `"Hello"` |

## Variable Declaration

```lex
let name: type = value;
```

Examples:
```lex
let age: int = 25;
let pi: float = 3.14159;
let active: bool = true;
let message: string = "Welcome";
```

## Functions

### Declaration
```lex
fn functionName(param1: type1, param2: type2): returnType {
    // body
}
```

### Example
```lex
fn add(a: int, b: int): int {
    return a + b;
}

fn greet(name: string): string {
    return "Hello, " + name;
}
```

## Control Flow

### If/Else
```lex
if condition {
    // ...
} else if otherCondition {
    // ...
} else {
    // ...
}
```

### While Loop
```lex
while condition {
    // ...
}
```

### For Loop
```lex
for let i: int = 0; i < 10; i = i + 1 {
    // ...
}
```

## Game-Specific Keywords

| Keyword | Description |
|---------|-------------|
| `spawn` | Create a unit |
| `destroy` | Remove entity |
| `move` | Move entity |
| `wait` | Pause execution |
| `emit` | Send event |
| `on` | Event handler |

### Example
```lex
fn spawnWorker(x: int, y: int) {
    spawn("worker_unit", x, y, 0);
}

on unit_died {
    emit("respawn_request");
}
```

## Comments

```lex
// Single line comment

/*
 Multi-line
 comment
*/
```

## Operators

| Type | Operators |
|------|-----------|
| Arithmetic | `+`, `-`, `*`, `/`, `%` |
| Comparison | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Logical | `and`, `or`, `not` |
| Assignment | `=`, `+=`, `-=`, `*=`, `/=` |
