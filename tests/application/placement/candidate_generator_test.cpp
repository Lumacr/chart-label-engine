
#include "chart_label_engine/application/placement/candidate_generator.hpp"

#include "chart_label_engine/domain/chart/label_target.hpp"
#include "chart_label_engine/domain/geometry/point2d.hpp"
#include "chart_label_engine/domain/label/label.hpp"
#include "chart_label_engine/domain/placement/placement.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
#include <vector>

namespace chart_label_engine::application::placement {
namespace {

using domain::chart::LabelTarget;
using domain::geometry::Point2D;
using domain::label::Label;
using domain::placement::Placement;

LabelTarget make_target() {
    return LabelTarget{
        Label{"Revenue", 80.0, 20.0},
        Point2D{100.0, 100.0}
    };
}

TEST(CandidateGeneratorTest, GeneratesEightCandidates) {
    const CandidateGenerator generator;
    const auto candidates = generator.generate(make_target());

    EXPECT_EQ(candidates.size(), 8);
}

TEST(CandidateGeneratorTest, GeneratesCandidatesInDeterministicOrder) {
    const CandidateGenerator generator;
    const auto candidates = generator.generate(make_target());

    const std::vector<Point2D> expected_positions{
        Point2D{60.0, 125.0},  // North
        Point2D{105.0, 125.0}, // North-East
        Point2D{105.0, 110.0}, // East
        Point2D{105.0, 95.0},  // South-East
        Point2D{60.0, 95.0},   // South
        Point2D{15.0, 95.0},   // South-West
        Point2D{15.0, 110.0},  // West
        Point2D{15.0, 125.0}   // North-West
    };

    ASSERT_EQ(candidates.size(), expected_positions.size());

    for (std::size_t index = 0; index < candidates.size(); ++index) {
        EXPECT_EQ(
            candidates[index].top_left(),
            expected_positions[index]
        );
    }
}

TEST(CandidateGeneratorTest, PreservesTargetLabelInEveryCandidate) {
    const auto target = make_target();
    const CandidateGenerator generator;

    const auto candidates = generator.generate(target);

    for (const auto& candidate : candidates) {
        EXPECT_EQ(candidate.label(), target.label());
    }
}

TEST(CandidateGeneratorTest, UsesDefaultGapOfFiveUnits) {
    const CandidateGenerator generator;
    const auto candidates = generator.generate(make_target());

    ASSERT_EQ(candidates.size(), 8);

    EXPECT_EQ(candidates[0].top_left(), Point2D(60.0, 125.0));
    EXPECT_EQ(candidates[1].top_left(), Point2D(105.0, 125.0));
}

TEST(CandidateGeneratorTest, UsesCustomGap) {
    const CandidateGenerator generator{10.0};
    const auto candidates = generator.generate(make_target());

    ASSERT_EQ(candidates.size(), 8);

    EXPECT_EQ(candidates[0].top_left(), Point2D(60.0, 130.0));
    EXPECT_EQ(candidates[1].top_left(), Point2D(110.0, 130.0));
    EXPECT_EQ(candidates[2].top_left(), Point2D(110.0, 110.0));
    EXPECT_EQ(candidates[3].top_left(), Point2D(110.0, 90.0));
}

TEST(CandidateGeneratorTest, SupportsZeroGap) {
    const CandidateGenerator generator{0.0};
    const auto candidates = generator.generate(make_target());

    ASSERT_EQ(candidates.size(), 8);

    EXPECT_EQ(candidates[0].top_left(), Point2D(60.0, 120.0));
    EXPECT_EQ(candidates[1].top_left(), Point2D(100.0, 120.0));
    EXPECT_EQ(candidates[2].top_left(), Point2D(100.0, 110.0));
    EXPECT_EQ(candidates[3].top_left(), Point2D(100.0, 100.0));
}

TEST(CandidateGeneratorTest, SupportsNegativeAnchorCoordinates) {
    const LabelTarget target{
        Label{"Temperature", 40.0, 10.0},
        Point2D{-50.0, -30.0}
    };

    const CandidateGenerator generator;
    const auto candidates = generator.generate(target);

    ASSERT_EQ(candidates.size(), 8);

    EXPECT_EQ(candidates[0].top_left(), Point2D(-70.0, -15.0));
    EXPECT_EQ(candidates[2].top_left(), Point2D(-45.0, -25.0));
}

TEST(CandidateGeneratorTest, SupportsDifferentLabelDimensions) {
    const LabelTarget target{
        Label{"Small", 20.0, 10.0},
        Point2D{50.0, 50.0}
    };

    const CandidateGenerator generator;
    const auto candidates = generator.generate(target);

    ASSERT_EQ(candidates.size(), 8);

    EXPECT_EQ(candidates[0].top_left(), Point2D(40.0, 65.0));
    EXPECT_EQ(candidates[2].top_left(), Point2D(55.0, 55.0));
    EXPECT_EQ(candidates[4].top_left(), Point2D(40.0, 45.0));
}

TEST(CandidateGeneratorTest, DoesNotFilterCandidatesOutsidePlotArea) {
    const LabelTarget target{
        Label{"Outside", 80.0, 20.0},
        Point2D{0.0, 0.0}
    };

    const CandidateGenerator generator;
    const auto candidates = generator.generate(target);

    ASSERT_EQ(candidates.size(), 8);

    EXPECT_LT(candidates[5].top_left().x(), 0.0);
    EXPECT_LT(candidates[5].top_left().y(), 0.0);
}

TEST(CandidateGeneratorTest, RejectsNegativeGap) {
    EXPECT_THROW(
        (CandidateGenerator{-1.0}),
        std::invalid_argument
    );
}

TEST(CandidateGeneratorTest, RejectsNaNGap) {
    EXPECT_THROW(
        (CandidateGenerator{std::numeric_limits<double>::quiet_NaN()}),
        std::invalid_argument
    );
}

TEST(CandidateGeneratorTest, RejectsInfiniteGap) {
    EXPECT_THROW(
        (CandidateGenerator{std::numeric_limits<double>::infinity()}),
        std::invalid_argument
    );
}

TEST(CandidateGeneratorTest, RejectsCandidateCoordinateOverflow) {
    const LabelTarget target{
        Label{
            "Overflow",
            std::numeric_limits<double>::max(),
            1.0
        },
        Point2D{
            std::numeric_limits<double>::max(),
            0.0
        }
    };

    const CandidateGenerator generator;

    EXPECT_THROW(
        static_cast<void>(generator.generate(target)),
        std::invalid_argument
    );
}

}
}
