#pragma once

namespace chart_label_engine::domain::geometry {

    class Point2D {
    public:
        Point2D(double x, double y);

        [[nodiscard]] double x() const noexcept;
        [[nodiscard]] double y() const noexcept;

        [[nodiscard]] double distance_to(const Point2D& other) const noexcept;

        [[nodiscard]] bool operator==(const Point2D& other) const noexcept;

    private:
        double x_;
        double y_;
    };

}
