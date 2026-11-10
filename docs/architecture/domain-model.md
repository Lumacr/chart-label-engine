
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

### 3.5 GreedyPlacementOptimizer

`GreedyPlacementOptimizer` selects a placement for every label target by applying a Greedy Local Improvement algorithm based on coordinate descent.

It coordinates the existing application components:

- `CandidateGenerator` generates alternative positions for each label target.
- `PlacementScorer` evaluates complete placement solutions.

The optimizer operates as follows:

1. Generate and store the candidate placements for every label target.
2. Initialize the solution using the first candidate of each target.
3. Evaluate the initial solution using `PlacementScorer`.
4. Iterate over each label target individually.
5. Evaluate every candidate for the current target while keeping all other placements unchanged.
6. Select the candidate with the best placement score.
7. Accept a new placement only when it strictly improves the current solution.
8. Repeat complete passes until no placement can be improved.

The optimizer uses the lexicographical ordering defined by `PlacementScore`:

1. Collision count.
2. Boundary penalty.
3. Anchor distance.

When multiple candidates produce the same score, the existing placement is retained. This preserves deterministic behavior.

The candidate gap is configurable and defaults to `5.0`.

The optimizer preserves the original order of label targets and returns one placement for each target. An empty chart produces an empty solution.

The optimizer does not modify the input chart.

The optimization process terminates when a complete pass over all label targets produces no strictly improving placement. Since each target has a finite set of candidates and the optimizer accepts only strictly improving solutions, the algorithm cannot revisit a previous solution.

The algorithm guarantees that accepted changes never worsen the solution score. However, it only evaluates changes to one label at a time and may converge to a local optimum rather than a global optimum.

`GreedyPlacementOptimizer` does not implement candidate generation or scoring itself. It coordinates these independent operations to produce a complete placement solution.

## 4. Rendering Layer

The rendering layer converts placement solutions into visual representations without introducing presentation concerns into the domain model.

### 4.1 SvgRenderer

`SvgRenderer` generates a complete SVG document from a `Chart` and its corresponding collection of `Placement` objects.

It represents:

- The chart plotting area.
- Label anchor points.
- Final label positions.
- Connections between anchors and label placements.
- Visual indicators for colliding placements.
- Visual indicators for placements extending beyond the plotting area.

The renderer converts the domain coordinate system into SVG coordinates, accounting for the difference in vertical axis direction.

It also calculates a viewport that includes the plotting area, placements, and anchors, with an additional visual margin.

Before generating the document, the renderer validates that the number and order of placements correspond to the chart's label targets. It also validates label text against UTF-8 encoding requirements and XML 1.0 character restrictions.

XML special characters are escaped, while valid Unicode and permitted XML whitespace characters are preserved.

The renderer returns the SVG document as a `std::string`. It does not modify its input objects or perform filesystem operations.

Collision visualization reuses the existing `CollisionDetector` from the application layer rather than implementing collision detection independently.

Consequently, the rendering layer has an explicit dependency on the application layer for collision detection. This dependency is intentional and documented in the SVG Rendering architectural decision.

The renderer does not perform placement generation, scoring, or optimization, and it does not measure text dimensions or implement a complete charting solution.

## 5. Separation of Responsibilities

The architecture separates the following concerns:

- **Geometry:** represents points and rectangular regions through `Point2D` and `Rectangle`.
- **Label definition:** represents textual content and its dimensions through `Label`.
- **Placement problem definition:** `Chart` groups the plotting area and `LabelTarget` objects, which associate labels with their original anchor positions.
- **Label placement:** associates a label with a geometric position through `Placement`.
- **Candidate generation:** the application layer generates alternative placements through `CandidateGenerator`.
- **Collision detection:** identifies conflicting placements through `CollisionDetector`, returning their indices as `CollisionPair` objects.
- **Placement scoring:** evaluates complete placement solutions using collision count, boundary penalty, and anchor distance.
- **Placement optimization:** selects a complete placement solution through `GreedyPlacementOptimizer`, coordinating candidate generation and scoring using a Greedy Local Improvement strategy.
- **Rendering:** converts charts and placement solutions into SVG documents through `SvgRenderer`, reusing application-level collision detection.

This separation prevents domain objects from depending on placement algorithms or presentation technologies. Application and rendering responsibilities remain outside the domain model.

For example, a label target can be used as input for generating multiple candidate positions without modifying either the label definition or its original anchor. The resulting placements can then be evaluated independently of the original chart data and subsequently rendered as an SVG document.

## 6. Design Principles

The architecture follows these principles:

- **Single responsibility:** each type represents one distinct concept or operation.
- **Value semantics:** core domain objects are modeled as values rather than entities with independent identities.
- **Encapsulation:** internal state is protected and exposed through controlled interfaces.
- **Validation at construction:** invalid domain objects are rejected when created.
- **Separation of concerns:** geometry, label definition, placement problem definition, placement, application operations, and rendering have distinct responsibilities.
- **Determinism:** candidate generation produces the same results in the same order for identical inputs and configuration.
- **Layer separation:** domain objects remain independent of application and rendering concerns. Application services depend on the domain layer, while the rendering layer reuses application functionality where required, as explicitly documented in the SVG Rendering decision.
- **Deterministic collision detection:** collision pairs are evaluated once and returned in a consistent order.
- **Independent scoring:** placement evaluation is separated from candidate generation and optimization.
- **Deterministic comparison:** placement scores use a consistent lexicographical ordering.
- **Non-mutating evaluation:** scoring operations do not modify charts or placements.
- **Solution consistency:** every placement must correspond to the label target at the same index.
- **Deterministic optimization:** placement optimization produces consistent results for identical inputs and configuration.
- **Monotonic improvement:** the greedy optimization algorithm accepts only strictly improving changes, ensuring that the solution score never worsens.
- **Optimization separation:** placement optimization coordinates candidate generation and scoring without duplicating their responsibilities.

These principles provide a foundation for implementing increasingly sophisticated placement algorithms without coupling them to presentation or infrastructure details.
