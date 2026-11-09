#include "chart_label_engine/domain/chart/label_target.hpp"

#include <utility>

namespace chart_label_engine::domain::chart {

    LabelTarget::LabelTarget(label::Label label, geometry::Point2D anchor)
        : label_(std::move(label)),
          anchor_(anchor) {
    }

    const label::Label& LabelTarget::label() const noexcept {
        return label_;
    }

    geometry::Point2D LabelTarget::anchor() const noexcept {
        return anchor_;
    }

}