#pragma once

#include "chart_label_engine/domain/chart/label_target.hpp"
#include "chart_label_engine/domain/geometry/rectangle.hpp"

#include <vector>

namespace chart_label_engine::domain::chart {

    class Chart {
    public:
        Chart(
            geometry::Rectangle plot_area,
            std::vector<LabelTarget> label_targets
        );

        [[nodiscard]] const geometry::Rectangle& plot_area() const noexcept;
        [[nodiscard]] const std::vector<LabelTarget>& label_targets() const noexcept;

        [[nodiscard]] bool operator==(const Chart& other) const noexcept = default;

    private:
        geometry::Rectangle plot_area_;
        std::vector<LabelTarget> label_targets_;
    };

}