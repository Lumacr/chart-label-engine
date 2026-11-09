# Placement Model

## Status

Accepted

## Context

The Chart Label Placement Engine needs to represent a label assigned to a specific position in the coordinate system.

The existing domain model already provides:

- `Label`, representing textual content and required dimensions.
- `Point2D`, representing a two-dimensional position.
- `Rectangle`, representing an axis-aligned geometric region.

A placement must connect these concepts without duplicating information or introducing dependencies on rendering technologies or placement algorithms.

## Decisions

### 1. Placement as a Value Object

`Placement` is modeled as a value object representing a label assigned to a specific position.

It encapsulates:

- A `Label`.
- A `Point2D` representing the label's top-left corner.

Two placements are considered equal when their labels and top-left positions are equal.

### 2. Store Position Instead of Rectangle

`Placement` stores the label and its top-left position rather than storing a `Rectangle`.

The rectangle's dimensions are already defined by the associated label.

Storing both the label and its rectangle would duplicate width and height information, introducing an unnecessary consistency requirement.

Instead, the geometric bounds are derived when needed.

### 3. Derived Geometric Bounds

The `bounds()` operation returns a `Rectangle` constructed from:

- The placement's top-left position.
- The associated label's width.
- The associated label's height.

This ensures that the geometric bounds always reflect the dimensions defined by the label.

The rectangle is not stored as an independent member.

### 4. Constructor Validation

A placement must always represent a valid geometric region.

Although `Label` and `Point2D` already enforce their own invariants, their combination may still produce invalid rectangle boundaries due to floating-point overflow.

The constructor therefore validates that a `Rectangle` can be constructed using the supplied label dimensions and position.

Invalid geometric bounds are rejected during construction.

This ensures that every successfully constructed placement can provide valid geometric bounds.

### 5. Ownership and Value Semantics

`Placement` owns its `Label` and stores its position by value.

This avoids external lifetime dependencies and ensures that a placement remains valid independently of the objects originally supplied to its constructor.

The public interface provides read-only access to its state.

No public setters are provided.

### 6. Separation from Placement Algorithms

`Placement` represents an already assigned position.

It does not determine where a label should be placed.

Candidate generation, collision detection between placements, optimization, and label positioning strategies belong to separate application or domain components.

This keeps the value object focused on representing a valid placement rather than implementing placement behavior.

## Consequences

These decisions establish a simple relationship between labels and geometry.

The resulting model:

- Avoids duplicating label dimensions.
- Keeps label definition independent of position.
- Provides geometric bounds on demand.
- Rejects invalid placements during construction.
- Avoids external ownership and lifetime dependencies.
- Provides a foundation for future placement algorithms without coupling them to the value object.