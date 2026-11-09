#pragma once

#include "chart_label_engine/domain/geometry/point2d.hpp"
#include "chart_label_engine/domain/label/label.hpp"

namespace chart_label_engine::domain::chart {

    class LabelTarget {
    public:
        LabelTarget(label::Label label, geometry::Point2D anchor);

        [[nodiscard]] const label::Label& label() const noexcept;
        [[nodiscard]] geometry::Point2D anchor() const noexcept;

        [[nodiscard]] bool operator==(const LabelTarget& other) const noexcept = default;

    private:
        label::Label label_;
        geometry::Point2D anchor_;
    };

}