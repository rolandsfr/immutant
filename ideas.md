## Reactiveness [DUMP]

```
immutant var // pass by value
mutant var // pass by ref
immutant k = var; // pass by value, is immutable
// reactive var // pass by value, is mutable: MAYBE???

when (mutantVar change) {

}

when (mutantVar init) {

}
```

## Immutability with restriction / partial immutability

### Range restriction

```
imt age < 100 = 25  # automatically enforce a <= 100
age = 200                # runtime error
```

### const restriction (same as full const immutability)

```
imt age := 100;
```

### nonnullable restriction

```
imt criticalValue !?=  ...;
```

### Custom restriction

```
imt age restrict(x -> x >= 0 && x <= 120);
age = -5;  # runtime error
```

### no restriction

```
mt mutableValue = ...;
```
