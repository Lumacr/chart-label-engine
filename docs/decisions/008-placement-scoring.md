# 008 - Placement Scoring

- Status: Accepted
- Date: 2026-11-09

## Context

The chart label placement engine currently generates candidate placements and detects geometric collisions.

The next step is to evaluate the quality of a complete placement solution before introducing an optimization algorithm.

The scoring mechanism must evaluate solutions consistently while remaining independent of candidate generation and optimization strategies.

## Decisions

### 1. Separate scoring from optimization

A dedicated `PlacementScorer` will evaluate complete placement solutions.

It will not generate candidates, modify placements, or select an optimal solution.

Optimization algorithms will consume placement scores without being coupled to the scoring implementation.

### 2. Evaluate complete placement solutions

A placement solution consists of one `Placement` for each `LabelTarget` in a `Chart`.

The scorer requires the number of placements to match the number of label targets.

Each placement is associated with the target at the same index in the chart's label target collection.

Each placement must contain the same label as its corresponding target.
Solutions containing mismatched labels are rejected.

### 3. Use three scoring criteria

The initial scoring model considers:

- **Collision count:** the number of overlapping placement pairs.
- **Boundary penalty:** the total distance by which placements extend beyond the chart's plotting area.
- **Anchor distance:** the sum of the Euclidean distances between each placement's center and its corresponding target anchor.

Lower values are better for all three criteria.

### 4. Reuse existing collision detection

The scorer will use `CollisionDetector` to calculate the number of collisions.

This avoids duplicating collision detection logic and ensures consistent geometric behaviour.

### 5. Use lexicographical score comparison

Scores are compared according to the following priority:

1. Collision count.
2. Boundary penalty.
3. Anchor distance.

A solution with fewer collisions is always preferred, regardless of its other penalties.

When collision counts are equal, the boundary penalty is compared.

When both previous criteria are equal, anchor distance determines the preference.

No configurable weights are introduced at this stage.

### 6. Represent scores independently

`PlacementScore` represents the numerical result of evaluating a placement solution.

It contains the three scoring criteria and provides a comparison operation.

The score does not retain references to the chart or placements used during evaluation.

### 7. Keep scoring deterministic

The same chart and placement solution must always produce the same score.

The scoring process must not modify its inputs.

## Consequences

- Scoring is independent of optimization algorithms.
- Existing collision detection is reused.
- The scoring criteria can be tested independently.
- Lexicographical comparison avoids arbitrary weighting decisions.
- Future scoring criteria can be introduced through explicit architectural decisions.
- The initial model prioritizes collision avoidance over boundary compliance and anchor proximity.