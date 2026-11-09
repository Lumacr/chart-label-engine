#include <cmath>
#include <stdexcept>

#include "chart_label_engine/domain/geometry/rectangle.hpp"

namespace chart_label_engine::domain::geometry {

    Rectangle::Rectangle(Point2D top_left, const double width, const double height)
        : top_left_(top_left), width_(width), height_(height) {

        if (!std::isfinite(width_) || !std::isfinite(height_)) {
            throw std::invalid_argument("Rectangle dimensions must be finite.");
        }

        if (width_ <= 0.0 || height_ <= 0.0) {
            throw std::invalid_argument("Rectangle dimensions must be positive.");
        }

        if (!std::isfinite(top_left_.x() + width_) ||
            !std::isfinite(top_left_.y() - height_)) {
            throw std::invalid_argument("Rectangle boundaries must be finite.");
        }
    }

    Point2D Rectangle::top_left() const noexcept {
        return top_left_;
    }

    double Rectangle::width() const noexcept {
        return width_;
    }

    double Rectangle::height() const noexcept {
        return height_;
    }

    double Rectangle::left() const noexcept {
        return top_left_.x();
    }

    double Rectangle::top() const noexcept {
        return top_left_.y();
    }

    double Rectangle::right() const noexcept {
        return top_left_.x() + width_;
    }

    double Rectangle::bottom() const noexcept {
        return top_left_.y() - height_;
    }

    double Rectangle::area() const {
        const double result = width_ * height_;

        if (!std::isfinite(result) || result == 0.0) {
            throw std::overflow_error("Rectangle area is not representable.");
        }

        return result;
    }

    Point2D Rectangle::center() const noexcept {
        return Point2D{
            left() + width_ / 2.0,
            top() - height_ / 2.0
        };
    }

    bool Rectangle::contains(const Point2D& point) const noexcept {
        return point.x() >= left() &&
               point.x() <= right() &&
               point.y() >= bottom() &&
               point.y() <= top();
    }

    bool Rectangle::intersects(const Rectangle& other) const noexcept {
        return left() <= other.right() &&
               right() >= other.left() &&
               bottom() <= other.top() &&
               top() >= other.bottom();
    }

    bool Rectangle::overlaps(const Rectangle& other) const noexcept {
        return left() < other.right() &&
               right() > other.left() &&
               bottom() < other.top() &&
               top() > other.bottom();
    }

    bool Rectangle::operator==(const Rectangle& other) const noexcept {
        return top_left_ == other.top_left_ &&
               width_ == other.width_ &&
               height_ == other.height_;
    }

}