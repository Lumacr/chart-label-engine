
# Collision Detection

## Status

Accepted

## Date

2026-11-09

## Context

The Chart Label Placement Engine generates multiple candidate placements for each label.

Before evaluating or optimizing these placements, the system needs to identify geometric conflicts between labels.

Two placements are considered to be in collision when their associated rectangular bounds overlap with a positive intersection area.

The existing domain model already provides the necessary geometric functionality through `Placement::bounds()` and `Rectangle::overlaps()`.

Collision detection must reuse these operations rather than introducing duplicate geometric logic.

## Decisions

### 1. Application Layer Responsibility

Collision detection is implemented in the application layer.

The `CollisionDetector` component receives a collection of `Placement` objects and identifies all pairs whose geometric bounds overlap.

Collision detection is an operation performed over domain objects rather than an intrinsic responsibility of an individual domain object.

### 2. Collision Representation

Each detected collision is represented by a `CollisionPair`.

A collision pair contains the indices of the two placements involved in the collision.

Indices refer to the original input collection.

This representation avoids copying placement objects and allows subsequent application operations to identify the conflicting placements.

### 3. Collision Detection Algorithm

The initial implementation uses a brute-force pairwise comparison algorithm.

For each placement, the detector compares its geometric bounds with the bounds of every subsequent placement.

Two placements are considered to be in collision when their rectangles overlap according to `Rectangle::overlaps()`.

The detector does not compare a placement with itself.

Each pair is evaluated only once.

### 4. Collision Semantics

A collision occurs only when two rectangular bounds share a positive intersection area.

Placements whose bounds only touch along an edge or at a corner are not considered to be in collision.

This behavior follows the existing semantics of `Rectangle::overlaps()`.

### 5. Deterministic Results

Collision pairs are returned in deterministic order.

The detector processes placement indices in ascending order and records pairs using the lower index first.

This ensures that identical input collections produce identical collision results.

### 6. Input Independence

Collision detection operates on a collection of placements independently of the complete `Chart` object.

The detector does not filter placements based on the chart's plotting area.

It does not modify the input collection or the individual placements.

### 7. Algorithmic Complexity

The brute-force algorithm has quadratic time complexity, O(n²), where n is the number of placements.

This approach provides a simple and reliable baseline for collision detection.

More efficient spatial data structures may be considered in future iterations if performance measurements justify their introduction.

## Consequences

- Collision detection reuses the existing geometry and placement domain models.
- The application layer remains responsible for coordinating placement-related operations.
- Every detected collision identifies the two conflicting placements.
- Edge and corner contact do not count as collisions.
- Results are deterministic and independent of rendering concerns.
- The initial implementation prioritizes simplicity and correctness over large-scale optimization.
- The brute-force implementation provides a baseline for future performance comparisons.
