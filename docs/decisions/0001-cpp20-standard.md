# ADR 0001: Adopt C++20 as the Project Standard

- **Status:** Accepted
- **Date:** 2026-11-03

## Context

The Chart Label Placement Engine requires a programming language and standard that support efficient geometric computations, algorithm development and performance-oriented software engineering.

The project also aims to demonstrate modern C++ development practices, maintainability and compatibility with commonly available development toolchains.

## Decision

The project will use **C++20** as its required language standard.

The CMake configuration enforces this standard through:

- `CMAKE_CXX_STANDARD 20`
- `CMAKE_CXX_STANDARD_REQUIRED ON`
- `CMAKE_CXX_EXTENSIONS OFF`

Compiler-specific language extensions are disabled to encourage standard-compliant C++ code.

## Alternatives Considered

### C++17

C++17 offers broad compiler support and includes several important modern C++ features.

However, it does not provide all the language and standard library improvements available in C++20.

### C++23

C++23 introduces additional language and library improvements.

However, the project does not currently require its newer features, and C++20 provides a more conservative compatibility target.

## Rationale

C++20 provides a suitable balance between modern language capabilities and compiler support.

It offers useful features for developing maintainable and efficient software, while allowing the project to remain compatible with established C++ toolchains.

It also provides room to explore modern C++ practices without requiring the adoption of the newest available standard.

## Consequences

### Positive

- Access to modern C++20 language and library features.
- A consistent language standard across development and CI environments.
- Compiler extensions are disabled, encouraging portability.
- A stable foundation for future geometric and optimization components.

### Trade-offs

- Development environments must provide a C++20-compatible compiler.
- Some C++23 features will not be available unless the project changes its language standard in the future.