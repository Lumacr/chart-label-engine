#pragma once

#include <cmath>
#include <cstddef>
#include <stdexcept>

namespace chart_label_engine::application::placement {

class PlacementScore {
public:
    PlacementScore(
        std::size_t collision_count,
        double boundary_penalty,
        double anchor_distance
    )
        : collision_count_(collision_count),
          boundary_penalty_(boundary_penalty),
          anchor_distance_(anchor_distance) {

        if (!std::isfinite(boundary_penalty_)) {
            throw std::invalid_argument(
                "Boundary penalty must be finite."
            );
        }

        if (boundary_penalty_ < 0.0) {
            throw std::invalid_argument(
                "Boundary penalty cannot be negative."
            );
        }

        if (!std::isfinite(anchor_distance_)) {
            throw std::invalid_argument(
                "Anchor distance must be finite."
            );
        }

        if (anchor_distance_ < 0.0) {
            throw std::invalid_argument(
                "Anchor distance cannot be negative."
            );
        }
    }

    [[nodiscard]] std::size_t collision_count() const noexcept {
        return collision_count_;
    }

    [[nodiscard]] double boundary_penalty() const noexcept {
        return boundary_penalty_;
    }

    [[nodiscard]] double anchor_distance() const noexcept {
        return anchor_distance_;
    }

    [[nodiscard]] bool is_better_than(
        const PlacementScore& other
    ) const noexcept {

        if (collision_count_ != other.collision_count_) {
            return collision_count_ < other.collision_count_;
        }

        if (boundary_penalty_ != other.boundary_penalty_) {
            return boundary_penalty_ < other.boundary_penalty_;
        }

        return anchor_distance_ < other.anchor_distance_;
    }

    [[nodiscard]] bool operator==(
        const PlacementScore& other
    ) const noexcept = default;

private:
    std::size_t collision_count_;
    double boundary_penalty_;
    double anchor_distance_;
};

}