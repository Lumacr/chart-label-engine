#include "chart_label_engine/application/placement/greedy_placement_optimizer.hpp"

#include <cstddef>
#include <utility>
#include <vector>

namespace chart_label_engine::application::placement {

    GreedyPlacementOptimizer::GreedyPlacementOptimizer(double candidate_gap)
        : candidate_generator_(candidate_gap) {}

    std::vector<domain::placement::Placement>
    GreedyPlacementOptimizer::optimize(
        const domain::chart::Chart& chart
    ) const {

        const auto& label_targets = chart.label_targets();

        std::vector<std::vector<domain::placement::Placement>> candidates;
        candidates.reserve(label_targets.size());

        std::vector<domain::placement::Placement> placements;
        placements.reserve(label_targets.size());

        // Generate candidates and build the initial solution.
        for (const auto& target : label_targets) {
            auto target_candidates = candidate_generator_.generate(target);

            placements.push_back(target_candidates.front());
            candidates.push_back(std::move(target_candidates));
        }

        // Evaluate the initial solution.
        auto current_score = placement_scorer_.score(chart, placements);

        bool improved = true;

        // Continue until a complete pass produces no improvements.
        while (improved) {
            improved = false;

            for (std::size_t i = 0; i < placements.size(); ++i) {
                const auto previous_score = current_score;

                auto best_placement = placements[i];
                auto best_score = current_score;

                // Evaluate every candidate for the current label.
                for (const auto& candidate : candidates[i]) {
                    placements[i] = candidate;

                    const auto candidate_score =
                        placement_scorer_.score(chart, placements);

                    if (candidate_score.is_better_than(best_score)) {
                        best_placement = candidate;
                        best_score = candidate_score;
                    }
                }

                // Keep the best candidate found.
                placements[i] = std::move(best_placement);

                if (best_score.is_better_than(previous_score)) {
                    improved = true;
                }

                current_score = best_score;
            }
        }

        return placements;
    }

}
