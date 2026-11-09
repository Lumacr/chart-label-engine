#include "chart_label_engine/domain/geometry/point2d.hpp"

#include <cmath>
#include <stdexcept>

namespace chart_label_engine::domain::geometry {

    Point2D::Point2D(const double x, const double y)
    : x_(x), y_(y) {
        if (!std::isfinite(x_) || !std::isfinite(y_)) {
            throw std::invalid_argument("Point2D coordinates must be finite.");
        }
    }

    double Point2D::x() const noexcept {
        return x_;
    }

    double Point2D::y() const noexcept {
        return y_;
    }

    double Point2D::distance_to(const Point2D& other) const noexcept {
        return std::hypot(x_ - other.x_, y_ - other.y_);
    }

    bool Point2D::operator==(const Point2D& other) const noexcept {
        return x_ == other.x_ && y_ == other.y_;
    }

}
