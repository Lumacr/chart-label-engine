# Chart Label Placement Engine

A C++20 library for automatic chart label placement, focused on geometric modeling, collision detection, placement optimization, and SVG visualization.

## Overview

Chart Label Placement Engine automatically positions labels around chart elements while minimizing overlaps, avoiding plotting area boundaries, and preserving visual proximity to their corresponding data points.

The project explores geometric algorithms, optimization techniques, software architecture, automated testing, and performance evaluation in modern C++.

## Features

- **Geometry engine:** validated 2D points and axis-aligned rectangles.
- **Label modeling:** labels, anchors, charts, and placement representations.
- **Candidate generation:** eight deterministic candidate positions around each anchor.
- **Collision detection:** pairwise detection of overlapping label placements.
- **Placement scoring:** lexicographical evaluation based on collisions, boundary violations, and anchor distance.
- **Placement optimization:** Greedy Local Improvement using coordinate descent.
- **SVG rendering:** visualization of chart areas, anchors, label placements, collisions, and boundary violations.
- **Automated testing:** comprehensive unit test suite using GoogleTest.
- **Performance benchmarking:** optional benchmarks using Google Benchmark.

## Architecture

The project follows a lightweight Domain-Driven Design approach and separates responsibilities into three main layers:

- **Domain:** geometric primitives, labels, placement problems, and placement representations.
- **Application:** candidate generation, collision detection, placement scoring, and optimization.
- **Rendering:** SVG visualization of placement results.

The placement optimizer coordinates candidate generation and scoring without duplicating their responsibilities.

The rendering layer reuses application-level collision detection to visualize overlapping placements. This introduces an intentional dependency from Rendering to Application, as documented in the SVG Rendering architectural decision.

See [Domain Model](docs/architecture/domain-model.md) for a more detailed architectural description and [Architecture Decision Records](docs/decisions/) for the main design decisions.

## Technology Stack

- **Language:** C++20
- **Build system:** CMake 3.20+
- **Testing:** GoogleTest 1.17.0
- **Benchmarking:** Google Benchmark 1.9.1 (optional)
- **CI:** GitHub Actions
- **Version control:** Git

## Project Structure

```text
chart-label-engine/
├── .github/
│   └── workflows/
├── include/
│   └── chart_label_engine/
│       ├── application/
│       ├── domain/
│       └── rendering/
├── src/
│   ├── application/
│   ├── domain/
│   └── rendering/
├── tests/
├── benchmarks/
│   ├── placement/
│   └── CMakeLists.txt
├── docs/
│   ├── architecture/
│   ├── decisions/
│   └── performance/
├── CMakeLists.txt
└── README.md
```

## Getting Started

### Requirements

- C++20-compatible compiler
- CMake 3.20 or later
- Git
- Internet connection for downloading GoogleTest during the initial CMake configuration

### Clone the repository

```bash
git clone https://github.com/Lumacr/chart-label-engine.git
cd chart-label-engine
```

### Configure the project

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

### Build

```bash
cmake --build build --config Release --parallel
```

### Run tests

```bash
ctest --test-dir build --output-on-failure
```

## Performance Benchmarks

Performance benchmarks are optional and disabled by default.

Google Benchmark is fetched automatically through CMake FetchContent when benchmarks are enabled.

### Configure and build benchmarks

```bash
cmake -S . -B build-benchmarks \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTING=OFF \
    -DBUILD_BENCHMARKS=ON

cmake --build build-benchmarks --config Release --parallel
```

### Run benchmarks

```bash
./build-benchmarks/benchmarks/chart_label_engine_benchmarks
```

The benchmark suite covers candidate generation, collision detection, placement scoring, and greedy placement optimization.

See [Benchmark Results](docs/performance/benchmark-results.md) for the initial performance baseline, methodology, analysis, and identified optimization opportunities.

## Current Limitations

- The optimizer uses a greedy local improvement strategy and does not guarantee a globally optimal solution.
- Collision detection currently uses pairwise comparisons with O(n²) time complexity.
- Placement scoring recalculates the complete solution score for each candidate evaluated by the optimizer.
- Label dimensions are supplied as input; text measurement and font metrics are outside the library's current scope.
- SVG text rendering uses fixed font sizing and does not calculate actual text dimensions or font metrics.
- SVG rendering provides a visualization of placements rather than a complete chart-rendering system.

## Project Status

The core placement pipeline and SVG rendering functionality are implemented.

The project has completed its initial implementation, performance evaluation, API review, and documentation.

## License

This project is licensed under the MIT License.

See the [LICENSE](LICENSE) file for details.
