
# Greedy Placement Optimization

## 1. Context

The placement engine needs an algorithm capable of selecting one candidate placement for each label while minimizing visual conflicts.

The previous milestones provide the necessary building blocks:

- Candidate generation produces eight deterministic positions for each label.
- Collision detection identifies overlapping placements.
- Placement scoring evaluates complete placement solutions using collision count, boundary penalty, and anchor distance.

A placement optimization strategy is therefore required to combine these components and produce a complete solution.

## 2. Decision

The first placement optimization strategy is a Greedy Local Improvement algorithm based on coordinate descent.

The algorithm operates as follows:

1. Generate and store the eight candidate placements for every label target.
2. Initialize the solution using the first candidate of each target.
3. Evaluate the initial solution using `PlacementScorer`.
4. Iterate over each label target individually.
5. Evaluate every candidate for the current target while keeping all other placements unchanged.
6. Select the candidate with the best placement score.
7. Accept the new placement only when it strictly improves the current solution.
8. Repeat complete passes until no placement can be improved.

The placement score is evaluated lexicographically using the following criteria:

1. Minimize collision count.
2. Minimize boundary penalty.
3. Minimize anchor distance.

Candidate generation order is deterministic. When multiple candidates produce the same score, the algorithm retains the existing placement.

The candidate gap is configurable and defaults to `5.0`.

## 3. Rationale

Greedy Local Improvement was selected as the initial optimization strategy because it:

- Reuses the existing candidate generation and scoring components.
- Is deterministic and straightforward to test.
- Provides a simple baseline for evaluating future optimization strategies.
- Accepts only strictly improving changes, ensuring that the solution score never worsens.
- Avoids introducing additional algorithmic complexity at this stage.

The strategy also provides a foundation for comparing more advanced optimization approaches in future milestones.

## 4. Consequences

### Positive

- Produces a valid placement for every label target.
- Preserves the original target order.
- Eliminates avoidable collisions when improvements are possible through individual placement changes.
- Does not modify the input chart.
- Produces deterministic results for the same input.
- Terminates when a complete pass produces no improvements.

### Limitations

- The algorithm only evaluates changes to one label at a time.
- It can converge to a local optimum rather than a global optimum.
- The result may depend on the order in which label targets are processed.
- Repeated evaluation of the complete placement solution can become computationally expensive as the number of labels increases.

The current implementation prioritizes correctness and simplicity over optimization performance.

## 5. Alternatives Considered

### Simulated Annealing

Simulated Annealing can explore solutions that temporarily worsen the score, potentially escaping local optima.

It was not selected for the initial implementation because it introduces additional parameters, stochastic behavior, and more complex testing requirements.

It may be considered in a future milestone for comparison with the greedy baseline.

### Global Exhaustive Search

Evaluating every possible combination of candidate placements would guarantee finding the best solution within the generated candidate space.

However, the search space grows exponentially with the number of labels, making this approach impractical for larger charts.

## 6. Status

Accepted.
