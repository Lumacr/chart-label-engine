# Decision 005: Placement Problem Model

## Status

Accepted

## Context

The existing domain model provides the foundational concepts required to represent geometry, labels, and assigned label positions.

However, the engine still needs a representation of the problem it must solve: a collection of labels associated with positions in a chart, constrained by an available plotting area.

The model must provide enough information for placement algorithms without introducing dependencies on chart rendering, data visualization, or presentation concerns.

## Decisions

### 1. Introduce LabelTarget

`LabelTarget` represents a label associated with a reference position in the chart.

It contains:

- A `Label` representing the textual content and required dimensions.
- A `Point2D` representing the anchor position.

The anchor identifies the original chart element associated with the label. It is independent of the label's eventual position.

`LabelTarget` is modeled as a Value Object.

### 2. Reuse Point2D for Anchor Positions

The existing `Point2D` type is used to represent anchor positions.

A separate `AnchorPoint` type is not introduced because it would duplicate the existing geometric representation without providing additional behavior or invariants.

The semantic meaning of the position is expressed through the `anchor` member of `LabelTarget`.

### 3. Introduce Chart

`Chart` represents the input required by the placement engine.

It contains:

- A `Rectangle` representing the plotting area.
- A collection of `LabelTarget` objects representing the labels to be positioned.

The chart does not model complete visualization concepts such as axes, series, colors, or numerical datasets.

Its responsibility is limited to providing the geometric context and label targets required by the placement problem.

### 4. Allow Empty Charts

A chart may contain zero or more label targets.

An empty collection is valid and represents a chart with no labels to position.

### 5. Allow Shared Anchor Positions

Multiple label targets may reference the same anchor position.

This supports scenarios where different chart elements share the same coordinates.

No uniqueness constraint is imposed on anchor positions.

### 6. Do Not Require Anchors to Be Inside the Plotting Area

An anchor position is not required to be contained within the chart's plotting area.

This avoids imposing unnecessary restrictions on the input model and allows future algorithms to handle points near or beyond plotting boundaries.

### 7. Keep Placement Algorithms Separate

`Chart` and `LabelTarget` represent the input problem, not its solution.

Candidate generation, collision detection, scoring, and optimization belong to separate components.

The output of a placement algorithm will consist of `Placement` objects.

## Consequences

- The placement problem has an explicit and minimal input model.
- Existing geometry and label types are reused.
- The domain remains independent of rendering and visualization technologies.
- Placement algorithms can operate on a well-defined input without modifying the chart or label definitions.
- Future extensions can introduce additional constraints without requiring the chart model to represent an entire visualization system.