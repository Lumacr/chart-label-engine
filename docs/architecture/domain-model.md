
# Domain Model

## 1. Overview

The Chart Label Placement Engine is designed to automatically position labels in charts while minimizing visual overlap and preserving their relationship with the associated data points.

The domain model separates the concepts of geometric representation, label definition, placement problem definition, and label placement.

This separation keeps the core domain independent of rendering technologies and placement algorithms.

## 2. Core Domain Concepts

### 2.1 Point2D

`Point2D` represents a point in a two-dimensional Cartesian coordinate system.

It encapsulates two coordinates:

- `x`: horizontal position.
- `y`: vertical position.

The class provides basic geometric functionality, including Euclidean distance calculation and equality comparison.

Coordinates must be finite values. Invalid coordinates are rejected during construction.

### 2.2 Rectangle

`Rectangle` represents an axis-aligned rectangular region in the coordinate system.

It is defined by:

- Its top-left corner.
- Its width.
- Its height.

The coordinate system follows these conventions:

- The X-axis increases from left to right.
- The Y-axis increases from bottom to top.
- The rectangle's height extends downwards from its top-left corner.

This convention allows rectangles to be defined using their top-left corner while maintaining a Cartesian coordinate system.

The class provides geometric operations such as point containment, rectangle intersection, overlap detection, area calculation, and center calculation.

Rectangle dimensions must be finite and strictly positive.

### 2.3 Label

`Label` represents the textual content associated with a chart element, together with its required dimensions.

It encapsulates:

- The label text.
- Its width.
- Its height.

The dimensions represent the space required to display the label. Their calculation is outside the responsibility of the domain model and may be provided by an external text measurement or rendering component.

A label does not contain positional information. Its position is determined separately by the placement domain.

Label text must not be empty, and its dimensions must be finite and strictly positive.

### 2.4 Placement

`Placement` represents a label assigned to a specific position in the coordinate system.

It encapsulates:

- A `Label` containing the textual content and dimensions.
- A `Point2D` representing the label's top-left position.

The geometric bounds are derived from the associated label dimensions and position through the `bounds()` operation.

The rectangle is not stored independently, avoiding duplication of dimensional information.

A placement must always represent a valid geometric region. Invalid rectangle boundaries are rejected during construction.

`Placement` represents an assigned position but does not determine where a label should be placed. Automatic positioning and optimization belong to separate components.

### 2.5 LabelTarget

`LabelTarget` represents a chart element that requires a label to be positioned.

It encapsulates:

- A `Label` containing the textual content and required dimensions.
- A `Point2D` representing the original position of the associated chart element, referred to as its anchor.

The anchor identifies the position that the label should remain visually associated with. It is independent of the label's eventual position.

A `LabelTarget` defines an input to the placement problem rather than an assigned position.

Multiple label targets may share the same anchor position. The anchor is not required to lie within any particular plotting area.

### 2.6 Chart

`Chart` represents the placement problem by grouping the available plotting area and the chart elements that require labels.

It encapsulates:

- A `Rectangle` representing the plotting area (`plot_area`).
- A collection of `LabelTarget` objects representing the elements that require labels.

The plotting area defines the geometric region available for label placement. The collection contains the input data required by subsequent placement operations.

A chart may contain an empty collection of label targets, representing a chart with no labels to position.

The collection preserves the insertion order of its elements. Multiple label targets may share the same anchor position, and their anchors are not required to lie within the plotting area.

`Chart` represents the input to the placement problem. It does not contain placement results or implement positioning algorithms.

## 3. Separation of Responsibilities

The domain model separates four concerns:

- **Geometry:** represents points and rectangular regions.
- **Label definition:** represents textual content and its dimensions.
- **Placement problem definition:** `Chart` groups the plotting area and `LabelTarget` objects, which associate labels with their original anchor positions.
- **Label placement:** associates a label with a geometric position through `Placement`.

This separation avoids coupling textual content to geometric positioning or rendering concerns.

For example, a label target can be used as input for generating multiple candidate positions without modifying either the label definition or its original anchor. The resulting placements can then be evaluated independently of the original chart data.

## 4. Design Principles

The domain model follows these principles:

- **Single responsibility:** each type represents one distinct domain concept.
- **Value semantics:** core domain objects are modeled as values rather than entities with independent identities.
- **Encapsulation:** internal state is protected and exposed through controlled interfaces.
- **Validation at construction:** invalid domain objects are rejected when created.
- **Separation of concerns:** geometry, label definition, placement problem definition, placement, and rendering remain independent.

These principles provide a foundation for implementing placement algorithms without coupling them to presentation or infrastructure details.
