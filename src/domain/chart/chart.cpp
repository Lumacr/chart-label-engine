#include "chart_label_engine/domain/chart/chart.hpp"

#include <utility>

namespace chart_label_engine::domain::chart {

    Chart::Chart(
        geometry::Rectangle plot_area,
        std::vector<LabelTarget> label_targets
    )
        : plot_area_(std::move(plot_area)),
          label_targets_(std::move(label_targets)) {
    }

    const geometry::Rectangle& Chart::plot_area() const noexcept {
        return plot_area_;
    }

    const std::vector<LabelTarget>& Chart::label_targets() const noexcept {
        return label_targets_;
    }

}