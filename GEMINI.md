# nstdlib

`nstdlib` is a project aimed at implementing the C standard library from scratch as a study exercise. The goal is to deeply understand the inner workings of memory management, string manipulation, and other core systems.

## Key Principles

- **POSIX Compliance**: Tests should be written by cross-referencing POSIX requirements of the corresponding C standard library functions.
- **Modularity**: The library is divided into logical modules like `jmm` (memory management) and `jstring` (string operations).

## Modules

- **jmm**: Custom memory allocator and related operations.
- **jstring**: String and memory manipulation functions.

## Testing

The project uses a custom test dispatcher that allows for targeted testing of specific functions or entire modules.
- **IF YOU'RE AN AI, READ HERE.**
    - Your main mission is to write the test cases. Do not touch the implementation files.
    - Make sure every test for every function challenges every possible edge-cases defined by their POSIX counterpart.
