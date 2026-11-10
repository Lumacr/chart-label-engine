#include "chart_label_engine/application/placement/placement_scorer.hpp"

#include "chart_label_engine/application/placement/collision_detector.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace chart_label_engine::application::placement {
namespace {

double calculate_boundary_penalty(
    const domain::geometry::Rectangle& bounds,
    const domain::geometry::Rectangle& plot_area
) {
    double penalty = 0.0;

    penalty += std::max(0.0, plot_area.left() - bounds.left());
    penalty += std::max(0.0, bounds.right() - plot_area.right());
    penalty += std::max(0.0, bounds.top() - plot_area.top());
    penalty += std::max(0.0, plot_area.bottom() - bounds.bottom());

    if (!std::isfinite(penalty)) {
        throw std::overflow_error(
            "Boundary penalty calculation overflowed."
        );
    }

    return penalty;
}

}

PlacementScore PlacementScorer::score(
    const domain::chart::Chart& chart,
    const std::vector<domain::placement::Placement>& placements
) const {

    const auto& targets = chart.label_targets();

    if (placements.size() != targets.size()) {
        throw std::invalid_argument(
            "The number of placements must match the number of label targets."
        );
    }

    for (std::size_t i = 0; i < placements.size(); ++i) {
        if (placements[i].label() != targets[i].label()) {
            throw std::invalid_argument(
                "Each placement must contain the label of its corresponding target."
            );
        }
    }

    const CollisionDetector collision_detector;
    const auto collisions = collision_detector.detect(placements);

    const std::size_t collision_count = collisions.size();

    double boundary_penalty = 0.0;
    double anchor_distance = 0.0;

    const auto& plot_area = chart.plot_area();

    for (std::size_t i = 0; i < placements.size(); ++i) {
        const auto bounds = placements[i].bounds();

        boundary_penalty += calculate_boundary_penalty(
            bounds,
            plot_area
        );

        if (!std::isfinite(boundary_penalty)) {
            throw std::overflow_error(
                "Total boundary penalty calculation overflowed."
            );
        }

        const auto center = bounds.center();
        const auto distance = center.distance_to(targets[i].anchor());

        if (!std::isfinite(distance)) {
            throw std::overflow_error(
                "Anchor distance calculation overflowed."
            );
        }

        anchor_distance += distance;

        if (!std::isfinite(anchor_distance)) {
            throw std::overflow_error(
                "Total anchor distance calculation overflowed."
            );
        }
    }

    return PlacementScore{
        collision_count,
        boundary_penalty,
        anchor_distance
    };
}

}
