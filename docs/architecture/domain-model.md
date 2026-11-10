
# Domain Model

## 1. Overview

The Chart Label Placement Engine is designed to automatically position labels in charts while minimizing visual overlap and preserving their relationship with the associated data points.

The domain model separates the concepts of geometric representation, label definition, placement problem definition, and label placement.

The application layer builds on these domain concepts to perform operations such as candidate generation.

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

## 3. Application Layer

The application layer implements operations that use domain objects to perform placement-related tasks.

It coordinates domain concepts without introducing rendering or infrastructure dependencies.

### 3.1 CandidateGenerator

`CandidateGenerator` generates possible positions for a single `LabelTarget`.

It receives a `LabelTarget` and returns a collection of `Placement` objects representing alternative positions for its label.

The generator produces eight candidates around the target's anchor:

1. North
2. North-East
3. East
4. South-East
5. South
6. South-West
7. West
8. North-West

Candidates are generated in a fixed order to ensure deterministic results.

The generator accepts a configurable gap that determines the separation between the anchor and the candidate label positions.

The default gap is `5.0` coordinate units. The value must be finite and non-negative. A zero gap is valid.

For diagonal positions, the gap is applied independently along both axes rather than representing a Euclidean distance.

Candidate generation does not depend on the complete `Chart` object. Each target is processed independently.

The generator does not:

- Filter candidates based on the chart's plotting area.
- Detect collisions between candidate placements.
- Calculate placement scores.
- Select an optimal placement.
- Modify the original label target or its anchor.

These responsibilities belong to subsequent stages of the placement pipeline.

### 3.2 CollisionDetector

`CollisionDetector` identifies geometric conflicts between a collection of label placements.

It receives a collection of `Placement` objects and returns the pairs of placements whose geometric bounds overlap.

Each detected collision is represented by a `CollisionPair`, containing the indices of the two placements in the original collection.

The detector uses `Placement::bounds()` to obtain the geometric bounds and `Rectangle::overlaps()` to determine whether two placements collide.

The initial implementation uses a brute-force pairwise comparison algorithm.

Each pair is evaluated exactly once, with the lower index stored first. Results are returned in deterministic order.

The detector does not:

- Compare a placement with itself.
- Consider edge or corner contact a collision.
- Modify the input placements.
- Filter placements based on the chart's plotting area.
- Calculate placement scores or select optimal positions.

The algorithm has O(n²) time complexity, where n is the number of placements.

This implementation provides a simple baseline for future collision detection optimizations.

### 3.3 PlacementScore

`PlacementScore` represents the result of evaluating a complete placement solution.

It contains three scoring criteria:

- Collision count.
- Boundary penalty.
- Anchor distance.

Lower values represent better results for all criteria.

Scores are compared lexicographically, following this priority:

1. Collision count.
2. Boundary penalty.
3. Anchor distance.

This comparison ensures that reducing collisions always takes priority over improving the remaining criteria.

`PlacementScore` is independent of charts, placements, and optimization algorithms.

### 3.4 PlacementScorer

`PlacementScorer` evaluates a complete placement solution for a chart.

It receives a `Chart` and a collection of `Placement` objects.

The number of placements must match the number of label targets. Each placement must contain the label associated with the target at the same index.

The scorer calculates three metrics:

- **Collision count:** the number of overlapping placement pairs, using `CollisionDetector`.
- **Boundary penalty:** the total distance by which placements extend beyond the chart's plotting area.
- **Anchor distance:** the sum of the Euclidean distances between placement centers and their corresponding target anchors.

The scorer returns a `PlacementScore` containing these metrics.

It does not modify the chart or placements, generate candidates, or select an optimal solution.

The scoring process is independent of optimization algorithms.

## 4. Separation of Responsibilities

The architecture separates the following concerns:

- **Geometry:** represents points and rectangular regions through `Point2D` and `Rectangle`.
- **Label definition:** represents textual content and its dimensions through `Label`.
- **Placement problem definition:** `Chart` groups the plotting area and `LabelTarget` objects, which associate labels with their original anchor positions.
- **Label placement:** associates a label with a geometric position through `Placement`.
- **Candidate generation:** the application layer generates alternative placements through `CandidateGenerator`.
- **Collision detection:** identifies conflicting placements through `CollisionDetector`, returning their indices as `CollisionPair` objects.
- **Placement scoring:** evaluates complete placement solutions using collision count, boundary penalty, and anchor distance.

This separation avoids coupling textual content to geometric positioning or rendering concerns.

For example, a label target can be used as input for generating multiple candidate positions without modifying either the label definition or its original anchor. The resulting placements can then be evaluated independently of the original chart data.

## 5. Design Principles

The architecture follows these principles:

- **Single responsibility:** each type represents one distinct concept or operation.
- **Value semantics:** core domain objects are modeled as values rather than entities with independent identities.
- **Encapsulation:** internal state is protected and exposed through controlled interfaces.
- **Validation at construction:** invalid domain objects are rejected when created.
- **Separation of concerns:** geometry, label definition, placement problem definition, placement, candidate generation, and rendering remain independent.
- **Determinism:** candidate generation produces the same results in the same order for identical inputs and configuration.
- **Layer separation:** application services use domain objects without moving application-specific responsibilities into the domain model.
- **Deterministic collision detection:** collision pairs are evaluated once and returned in a consistent order.
- **Independent scoring:** placement evaluation is separated from candidate generation and optimization.
- **Deterministic comparison:** placement scores use a consistent lexicographical ordering.
- **Non-mutating evaluation:** scoring operations do not modify charts or placements.
- **Solution consistency:** every placement must correspond to the label target at the same index.

These principles provide a foundation for implementing increasingly sophisticated placement algorithms without coupling them to presentation or infrastructure details.
