#pragma once

#include <cstddef>

namespace chart_label_engine::application::placement {

    struct CollisionPair {
        std::size_t first_index;
        std::size_t second_index;

        [[nodiscard]] bool operator==(const CollisionPair& other) const noexcept = default;
    };

}
