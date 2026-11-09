#include <gtest/gtest.h>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "chart_label_engine/domain/geometry/point2d.hpp"

using chart_label_engine::domain::geometry::Point2D;

TEST(Point2DTest, ConstructsWithPositiveCoordinates) {
    const Point2D point{10.0, 20.0};

    EXPECT_DOUBLE_EQ(point.x(), 10.0);
    EXPECT_DOUBLE_EQ(point.y(), 20.0);
}

TEST(Point2DTest, ConstructsWithNegativeCoordinates) {
    const Point2D point{-10.0, -20.0};

    EXPECT_DOUBLE_EQ(point.x(), -10.0);
    EXPECT_DOUBLE_EQ(point.y(), -20.0);
}

TEST(Point2DTest, PointsWithSameCoordinatesAreEqual) {
    const Point2D first{10.0, 20.0};
    const Point2D second{10.0, 20.0};

    EXPECT_EQ(first, second);
}

TEST(Point2DTest, PointsWithDifferentXCoordinatesAreNotEqual) {
    const Point2D first{10.0, 20.0};
    const Point2D second{15.0, 20.0};

    EXPECT_NE(first, second);
}

TEST(Point2DTest, PointsWithDifferentYCoordinatesAreNotEqual) {
    const Point2D first{10.0, 20.0};
    const Point2D second{10.0, 25.0};

    EXPECT_NE(first, second);
}

TEST(Point2DTest, RejectsNaNCoordinates) {
    const double nan = std::numeric_limits<double>::quiet_NaN();

    EXPECT_THROW(Point2D(nan, 10.0), std::invalid_argument);
    EXPECT_THROW(Point2D(10.0, nan), std::invalid_argument);
}

TEST(Point2DTest, RejectsInfiniteCoordinates) {
    const double infinity = std::numeric_limits<double>::infinity();

    EXPECT_THROW(Point2D(infinity, 10.0), std::invalid_argument);
    EXPECT_THROW(Point2D(-infinity, 10.0), std::invalid_argument);
    EXPECT_THROW(Point2D(10.0, infinity), std::invalid_argument);
    EXPECT_THROW(Point2D(10.0, -infinity), std::invalid_argument);
}

TEST(Point2DTest, CalculatesDistanceAlongXAxis) {
    const Point2D first{0.0, 0.0};
    const Point2D second{3.0, 0.0};

    EXPECT_DOUBLE_EQ(first.distance_to(second), 3.0);
}

TEST(Point2DTest, CalculatesDistanceAlongYAxis) {
    const Point2D first{0.0, 0.0};
    const Point2D second{0.0, 4.0};

    EXPECT_DOUBLE_EQ(first.distance_to(second), 4.0);
}

TEST(Point2DTest, CalculatesEuclideanDistance) {
    const Point2D first{0.0, 0.0};
    const Point2D second{3.0, 4.0};

    EXPECT_DOUBLE_EQ(first.distance_to(second), 5.0);
}

TEST(Point2DTest, CalculatesDistanceWithNegativeCoordinates) {
    const Point2D first{-3.0, -4.0};
    const Point2D second{0.0, 0.0};

    EXPECT_DOUBLE_EQ(first.distance_to(second), 5.0);
}

TEST(Point2DTest, DistanceToItselfIsZero) {
    const Point2D point{10.0, 20.0};

    EXPECT_DOUBLE_EQ(point.distance_to(point), 0.0);
}

TEST(Point2DTest, DistanceIsSymmetric) {
    const Point2D first{2.0, 3.0};
    const Point2D second{7.0, 11.0};

    EXPECT_DOUBLE_EQ(first.distance_to(second), second.distance_to(first));
}

TEST(Point2DTest, CalculatesDistanceWithoutIntermediateOverflow) {
    const Point2D first{0.0, 0.0};
    const Point2D second{1.0e200, 1.0e200};

    const double distance = first.distance_to(second);

    EXPECT_TRUE(std::isfinite(distance));
    EXPECT_NEAR(distance, 1.4142135623730951e200, 1.0e185);
}