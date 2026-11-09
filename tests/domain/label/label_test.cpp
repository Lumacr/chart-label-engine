
#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
#include <string>

#include "chart_label_engine/domain/label/label.hpp"

using chart_label_engine::domain::label::Label;

TEST(LabelTest, ConstructsWithValidProperties) {
    const Label label{"Revenue", 80.0, 20.0};

    EXPECT_EQ(label.text(), "Revenue");
    EXPECT_DOUBLE_EQ(label.width(), 80.0);
    EXPECT_DOUBLE_EQ(label.height(), 20.0);
}

TEST(LabelTest, ReturnsCorrectText) {
    const Label label{"Revenue", 80.0, 20.0};

    EXPECT_EQ(label.text(), "Revenue");
}

TEST(LabelTest, ReturnsCorrectWidth) {
    const Label label{"Revenue", 80.0, 20.0};

    EXPECT_DOUBLE_EQ(label.width(), 80.0);
}

TEST(LabelTest, ReturnsCorrectHeight) {
    const Label label{"Revenue", 80.0, 20.0};

    EXPECT_DOUBLE_EQ(label.height(), 20.0);
}

TEST(LabelTest, LabelsWithSamePropertiesAreEqual) {
    const Label first{"Revenue", 80.0, 20.0};
    const Label second{"Revenue", 80.0, 20.0};

    EXPECT_EQ(first, second);
}

TEST(LabelTest, LabelsWithDifferentTextAreNotEqual) {
    const Label first{"Revenue", 80.0, 20.0};
    const Label second{"Profit", 80.0, 20.0};

    EXPECT_NE(first, second);
}

TEST(LabelTest, LabelsWithDifferentWidthsAreNotEqual) {
    const Label first{"Revenue", 80.0, 20.0};
    const Label second{"Revenue", 100.0, 20.0};

    EXPECT_NE(first, second);
}

TEST(LabelTest, LabelsWithDifferentHeightsAreNotEqual) {
    const Label first{"Revenue", 80.0, 20.0};
    const Label second{"Revenue", 80.0, 30.0};

    EXPECT_NE(first, second);
}

TEST(LabelTest, RejectsEmptyText) {
    EXPECT_THROW(
        (Label{"", 80.0, 20.0}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsZeroWidth) {
    EXPECT_THROW(
        (Label{"Revenue", 0.0, 20.0}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsNegativeWidth) {
    EXPECT_THROW(
        (Label{"Revenue", -10.0, 20.0}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsZeroHeight) {
    EXPECT_THROW(
        (Label{"Revenue", 80.0, 0.0}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsNegativeHeight) {
    EXPECT_THROW(
        (Label{"Revenue", 80.0, -10.0}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsNaNWidth) {
    const double nan = std::numeric_limits<double>::quiet_NaN();

    EXPECT_THROW(
        (Label{"Revenue", nan, 20.0}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsNaNHeight) {
    const double nan = std::numeric_limits<double>::quiet_NaN();

    EXPECT_THROW(
        (Label{"Revenue", 80.0, nan}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsInfiniteWidth) {
    const double infinity = std::numeric_limits<double>::infinity();

    EXPECT_THROW(
        (Label{"Revenue", infinity, 20.0}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsInfiniteHeight) {
    const double infinity = std::numeric_limits<double>::infinity();

    EXPECT_THROW(
        (Label{"Revenue", 80.0, infinity}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsNegativeInfiniteWidth) {
    const double infinity = -std::numeric_limits<double>::infinity();

    EXPECT_THROW(
        (Label{"Revenue", infinity, 20.0}),
        std::invalid_argument
    );
}

TEST(LabelTest, RejectsNegativeInfiniteHeight) {
    const double infinity = -std::numeric_limits<double>::infinity();

    EXPECT_THROW(
        (Label{"Revenue", 80.0, infinity}),
        std::invalid_argument
    );
}

TEST(LabelTest, AcceptsTextContainingOnlyWhitespace) {
    const Label label{"   ", 80.0, 20.0};

    EXPECT_EQ(label.text(), "   ");
}
