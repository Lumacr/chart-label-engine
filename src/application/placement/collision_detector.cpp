#include "chart_label_engine/application/placement/collision_detector.hpp"

namespace chart_label_engine::application::placement {

    std::vector<CollisionPair> CollisionDetector::detect(
        const std::vector<domain::placement::Placement>& placements
    ) const {

        std::vector<CollisionPair> collisions;

        for (std::size_t i = 0; i < placements.size(); ++i) {
            const auto first_bounds = placements[i].bounds();

            for (std::size_t j = i + 1; j < placements.size(); ++j) {
                const auto second_bounds = placements[j].bounds();

                if (first_bounds.overlaps(second_bounds)) {
                    collisions.push_back({i, j});
                }
            }
        }

        return collisions;
    }

}
