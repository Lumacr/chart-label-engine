#pragma once

#include "chart_label_engine/domain/chart/chart.hpp"
#include "chart_label_engine/domain/placement/placement.hpp"

#include <string>
#include <vector>

namespace chart_label_engine::rendering::svg {

    class SvgRenderer {
    public:
        [[nodiscard]] std::string render(
            const domain::chart::Chart& chart,
            const std::vector<domain::placement::Placement>& placements
        ) const;
    };

} // namespace chart_label_engine::rendering::svg
