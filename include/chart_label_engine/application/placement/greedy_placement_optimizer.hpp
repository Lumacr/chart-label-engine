#pragma once

#include "chart_label_engine/application/placement/candidate_generator.hpp"
#include "chart_label_engine/application/placement/placement_scorer.hpp"
#include "chart_label_engine/domain/chart/chart.hpp"
#include "chart_label_engine/domain/placement/placement.hpp"

#include <vector>

namespace chart_label_engine::application::placement {

    class GreedyPlacementOptimizer {
    public:
        explicit GreedyPlacementOptimizer(double candidate_gap = 5.0);

        [[nodiscard]] std::vector<domain::placement::Placement> optimize(
            const domain::chart::Chart& chart
        ) const;

    private:
        CandidateGenerator candidate_generator_;
        PlacementScorer placement_scorer_;
    };

}
