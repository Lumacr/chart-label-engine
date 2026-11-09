
#include "chart_label_engine/domain/chart/label_target.hpp"

#include <gtest/gtest.h>

#include <utility>

namespace chart_label_engine::domain::chart {

TEST(LabelTargetTest, ConstructsWithValidLabelAndAnchor) {
    const label::Label label{"Revenue", 80.0, 20.0};
    const geometry::Point2D anchor{100.0, 200.0};

    const LabelTarget target{label, anchor};

    EXPECT_EQ(target.label(), label);
    EXPECT_EQ(target.anchor(), anchor);
}

TEST(LabelTargetTest, ReturnsAssociatedLabel) {
    const label::Label label{"Revenue", 80.0, 20.0};
    const LabelTarget target{label, geometry::Point2D{100.0, 200.0}};

    EXPECT_EQ(target.label().text(), "Revenue");
    EXPECT_DOUBLE_EQ(target.label().width(), 80.0);
    EXPECT_DOUBLE_EQ(target.label().height(), 20.0);
}

TEST(LabelTargetTest, ReturnsAnchorPosition) {
    const geometry::Point2D anchor{100.0, 200.0};
    const LabelTarget target{
        label::Label{"Revenue", 80.0, 20.0},
        anchor
    };

    EXPECT_EQ(target.anchor(), anchor);
}

TEST(LabelTargetTest, EqualLabelTargetsAreEqual) {
    const LabelTarget first{
        label::Label{"Revenue", 80.0, 20.0},
        geometry::Point2D{100.0, 200.0}
    };

    const LabelTarget second{
        label::Label{"Revenue", 80.0, 20.0},
        geometry::Point2D{100.0, 200.0}
    };

    EXPECT_EQ(first, second);
}

TEST(LabelTargetTest, DifferentLabelsAreNotEqual) {
    const LabelTarget first{
        label::Label{"Revenue", 80.0, 20.0},
        geometry::Point2D{100.0, 200.0}
    };

    const LabelTarget second{
        label::Label{"Expenses", 80.0, 20.0},
        geometry::Point2D{100.0, 200.0}
    };

    EXPECT_NE(first, second);
}

TEST(LabelTargetTest, DifferentLabelDimensionsAreNotEqual) {
    const LabelTarget first{
        label::Label{"Revenue", 80.0, 20.0},
        geometry::Point2D{100.0, 200.0}
    };

    const LabelTarget second{
        label::Label{"Revenue", 100.0, 30.0},
        geometry::Point2D{100.0, 200.0}
    };

    EXPECT_NE(first, second);
}

TEST(LabelTargetTest, DifferentAnchorXCoordinatesAreNotEqual) {
    const LabelTarget first{
        label::Label{"Revenue", 80.0, 20.0},
        geometry::Point2D{100.0, 200.0}
    };

    const LabelTarget second{
        label::Label{"Revenue", 80.0, 20.0},
        geometry::Point2D{150.0, 200.0}
    };

    EXPECT_NE(first, second);
}

TEST(LabelTargetTest, DifferentAnchorYCoordinatesAreNotEqual) {
    const LabelTarget first{
        label::Label{"Revenue", 80.0, 20.0},
        geometry::Point2D{100.0, 200.0}
    };

    const LabelTarget second{
        label::Label{"Revenue", 80.0, 20.0},
        geometry::Point2D{100.0, 250.0}
    };

    EXPECT_NE(first, second);
}

TEST(LabelTargetTest, OwnsLabelIndependentlyOfOriginalObject) {
    label::Label original_label{"Revenue", 80.0, 20.0};

    const LabelTarget target{
        std::move(original_label),
        geometry::Point2D{100.0, 200.0}
    };

    EXPECT_EQ(target.label().text(), "Revenue");
    EXPECT_DOUBLE_EQ(target.label().width(), 80.0);
    EXPECT_DOUBLE_EQ(target.label().height(), 20.0);
}

}
