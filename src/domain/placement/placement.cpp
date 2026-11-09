#include "chart_label_engine/domain/placement/placement.hpp"

#include <utility>

namespace chart_label_engine::domain::placement {

    Placement::Placement(label::Label label, geometry::Point2D top_left)
        : label_(std::move(label)),
          top_left_(top_left) {

        static_cast<void>(bounds());
    }

    const label::Label& Placement::label() const noexcept {
        return label_;
    }

    geometry::Point2D Placement::top_left() const noexcept {
        return top_left_;
    }

    geometry::Rectangle Placement::bounds() const {
        return geometry::Rectangle{
            top_left_,
            label_.width(),
            label_.height()
        };
    }

}