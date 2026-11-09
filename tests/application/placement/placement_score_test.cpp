#include "chart_label_engine/application/placement/placement_score.hpp"

#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>

namespace chart_label_engine::application::placement {
namespace {

TEST(PlacementScoreTest, ConstructsWithValidValues) {
    const PlacementScore score{2, 15.0, 30.0};

    EXPECT_EQ(score.collision_count(), 2);
    EXPECT_DOUBLE_EQ(score.boundary_penalty(), 15.0);
    EXPECT_DOUBLE_EQ(score.anchor_distance(), 30.0);
}

TEST(PlacementScoreTest, AcceptsZeroValues) {
    const PlacementScore score{0, 0.0, 0.0};

    EXPECT_EQ(score.collision_count(), 0);
    EXPECT_DOUBLE_EQ(score.boundary_penalty(), 0.0);
    EXPECT_DOUBLE_EQ(score.anchor_distance(), 0.0);
}

TEST(PlacementScoreTest, EqualScoresAreEqual) {
    const PlacementScore first{1, 10.0, 20.0};
    const PlacementScore second{1, 10.0, 20.0};

    EXPECT_EQ(first, second);
}

TEST(PlacementScoreTest, DifferentScoresAreNotEqual) {
    const PlacementScore first{1, 10.0, 20.0};
    const PlacementScore second{2, 10.0, 20.0};

    EXPECT_NE(first, second);
}

TEST(PlacementScoreTest, PrefersFewerCollisions) {
    const PlacementScore better{0, 100.0, 100.0};
    const PlacementScore worse{1, 0.0, 0.0};

    EXPECT_TRUE(better.is_better_than(worse));
    EXPECT_FALSE(worse.is_better_than(better));
}

TEST(PlacementScoreTest, PrefersLowerBoundaryPenaltyWhenCollisionsAreEqual) {
    const PlacementScore better{1, 5.0, 100.0};
    const PlacementScore worse{1, 10.0, 0.0};

    EXPECT_TRUE(better.is_better_than(worse));
}

TEST(PlacementScoreTest, PrefersLowerAnchorDistanceWhenPreviousCriteriaAreEqual) {
    const PlacementScore better{1, 10.0, 15.0};
    const PlacementScore worse{1, 10.0, 20.0};

    EXPECT_TRUE(better.is_better_than(worse));
}

TEST(PlacementScoreTest, DoesNotPreferEqualScores) {
    const PlacementScore first{1, 10.0, 20.0};
    const PlacementScore second{1, 10.0, 20.0};

    EXPECT_FALSE(first.is_better_than(second));
    EXPECT_FALSE(second.is_better_than(first));
}

TEST(PlacementScoreTest, RejectsNegativeBoundaryPenalty) {
    EXPECT_THROW(
        (PlacementScore{0, -1.0, 10.0}),
        std::invalid_argument
    );
}

TEST(PlacementScoreTest, RejectsNonFiniteBoundaryPenalty) {
    EXPECT_THROW(
        (PlacementScore{0, std::numeric_limits<double>::quiet_NaN(), 10.0}),
        std::invalid_argument
    );

    EXPECT_THROW(
        (PlacementScore{0, std::numeric_limits<double>::infinity(), 10.0}),
        std::invalid_argument
    );
}

TEST(PlacementScoreTest, RejectsNegativeAnchorDistance) {
    EXPECT_THROW(
        (PlacementScore{0, 10.0, -1.0}),
        std::invalid_argument
    );
}

TEST(PlacementScoreTest, RejectsNonFiniteAnchorDistance) {
    EXPECT_THROW(
        (PlacementScore{0, 10.0, std::numeric_limits<double>::quiet_NaN()}),
        std::invalid_argument
    );

    EXPECT_THROW(
        (PlacementScore{0, 10.0, std::numeric_limits<double>::infinity()}),
        std::invalid_argument
    );
}

}
}