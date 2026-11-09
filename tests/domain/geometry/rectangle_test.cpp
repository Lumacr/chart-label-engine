#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>

#include "chart_label_engine/domain/geometry/rectangle.hpp"

using chart_label_engine::domain::geometry::Point2D;
using chart_label_engine::domain::geometry::Rectangle;

TEST(RectangleTest, ConstructsWithValidDimensions) {
    const Rectangle rectangle{Point2D{10.0, 20.0}, 30.0, 15.0};

    EXPECT_DOUBLE_EQ(rectangle.width(), 30.0);
    EXPECT_DOUBLE_EQ(rectangle.height(), 15.0);
}

TEST(RectangleTest, ReturnsCorrectBoundaries) {
    const Rectangle rectangle{Point2D{10.0, 20.0}, 30.0, 15.0};

    EXPECT_DOUBLE_EQ(rectangle.left(), 10.0);
    EXPECT_DOUBLE_EQ(rectangle.top(), 20.0);
    EXPECT_DOUBLE_EQ(rectangle.right(), 40.0);
    EXPECT_DOUBLE_EQ(rectangle.bottom(), 5.0);
}

TEST(RectangleTest, ReturnsTopLeftPoint) {
    const Rectangle rectangle{Point2D{10.0, 20.0}, 30.0, 15.0};

    EXPECT_EQ(rectangle.top_left(), Point2D(10.0, 20.0));
}

TEST(RectangleTest, SupportsNegativeCoordinates) {
    const Rectangle rectangle{Point2D{-20.0, -10.0}, 15.0, 8.0};

    EXPECT_DOUBLE_EQ(rectangle.left(), -20.0);
    EXPECT_DOUBLE_EQ(rectangle.top(), -10.0);
    EXPECT_DOUBLE_EQ(rectangle.right(), -5.0);
    EXPECT_DOUBLE_EQ(rectangle.bottom(), -18.0);
}

TEST(RectangleTest, RectanglesWithSamePropertiesAreEqual) {
    const Rectangle first{Point2D{10.0, 20.0}, 30.0, 15.0};
    const Rectangle second{Point2D{10.0, 20.0}, 30.0, 15.0};

    EXPECT_EQ(first, second);
}

TEST(RectangleTest, RectanglesWithDifferentPropertiesAreNotEqual) {
    const Rectangle first{Point2D{10.0, 20.0}, 30.0, 15.0};
    const Rectangle second{Point2D{10.0, 20.0}, 40.0, 15.0};

    EXPECT_NE(first, second);
}

TEST(RectangleTest, RejectsZeroDimensions) {
    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, 0.0, 15.0),
        std::invalid_argument
    );

    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, 30.0, 0.0),
        std::invalid_argument
    );
}

TEST(RectangleTest, RejectsNegativeDimensions) {
    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, -30.0, 15.0),
        std::invalid_argument
    );

    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, 30.0, -15.0),
        std::invalid_argument
    );
}

TEST(RectangleTest, RejectsNaNDimensions) {
    const double nan = std::numeric_limits<double>::quiet_NaN();

    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, nan, 15.0),
        std::invalid_argument
    );

    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, 30.0, nan),
        std::invalid_argument
    );
}

TEST(RectangleTest, RejectsInfiniteDimensions) {
    const double infinity = std::numeric_limits<double>::infinity();

    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, infinity, 15.0),
        std::invalid_argument
    );

    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, 30.0, infinity),
        std::invalid_argument
    );

    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, -infinity, 15.0),
        std::invalid_argument
    );

    EXPECT_THROW(
        Rectangle(Point2D{10.0, 20.0}, 30.0, -infinity),
        std::invalid_argument
    );
}

TEST(RectangleTest, RejectsOverflowInRightBoundary) {
    const double large_value = 1.0e308;

    EXPECT_THROW(
        Rectangle(Point2D{large_value, 0.0}, large_value, 10.0),
        std::invalid_argument
    );
}

TEST(RectangleTest, RejectsOverflowInBottomBoundary) {
    const double large_value = 1.0e308;

    EXPECT_THROW(
        Rectangle(Point2D{0.0, -large_value}, 10.0, large_value),
        std::invalid_argument
    );
}

TEST(RectangleTest, ContainsPointInside) {
    const Rectangle rectangle{Point2D{0.0, 10.0}, 10.0, 10.0};

    EXPECT_TRUE(rectangle.contains(Point2D{5.0, 5.0}));
}

TEST(RectangleTest, ContainsPointOnBoundary) {
    const Rectangle rectangle{Point2D{0.0, 10.0}, 10.0, 10.0};

    EXPECT_TRUE(rectangle.contains(Point2D{0.0, 5.0}));
    EXPECT_TRUE(rectangle.contains(Point2D{10.0, 5.0}));
    EXPECT_TRUE(rectangle.contains(Point2D{5.0, 10.0}));
    EXPECT_TRUE(rectangle.contains(Point2D{5.0, 0.0}));
}

TEST(RectangleTest, DoesNotContainPointOutside) {
    const Rectangle rectangle{Point2D{0.0, 10.0}, 10.0, 10.0};

    EXPECT_FALSE(rectangle.contains(Point2D{11.0, 5.0}));
    EXPECT_FALSE(rectangle.contains(Point2D{-1.0, 5.0}));
    EXPECT_FALSE(rectangle.contains(Point2D{5.0, 11.0}));
    EXPECT_FALSE(rectangle.contains(Point2D{5.0, -1.0}));
}

TEST(RectangleTest, IntersectsWithOverlappingRectangle) {
    const Rectangle first{Point2D{0.0, 10.0}, 10.0, 10.0};
    const Rectangle second{Point2D{5.0, 8.0}, 10.0, 10.0};

    EXPECT_TRUE(first.intersects(second));
    EXPECT_TRUE(second.intersects(first));
}

TEST(RectangleTest, IntersectsWhenSharingAnEdge) {
    const Rectangle first{Point2D{0.0, 10.0}, 10.0, 10.0};
    const Rectangle second{Point2D{10.0, 10.0}, 10.0, 10.0};

    EXPECT_TRUE(first.intersects(second));
    EXPECT_TRUE(second.intersects(first));
}

TEST(RectangleTest, IntersectsWhenSharingACorner) {
    const Rectangle first{Point2D{0.0, 10.0}, 10.0, 10.0};
    const Rectangle second{Point2D{10.0, 0.0}, 10.0, 10.0};

    EXPECT_TRUE(first.intersects(second));
    EXPECT_TRUE(second.intersects(first));
}

TEST(RectangleTest, DoesNotIntersectWhenSeparated) {
    const Rectangle first{Point2D{0.0, 10.0}, 10.0, 10.0};
    const Rectangle second{Point2D{11.0, 10.0}, 10.0, 10.0};

    EXPECT_FALSE(first.intersects(second));
    EXPECT_FALSE(second.intersects(first));
}

TEST(RectangleTest, OverlapsWithPositiveArea) {
    const Rectangle first{Point2D{0.0, 10.0}, 10.0, 10.0};
    const Rectangle second{Point2D{5.0, 8.0}, 10.0, 10.0};

    EXPECT_TRUE(first.overlaps(second));
    EXPECT_TRUE(second.overlaps(first));
}

TEST(RectangleTest, DoesNotOverlapWhenSharingAnEdge) {
    const Rectangle first{Point2D{0.0, 10.0}, 10.0, 10.0};
    const Rectangle second{Point2D{10.0, 10.0}, 10.0, 10.0};

    EXPECT_FALSE(first.overlaps(second));
    EXPECT_FALSE(second.overlaps(first));
}

TEST(RectangleTest, DoesNotOverlapWhenSharingACorner) {
    const Rectangle first{Point2D{0.0, 10.0}, 10.0, 10.0};
    const Rectangle second{Point2D{10.0, 0.0}, 10.0, 10.0};

    EXPECT_FALSE(first.overlaps(second));
    EXPECT_FALSE(second.overlaps(first));
}

TEST(RectangleTest, DoesNotOverlapWhenSeparated) {
    const Rectangle first{Point2D{0.0, 10.0}, 10.0, 10.0};
    const Rectangle second{Point2D{11.0, 10.0}, 10.0, 10.0};

    EXPECT_FALSE(first.overlaps(second));
    EXPECT_FALSE(second.overlaps(first));
}

TEST(RectangleTest, RectangleOverlapsWithItself) {
    const Rectangle rectangle{Point2D{0.0, 10.0}, 10.0, 10.0};

    EXPECT_TRUE(rectangle.intersects(rectangle));
    EXPECT_TRUE(rectangle.overlaps(rectangle));
}

// Area tests

TEST(RectangleTest, CalculatesArea) {
    const Rectangle rectangle{Point2D{0.0, 10.0}, 10.0, 5.0};

    EXPECT_DOUBLE_EQ(rectangle.area(), 50.0);
}

TEST(RectangleTest, CalculatesAreaWithDecimalDimensions) {
    const Rectangle rectangle{Point2D{0.0, 10.0}, 2.5, 4.0};

    EXPECT_DOUBLE_EQ(rectangle.area(), 10.0);
}

TEST(RectangleTest, CalculatesVerySmallArea) {
    const Rectangle rectangle{Point2D{0.0, 1.0}, 1.0e-150, 1.0e-150};

    EXPECT_DOUBLE_EQ(rectangle.area(), 1.0e-300);
}

TEST(RectangleTest, RejectsAreaOverflow) {
    const Rectangle rectangle{Point2D{0.0, 10.0}, 1.0e200, 1.0e200};

    EXPECT_THROW(
        static_cast<void>(rectangle.area()),
        std::overflow_error
    );
}

// Center tests

TEST(RectangleTest, CalculatesCenterWithPositiveCoordinates) {
    const Rectangle rectangle{Point2D{10.0, 20.0}, 10.0, 10.0};

    EXPECT_EQ(rectangle.center(), Point2D(15.0, 15.0));
}

TEST(RectangleTest, CalculatesCenterWithNegativeCoordinates) {
    const Rectangle rectangle{Point2D{-20.0, -10.0}, 10.0, 10.0};

    EXPECT_EQ(rectangle.center(), Point2D(-15.0, -15.0));
}

TEST(RectangleTest, CalculatesCenterWithOddAndEvenDimensions) {
    const Rectangle odd_rectangle{Point2D{0.0, 10.0}, 5.0, 3.0};
    const Rectangle even_rectangle{Point2D{0.0, 10.0}, 4.0, 2.0};

    EXPECT_EQ(odd_rectangle.center(), Point2D(2.5, 8.5));
    EXPECT_EQ(even_rectangle.center(), Point2D(2.0, 9.0));
}

TEST(RectangleTest, CalculatesCenterAwayFromOrigin) {
    const Rectangle rectangle{Point2D{25.0, 35.0}, 8.0, 12.0};

    EXPECT_EQ(rectangle.center(), Point2D(29.0, 29.0));
}