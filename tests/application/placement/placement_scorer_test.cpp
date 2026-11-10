#include "chart_label_engine/application/placement/placement_scorer.hpp"

#include "chart_label_engine/domain/geometry/point2d.hpp"
#include "chart_label_engine/domain/geometry/rectangle.hpp"
#include "chart_label_engine/domain/label/label.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace chart_label_engine::application::placement {
namespace {

using domain::chart::Chart;
using domain::chart::LabelTarget;
using domain::geometry::Point2D;
using domain::geometry::Rectangle;
using domain::label::Label;
using domain::placement::Placement;

Label make_label(
    double width = 10.0,
    double height = 10.0
) {
    return Label{"Label", width, height};
}

Rectangle make_plot_area() {
    return Rectangle{
        Point2D{0.0, 100.0},
        100.0,
        100.0
    };
}

Placement make_placement(
    double x,
    double y,
    const Label& label = make_label()
) {
    return Placement{
        label,
        Point2D{x, y}
    };
}

LabelTarget make_target(
    const Label& label,
    Point2D anchor
) {
    return LabelTarget{
        label,
        anchor
    };
}

TEST(PlacementScorerTest, ScoresEmptyPlacementSolution) {
    const Chart chart{
        make_plot_area(),
        {}
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, {});

    EXPECT_EQ(result.collision_count(), 0);
    EXPECT_DOUBLE_EQ(result.boundary_penalty(), 0.0);
    EXPECT_DOUBLE_EQ(result.anchor_distance(), 0.0);
}

TEST(PlacementScorerTest, ScoresSinglePlacementInsidePlotArea) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{15.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 80.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_EQ(result.collision_count(), 0);
    EXPECT_DOUBLE_EQ(result.boundary_penalty(), 0.0);
    EXPECT_DOUBLE_EQ(result.anchor_distance(), 0.0);
}

TEST(PlacementScorerTest, CalculatesCollisionCount) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{15.0, 75.0}),
            make_target(label, Point2D{15.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 80.0, label),
        make_placement(10.0, 80.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_EQ(result.collision_count(), 1);
}

TEST(PlacementScorerTest, DoesNotCountTouchingPlacementsAsCollisions) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{15.0, 75.0}),
            make_target(label, Point2D{25.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 80.0, label),
        make_placement(20.0, 80.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_EQ(result.collision_count(), 0);
}

TEST(PlacementScorerTest, CalculatesLeftBoundaryPenalty) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{0.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(-5.0, 80.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_DOUBLE_EQ(result.boundary_penalty(), 5.0);
}

TEST(PlacementScorerTest, CalculatesRightBoundaryPenalty) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{100.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(98.0, 80.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_DOUBLE_EQ(result.boundary_penalty(), 8.0);
}

TEST(PlacementScorerTest, CalculatesTopBoundaryPenalty) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{15.0, 100.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 105.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_DOUBLE_EQ(result.boundary_penalty(), 5.0);
}

TEST(PlacementScorerTest, CalculatesBottomBoundaryPenalty) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{15.0, 0.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 5.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_DOUBLE_EQ(result.boundary_penalty(), 5.0);
}

TEST(PlacementScorerTest, SumsPenaltiesAcrossMultipleBoundaries) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{0.0, 100.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(-5.0, 105.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_DOUBLE_EQ(result.boundary_penalty(), 10.0);
}

TEST(PlacementScorerTest, CalculatesTotalBoundaryPenaltyAcrossPlacements) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{0.0, 75.0}),
            make_target(label, Point2D{100.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(-5.0, 80.0, label),
        make_placement(98.0, 80.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_DOUBLE_EQ(result.boundary_penalty(), 13.0);
}

TEST(PlacementScorerTest, CalculatesDistanceFromPlacementCenterToAnchor) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{18.0, 79.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 80.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_DOUBLE_EQ(result.anchor_distance(), 5.0);
}

TEST(PlacementScorerTest, CalculatesTotalAnchorDistanceAcrossPlacements) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{18.0, 79.0}),
            make_target(label, Point2D{15.0, 79.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 80.0, label),
        make_placement(10.0, 80.0, label)
    };

    const PlacementScorer scorer;

    const auto result = scorer.score(chart, placements);

    EXPECT_DOUBLE_EQ(result.anchor_distance(), 9.0);
}

TEST(PlacementScorerTest, RejectsFewerPlacementsThanTargets) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{15.0, 75.0}),
            make_target(label, Point2D{25.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 80.0, label)
    };

    const PlacementScorer scorer;

    EXPECT_THROW(
        static_cast<void>(scorer.score(chart, placements)),
        std::invalid_argument
    );
}

TEST(PlacementScorerTest, RejectsMorePlacementsThanTargets) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{15.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 80.0, label),
        make_placement(30.0, 80.0, label)
    };

    const PlacementScorer scorer;

    EXPECT_THROW(
        static_cast<void>(scorer.score(chart, placements)),
        std::invalid_argument
    );
}

TEST(PlacementScorerTest, RejectsPlacementWithDifferentLabel) {
    const Label target_label{"Target", 10.0, 10.0};
    const Label placement_label{"Different", 10.0, 10.0};

    const Chart chart{
        make_plot_area(),
        {
            make_target(target_label, Point2D{15.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 80.0, placement_label)
    };

    const PlacementScorer scorer;

    EXPECT_THROW(
        static_cast<void>(scorer.score(chart, placements)),
        std::invalid_argument
    );
}

TEST(PlacementScorerTest, DoesNotModifyChartOrPlacements) {
    const auto label = make_label();

    const Chart chart{
        make_plot_area(),
        {
            make_target(label, Point2D{15.0, 75.0})
        }
    };

    const std::vector<Placement> placements{
        make_placement(10.0, 80.0, label)
    };

    const auto original_chart = chart;
    const auto original_placements = placements;

    const PlacementScorer scorer;

    static_cast<void>(scorer.score(chart, placements));

    EXPECT_EQ(chart, original_chart);
    EXPECT_EQ(placements, original_placements);
}

TEST(PlacementScorerTest, RejectsBoundaryPenaltyOverflow) {
    const double maximum = std::numeric_limits<double>::max();

    const Label label{"Large", maximum / 2.0, 1.0};

    const Chart chart{
        Rectangle{
            Point2D{-maximum, 1.0},
            maximum / 2.0,
            1.0
        },
        {
            make_target(label, Point2D{maximum / 2.0, 0.5})
        }
    };

    const std::vector<Placement> placements{
        make_placement(maximum / 2.0, 1.0, label)
    };

    const PlacementScorer scorer;

    EXPECT_THROW(
        static_cast<void>(scorer.score(chart, placements)),
        std::overflow_error
    );
}

}
}
