# My-Lang

The objectively perfect (not object-oriented) programming language.

### Building

Initialise cmake with:

```sh
cmake -B build
```

Build with:

```sh
cmake --build build -j 12
```

Run tests with:

```sh
GTEST_COLOR=1 ctest --test-dir build --output-on-failure -j12
```

### Dependencies

[googletest v1.15.0](https://github.com/google/googletest/releases/tag/v1.15.0)

### Todo

- [x] Build + test lexer
- [ ] Build + test parser
- [ ] Build + test abstract syntax tree
