#pragma once

#include "chart_label_engine/application/placement/collision_pair.hpp"
#include "chart_label_engine/domain/placement/placement.hpp"

#include <vector>

namespace chart_label_engine::application::placement {

    class CollisionDetector {
    public:
        [[nodiscard]] std::vector<CollisionPair> detect(
            const std::vector<domain::placement::Placement>& placements
        ) const;
    };

}
