#pragma once

#include "chart_label_engine/domain/chart/label_target.hpp"
#include "chart_label_engine/domain/placement/placement.hpp"

#include <vector>

namespace chart_label_engine::application::placement {

    class CandidateGenerator {
    public:
        explicit CandidateGenerator(double gap = 5.0);

        [[nodiscard]] std::vector<domain::placement::Placement> generate(
            const domain::chart::LabelTarget& target
        ) const;

    private:
        double gap_;
    };

}
