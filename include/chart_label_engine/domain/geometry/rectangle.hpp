#pragma once

#include "chart_label_engine/domain/geometry/point2d.hpp"

namespace chart_label_engine::domain::geometry {

    class Rectangle {
    public:
        Rectangle(Point2D top_left, double width, double height);

        [[nodiscard]] Point2D top_left() const noexcept;
        [[nodiscard]] double width() const noexcept;
        [[nodiscard]] double height() const noexcept;

        [[nodiscard]] double left() const noexcept;
        [[nodiscard]] double top() const noexcept;
        [[nodiscard]] double right() const noexcept;
        [[nodiscard]] double bottom() const noexcept;

        [[nodiscard]] double area() const;
        [[nodiscard]] Point2D center() const noexcept;

        [[nodiscard]] bool contains(const Point2D& point) const noexcept;
        [[nodiscard]] bool intersects(const Rectangle& other) const noexcept;
        [[nodiscard]] bool overlaps(const Rectangle& other) const noexcept;

        [[nodiscard]] bool operator==(const Rectangle& other) const noexcept;

    private:
        Point2D top_left_;
        double width_;
        double height_;
    };

}