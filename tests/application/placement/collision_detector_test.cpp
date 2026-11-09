
#include "chart_label_engine/application/placement/collision_detector.hpp"

#include "chart_label_engine/domain/geometry/point2d.hpp"
#include "chart_label_engine/domain/label/label.hpp"
#include "chart_label_engine/domain/placement/placement.hpp"

#include <gtest/gtest.h>

#include <vector>

namespace chart_label_engine::application::placement {
namespace {

domain::placement::Placement make_placement(double x, double y) {
    return domain::placement::Placement{
        domain::label::Label{"Label", 10.0, 10.0},
        domain::geometry::Point2D{x, y}
    };
}

TEST(CollisionDetectorTest, ReturnsEmptyResultForEmptyCollection) {
    const CollisionDetector detector;
    const std::vector<domain::placement::Placement> placements;

    const auto collisions = detector.detect(placements);

    EXPECT_TRUE(collisions.empty());
}

TEST(CollisionDetectorTest, ReturnsEmptyResultForSinglePlacement) {
    const CollisionDetector detector;
    const std::vector placements{
        make_placement(0.0, 10.0)
    };

    const auto collisions = detector.detect(placements);

    EXPECT_TRUE(collisions.empty());
}

TEST(CollisionDetectorTest, ReturnsEmptyResultWhenPlacementsAreSeparated) {
    const CollisionDetector detector;
    const std::vector placements{
        make_placement(0.0, 10.0),
        make_placement(20.0, 10.0)
    };

    const auto collisions = detector.detect(placements);

    EXPECT_TRUE(collisions.empty());
}

TEST(CollisionDetectorTest, DetectsOverlappingPlacements) {
    const CollisionDetector detector;
    const std::vector placements{
        make_placement(0.0, 10.0),
        make_placement(5.0, 10.0)
    };

    const auto collisions = detector.detect(placements);

    const std::vector<CollisionPair> expected{
        {0, 1}
    };

    EXPECT_EQ(collisions, expected);
}

TEST(CollisionDetectorTest, DoesNotDetectPlacementsTouchingAtAnEdge) {
    const CollisionDetector detector;
    const std::vector placements{
        make_placement(0.0, 10.0),
        make_placement(10.0, 10.0)
    };

    const auto collisions = detector.detect(placements);

    EXPECT_TRUE(collisions.empty());
}

TEST(CollisionDetectorTest, DoesNotDetectPlacementsTouchingAtACorner) {
    const CollisionDetector detector;
    const std::vector placements{
        make_placement(0.0, 10.0),
        make_placement(10.0, 0.0)
    };

    const auto collisions = detector.detect(placements);

    EXPECT_TRUE(collisions.empty());
}

TEST(CollisionDetectorTest, DetectsAllOverlappingPairs) {
    const CollisionDetector detector;
    const std::vector placements{
        make_placement(0.0, 10.0),
        make_placement(5.0, 10.0),
        make_placement(20.0, 10.0),
        make_placement(8.0, 10.0)
    };

    const auto collisions = detector.detect(placements);

    const std::vector<CollisionPair> expected{
        {0, 1},
        {0, 3},
        {1, 3}
    };

    EXPECT_EQ(collisions, expected);
}

TEST(CollisionDetectorTest, ReturnsCollisionPairsInDeterministicOrder) {
    const CollisionDetector detector;
    const std::vector placements{
        make_placement(8.0, 10.0),
        make_placement(0.0, 10.0),
        make_placement(5.0, 10.0)
    };

    const auto collisions = detector.detect(placements);

    const std::vector<CollisionPair> expected{
        {0, 1},
        {0, 2},
        {1, 2}
    };

    EXPECT_EQ(collisions, expected);
}

TEST(CollisionDetectorTest, ReportsIdenticalPlacementsAsColliding) {
    const CollisionDetector detector;
    const std::vector placements{
        make_placement(0.0, 10.0),
        make_placement(0.0, 10.0)
    };

    const auto collisions = detector.detect(placements);

    const std::vector<CollisionPair> expected{
        {0, 1}
    };

    EXPECT_EQ(collisions, expected);
}

TEST(CollisionDetectorTest, DetectsOverlapsWithNegativeCoordinates) {
    const CollisionDetector detector;
    const std::vector placements{
        make_placement(-15.0, -5.0),
        make_placement(-10.0, -5.0)
    };

    const auto collisions = detector.detect(placements);

    const std::vector<CollisionPair> expected{
        {0, 1}
    };

    EXPECT_EQ(collisions, expected);
}

} // namespace
} // namespace chart_label_engine::application::placement
