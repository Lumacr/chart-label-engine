# Chart Label Placement Engine

A C++20 library for automatic chart label placement.

## Overview

Chart Label Placement Engine is a project focused on automatically positioning labels in charts while minimizing overlaps and maintaining visual proximity to their corresponding data points.

The project explores geometric algorithms, collision detection, optimization techniques, data structures and performance evaluation.

## Project Goals

- Implement geometric primitives and collision detection.
- Develop algorithms for automatic label placement.
- Minimize overlapping labels while preserving readability.
- Explore optimization techniques for improving placement quality.
- Evaluate algorithm performance through benchmarking.

## Technology Stack

- **Language:** C++20
- **Build System:** CMake
- **Testing:** GoogleTest
- **CI:** GitHub Actions

## Project Structure

```text
chart-label-engine/
├── .github/
│   └── workflows/
├── include/
│   └── chart_label_engine/
├── src/
├── examples/
├── tests/
├── benchmarks/
├── docs/
│   └── decisions/
├── CMakeLists.txt
└── README.md
```

## Getting Started

### Requirements

- C++20 compatible compiler
- CMake 3.20 or later
- Git

### Clone the repository

```bash
git clone https://github.com/Lumacr/chart-label-engine.git
cd chart-label-engine
```

### Configure the project

```bash
cmake -S . -B build
```

### Build

```bash
cmake --build build
```

### Run tests

```bash
ctest --test-dir build --output-on-failure
```

## Project Status

The project is currently under development.

The initial project structure, CMake configuration, GoogleTest integration and GitHub Actions CI pipeline are in place.

## Roadmap

- [x] Project foundation and CI setup
- [ ] Geometry engine
- [ ] Basic label placement
- [ ] Optimization algorithms
- [ ] Performance benchmarking and improvements