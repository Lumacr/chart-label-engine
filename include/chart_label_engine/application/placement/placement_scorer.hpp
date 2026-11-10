#pragma once

#include "chart_label_engine/application/placement/placement_score.hpp"
#include "chart_label_engine/domain/chart/chart.hpp"
#include "chart_label_engine/domain/placement/placement.hpp"

#include <vector>

namespace chart_label_engine::application::placement {

    class PlacementScorer {
    public:
        [[nodiscard]] PlacementScore score(
            const domain::chart::Chart& chart,
            const std::vector<domain::placement::Placement>& placements
        ) const;
    };

}
