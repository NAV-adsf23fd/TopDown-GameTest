Compile with normal debug
```
make
```

Compile without debug (release)
```
make LOG=off
```

Build and Run with normal debug
```
make run
```

Build and Run without debug (release)
```
make run LOG=off
```

Build and Run and catch nullptr/memory errors
```
make asan
```

Build and Run and catch Data Race errors
```
make tsan
```

Clean build
```
make clean
```
