
#include "chart_label_engine/domain/placement/placement.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <utility>

namespace chart_label_engine::domain::placement::test {

using geometry::Point2D;
using geometry::Rectangle;
using label::Label;

TEST(PlacementTest, ConstructsWithValidLabelAndPosition) {
    const Label label{"Revenue", 80.0, 20.0};
    const Point2D top_left{100.0, 200.0};

    const Placement placement{label, top_left};

    EXPECT_EQ(placement.label(), label);
    EXPECT_EQ(placement.top_left(), top_left);
}

TEST(PlacementTest, ReturnsAssociatedLabel) {
    const Placement placement{
        Label{"Revenue", 80.0, 20.0},
        Point2D{100.0, 200.0}
    };

    EXPECT_EQ(placement.label().text(), "Revenue");
    EXPECT_DOUBLE_EQ(placement.label().width(), 80.0);
    EXPECT_DOUBLE_EQ(placement.label().height(), 20.0);
}

TEST(PlacementTest, ReturnsTopLeftPosition) {
    const Point2D expected{100.0, 200.0};

    const Placement placement{
        Label{"Revenue", 80.0, 20.0},
        expected
    };

    EXPECT_EQ(placement.top_left(), expected);
}

TEST(PlacementTest, CalculatesBoundsUsingLabelDimensions) {
    const Placement placement{
        Label{"Revenue", 80.0, 20.0},
        Point2D{100.0, 200.0}
    };

    const Rectangle expected{
        Point2D{100.0, 200.0},
        80.0,
        20.0
    };

    EXPECT_EQ(placement.bounds(), expected);
}

TEST(PlacementTest, CalculatesBoundsWithNegativeCoordinates) {
    const Placement placement{
        Label{"Revenue", 80.0, 20.0},
        Point2D{-100.0, -50.0}
    };

    const Rectangle expected{
        Point2D{-100.0, -50.0},
        80.0,
        20.0
    };

    EXPECT_EQ(placement.bounds(), expected);
}

TEST(PlacementTest, BoundsReflectLabelDimensions) {
    const Placement placement{
        Label{"Revenue", 125.5, 35.5},
        Point2D{10.0, 50.0}
    };

    const Rectangle bounds = placement.bounds();

    EXPECT_DOUBLE_EQ(bounds.width(), 125.5);
    EXPECT_DOUBLE_EQ(bounds.height(), 35.5);
}

TEST(PlacementTest, EqualPlacementsAreEqual) {
    const Placement first{
        Label{"Revenue", 80.0, 20.0},
        Point2D{100.0, 200.0}
    };

    const Placement second{
        Label{"Revenue", 80.0, 20.0},
        Point2D{100.0, 200.0}
    };

    EXPECT_EQ(first, second);
}

TEST(PlacementTest, DifferentLabelsAreNotEqual) {
    const Placement first{
        Label{"Revenue", 80.0, 20.0},
        Point2D{100.0, 200.0}
    };

    const Placement second{
        Label{"Expenses", 80.0, 20.0},
        Point2D{100.0, 200.0}
    };

    EXPECT_NE(first, second);
}

TEST(PlacementTest, DifferentLabelDimensionsAreNotEqual) {
    const Placement first{
        Label{"Revenue", 80.0, 20.0},
        Point2D{100.0, 200.0}
    };

    const Placement second{
        Label{"Revenue", 100.0, 20.0},
        Point2D{100.0, 200.0}
    };

    EXPECT_NE(first, second);
}

TEST(PlacementTest, DifferentPositionsAreNotEqual) {
    const Placement first{
        Label{"Revenue", 80.0, 20.0},
        Point2D{100.0, 200.0}
    };

    const Placement second{
        Label{"Revenue", 80.0, 20.0},
        Point2D{150.0, 200.0}
    };

    EXPECT_NE(first, second);
}

TEST(PlacementTest, OwnsLabelIndependentlyOfOriginalObject) {
    Label original{"Revenue", 80.0, 20.0};

    const Placement placement{
        std::move(original),
        Point2D{100.0, 200.0}
    };

    EXPECT_EQ(placement.label().text(), "Revenue");
    EXPECT_DOUBLE_EQ(placement.label().width(), 80.0);
    EXPECT_DOUBLE_EQ(placement.label().height(), 20.0);
}

TEST(PlacementTest, RejectsRightBoundaryOverflow) {
    const double maximum = std::numeric_limits<double>::max();

    EXPECT_THROW(
        (Placement{
            Label{"Revenue", maximum, 20.0},
            Point2D{maximum, 200.0}
        }),
        std::invalid_argument
    );
}

TEST(PlacementTest, RejectsBottomBoundaryOverflow) {
    const double maximum = std::numeric_limits<double>::max();

    EXPECT_THROW(
        (Placement{
            Label{"Revenue", 80.0, maximum},
            Point2D{100.0, -maximum}
        }),
        std::invalid_argument
    );
}

}
