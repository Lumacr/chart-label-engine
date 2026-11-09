#pragma once

#include "chart_label_engine/domain/geometry/point2d.hpp"
#include "chart_label_engine/domain/geometry/rectangle.hpp"
#include "chart_label_engine/domain/label/label.hpp"

namespace chart_label_engine::domain::placement {

    class Placement {
    public:
        Placement(label::Label label, geometry::Point2D top_left);

        [[nodiscard]] const label::Label& label() const noexcept;
        [[nodiscard]] geometry::Point2D top_left() const noexcept;
        [[nodiscard]] geometry::Rectangle bounds() const;

        [[nodiscard]] bool operator==(const Placement& other) const noexcept = default;

    private:
        label::Label label_;
        geometry::Point2D top_left_;
    };

}