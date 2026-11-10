
#pragma once

#include "chart_label_engine/domain/chart/chart.hpp"
#include "chart_label_engine/domain/chart/label_target.hpp"
#include "chart_label_engine/domain/geometry/point2d.hpp"
#include "chart_label_engine/domain/geometry/rectangle.hpp"
#include "chart_label_engine/domain/label/label.hpp"
#include "chart_label_engine/domain/placement/placement.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace benchmark_support {

namespace domain = chart_label_engine::domain;

inline constexpr double LABEL_WIDTH = 40.0;
inline constexpr double LABEL_HEIGHT = 12.0;
inline constexpr double GRID_SPACING = 100.0;
inline constexpr std::size_t GRID_COLUMNS = 25;

struct BenchmarkData {
    domain::chart::Chart chart;
    std::vector<domain::placement::Placement> placements;
};

inline domain::chart::LabelTarget make_label_target(
    std::size_t index
) {
    const auto row = index / GRID_COLUMNS;
    const auto column = index % GRID_COLUMNS;

    domain::label::Label label{
        "Label " + std::to_string(index),
        LABEL_WIDTH,
        LABEL_HEIGHT
    };

    domain::geometry::Point2D anchor{
        100.0 + static_cast<double>(column) * GRID_SPACING,
        100.0 - static_cast<double>(row) * GRID_SPACING
    };

    return {std::move(label), anchor};
}

inline BenchmarkData make_benchmark_data(std::size_t count) {
    std::vector<domain::chart::LabelTarget> targets;
    std::vector<domain::placement::Placement> placements;

    targets.reserve(count);
    placements.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        auto target = make_label_target(i);

        const auto anchor = target.anchor();

        placements.emplace_back(
            target.label(),
            domain::geometry::Point2D{
                anchor.x() - LABEL_WIDTH / 2.0,
                anchor.y() + LABEL_HEIGHT / 2.0
            }
        );

        targets.push_back(std::move(target));
    }

    domain::geometry::Rectangle plot_area{
        domain::geometry::Point2D{0.0, 1000.0},
        3000.0,
        3000.0
    };

    return {
        domain::chart::Chart{
            std::move(plot_area),
            std::move(targets)
        },
        std::move(placements)
    };
}

} // namespace benchmark_support
