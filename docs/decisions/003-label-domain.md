# Label Domain

## Status

Accepted

## Context

The Chart Label Placement Engine needs to represent textual labels associated with chart elements.

A label must contain enough information for future placement algorithms to determine the space it occupies. However, its textual content and dimensions should remain independent of its actual position.

The domain model must also remain independent of text rendering technologies and measurement implementations.

## Decisions

### 1. Label as a Value Object

`Label` is modeled as a value object representing a label's content and required dimensions.

It encapsulates:

- Text content.
- Width.
- Height.

The class does not have an independent identity. Two labels with identical text and dimensions are considered equal.

This makes `Label` suitable for value-based operations without requiring entity identifiers or external identity management.

### 2. Separation Between Definition and Position

A `Label` does not store positional information.

Its responsibility is limited to describing what must be displayed and how much space it requires.

The position will be managed separately by the placement domain.

This separation allows the same label definition to be evaluated at multiple candidate positions without modifying the label itself.

It also prevents label content from becoming coupled to geometric positioning or placement algorithms.

### 3. Dimensions Provided Externally

The label dimensions represent the space required to display its text.

Their calculation is outside the responsibility of the `Label` class.

Text measurement may depend on external factors such as:

- Font family.
- Font size.
- Font weight.
- Rendering technology.
- Text measurement implementation.

The domain model therefore receives the dimensions as constructor arguments instead of depending on a particular text rendering or measurement library.

This keeps the domain independent of infrastructure and presentation concerns.

### 4. Constructor Validation

Invalid labels are rejected during construction.

The following invariants are enforced:

- Text must not be empty.
- Width must be finite.
- Height must be finite.
- Width must be strictly positive.
- Height must be strictly positive.

Whitespace-only text is accepted.

The distinction is intentional: the domain rejects empty content but does not impose additional formatting or text normalization rules.

This keeps validation focused on the structural requirements of a label.

### 5. Ownership and Value Semantics

`Label` owns its text through a `std::string` member.

The constructor accepts the text by value and moves it into the internal member using `std::move`.

This approach supports both copying and moving:

- Passing an lvalue allows the constructor to create its own copy.
- Passing an rvalue allows the string to be moved into the object.

The class exposes its text through a constant reference and provides read-only access to its dimensions.

No public setters are provided, so the label's state cannot be modified through its public interface after construction.

### 6. Equality

Equality is defined by comparing all stored attributes:

- Text.
- Width.
- Height.

The defaulted equality operator allows the compiler to generate this comparison based on the class members.

Exact floating-point equality is used, consistently with the `Point2D` value semantics.

No tolerance-based comparison is introduced because the dimensions are part of the label's defined value.

## Consequences

These decisions establish a clear boundary between label definition and label placement.

The resulting model:

- Keeps label content independent of position.
- Avoids dependencies on rendering and text measurement libraries.
- Rejects structurally invalid labels at construction.
- Supports value semantics through member-based equality.
- Provides a simple foundation for the future `Placement` domain.

Future placement logic can use the label's dimensions to construct geometric bounds without changing the responsibility of the `Label` class.