
# 006 — Candidate Generation

- **Status:** Accepted
- **Date:** 2026-11-09

## Context

The chart placement problem requires generating possible positions for each label before evaluating collisions, calculating placement scores, or selecting a final arrangement.

The existing domain model provides:

- `LabelTarget`, representing a label associated with an anchor point.
- `Placement`, representing a label assigned to a top-left position.
- `Point2D` and `Rectangle`, providing the geometric operations required to calculate candidate positions.

Candidate generation must remain independent of rendering, collision detection, and optimization.

## Decisions

### 1. Candidate generator responsibility

Introduce a `CandidateGenerator` responsible for generating possible placements for a single `LabelTarget`.

The generator receives one `LabelTarget` and returns a `std::vector<Placement>`.

It does not process the entire `Chart`, detect collisions, calculate scores, or select a final placement.

### 2. Candidate positions

Generate eight candidates around the anchor point:

1. North
2. North-East
3. East
4. South-East
5. South
6. South-West
7. West
8. North-West

Candidates are generated in this fixed order to ensure deterministic results.

### 3. Coordinate conventions

The existing Cartesian coordinate convention applies:

- X increases to the right.
- Y increases upwards.
- A rectangle is represented by its top-left position, width, and height.
- Rectangle height extends downwards from its top edge.

Candidate positions are calculated as follows, where:

- `x` and `y` represent the anchor coordinates.
- `w` and `h` represent the label dimensions.
- `g` represents the configured gap.

| Position | Candidate top-left |
|---|---|
| North | `(x - w/2, y + g + h)` |
| North-East | `(x + g, y + g + h)` |
| East | `(x + g, y + h/2)` |
| South-East | `(x + g, y - g)` |
| South | `(x - w/2, y - g)` |
| South-West | `(x - g - w, y - g)` |
| West | `(x - g - w, y + h/2)` |
| North-West | `(x - g - w, y + g + h)` |

For diagonal positions, the gap is applied independently along both axes. It therefore represents an axis-aligned separation rather than a Euclidean distance.

### 4. Configurable gap

The generator accepts a configurable gap value.

- Default gap: `5.0`.
- Gap must be finite.
- Gap must be non-negative.
- A zero gap is valid and allows a candidate to touch the anchor.

The gap uses the same coordinate units as the geometric domain.

### 5. Candidate representation

Candidates are represented using the existing `Placement` value object.

A separate candidate-specific domain model is unnecessary at this stage because candidate placements do not yet contain additional information such as scores or rankings.

### 6. Plot area constraints

Candidate generation does not filter placements based on the chart's plot area.

Candidates may extend beyond the plot area. Boundary validation and candidate filtering belong to later stages of the placement pipeline.

### 7. Determinism

Given the same `LabelTarget` and gap configuration, the generator must always produce the same eight placements in the same order.

This makes the generator predictable and its behaviour straightforward to test.

## Consequences

- Candidate generation is independent of the number of labels in a chart.
- The generator can be tested without constructing a complete `Chart`.
- The fixed candidate order provides a predictable input for future scoring and optimization algorithms.
- The gap can be adjusted without changing the generation strategy.
- Candidate generation remains separate from collision detection, scoring, and optimization.
- Invalid geometric results, including coordinate overflow, are rejected by the existing geometry and placement models.
