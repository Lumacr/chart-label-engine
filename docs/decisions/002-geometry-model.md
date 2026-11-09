# Geometry Model

## Status

Accepted

## Context

The Chart Label Placement Engine requires a geometric foundation for representing chart elements, calculating distances, and detecting spatial relationships between labels and other chart objects.

The geometry model must provide predictable behavior, reject invalid geometric values, and remain independent of rendering technologies.

Two fundamental value objects were introduced:

- `Point2D`: represents positions in a two-dimensional coordinate system.
- `Rectangle`: represents axis-aligned rectangular regions.

## Decisions

### 1. Cartesian Coordinate System

The geometry model uses a two-dimensional Cartesian coordinate system.

The coordinate conventions are:

- The X-axis increases from left to right.
- The Y-axis increases from bottom to top.
- A rectangle is defined by its top-left corner.
- Rectangle width extends towards increasing X coordinates.
- Rectangle height extends towards decreasing Y coordinates.

This approach provides a consistent geometric model while allowing rectangles to use a top-left reference point.

Rendering-specific coordinate transformations, such as adapting to SVG's downward-increasing Y-axis, are handled outside the geometry domain.

### 2. Point2D as a Value Object

`Point2D` encapsulates two coordinates and provides basic geometric operations.

Its implementation includes:

- Coordinate accessors.
- Euclidean distance calculation.
- Exact equality comparison.

Exact equality was chosen because `Point2D` represents a mathematical value. Approximate equality would introduce a tolerance-dependent interpretation of equality.

If approximate comparisons become necessary, they should be implemented as explicit operations with a defined tolerance rather than changing the equality operator.

### 3. Rectangle Representation

`Rectangle` is represented by:

- A top-left `Point2D`.
- A width.
- A height.

The remaining boundaries are derived from these values:

- `left()` returns the X coordinate of the top-left corner.
- `top()` returns the Y coordinate of the top-left corner.
- `right()` returns the left boundary plus the width.
- `bottom()` returns the top boundary minus the height.

This representation avoids storing redundant boundary coordinates.

### 4. Boundary Contact and Overlap Semantics

Two separate operations are provided to distinguish geometric intersection from positive-area overlap.

#### intersects()

`intersects()` returns `true` when two rectangles share any point.

This includes:

- Partial or complete overlap.
- Edge contact.
- Corner contact.

This behavior is useful when detecting whether two rectangular regions have any geometric contact.

#### overlaps()

`overlaps()` returns `true` only when two rectangles share a region with positive area.

Consequently:

- Partial overlap returns `true`.
- Complete containment returns `true`.
- Edge contact returns `false`.
- Corner contact returns `false`.
- Separated rectangles return `false`.

This distinction is important for label placement, where touching boundaries may be acceptable even when geometric intersection is detected.

### 5. Point Containment

`contains()` uses inclusive boundaries.

A point located exactly on any rectangle edge or corner is considered contained within the rectangle.

This behavior follows the geometric interpretation of a rectangle as a closed region.

### 6. Numeric Validation

Invalid geometric values are rejected during construction.

For `Point2D`:

- Both coordinates must be finite.

For `Rectangle`:

- Width and height must be finite.
- Width and height must be strictly positive.
- The calculated right and bottom boundaries must remain finite.

This prevents invalid dimensions and non-finite boundaries from entering the geometry model.

### 7. Floating-Point Calculations

The implementation uses `std::hypot()` for Euclidean distance calculation.

This function is preferred over manually calculating the square root of the sum of squared coordinate differences because it provides better protection against intermediate overflow and underflow.

Rectangle area calculation checks whether the resulting value is finite.

An `std::overflow_error` is thrown when the calculated area is not finite. Underflow to zero is not treated as overflow.

The center coordinates are calculated using half the width and height added to or subtracted from the corresponding top-left coordinates.

## Consequences

The geometry model provides a consistent foundation for future placement algorithms.

The main consequences of these decisions are:

- Geometric objects reject invalid values at construction.
- Rectangle boundaries are derived rather than stored independently.
- Boundary contact and positive-area overlap have distinct semantics.
- Geometry remains independent of rendering and presentation concerns.
- Floating-point limitations are explicitly considered in geometric calculations.

These decisions establish the behavior expected from the geometry layer and provide a reference for future extensions.