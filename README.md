# My-Lang

The objectively perfect (not object-oriented) programming language.

### Building

Build with:

```sh
cmake -B build && cmake --build build -j 12
```

Run tests with:

```sh
GTEST_COLOR=1 ctest --test-dir build --output-on-failure -j12
```

### Dependencies

[googletest v1.15.0](https://github.com/google/googletest/releases/tag/v1.15.0) - Should be managed by cmake

[LLVM Core](https://llvm.org/) - Install via your package manager
