
#include "chart_label_engine/application/placement/greedy_placement_optimizer.hpp"

#include "chart_label_engine/application/placement/candidate_generator.hpp"
#include "chart_label_engine/application/placement/placement_scorer.hpp"
#include "chart_label_engine/domain/geometry/point2d.hpp"
#include "chart_label_engine/domain/geometry/rectangle.hpp"
#include "chart_label_engine/domain/label/label.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
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
    const std::string& text = "Label",
    double width = 10.0,
    double height = 10.0
) {
    return Label{text, width, height};
}

Rectangle make_plot_area() {
    return Rectangle{
        Point2D{0.0, 100.0},
        100.0,
        100.0
    };
}

Chart make_chart(
    std::vector<LabelTarget> label_targets
) {
    return Chart{
        make_plot_area(),
        std::move(label_targets)
    };
}

TEST(GreedyPlacementOptimizerTest, ReturnsEmptySolutionForChartWithoutTargets) {
    const auto chart = make_chart({});

    const GreedyPlacementOptimizer optimizer;

    const auto placements = optimizer.optimize(chart);

    EXPECT_TRUE(placements.empty());
}

TEST(GreedyPlacementOptimizerTest, OptimizesSingleLabelPlacement) {
    const auto label = make_label();

    const auto chart = make_chart({
        LabelTarget{label, Point2D{50.0, 50.0}}
    });

    const GreedyPlacementOptimizer optimizer;

    const auto placements = optimizer.optimize(chart);

    ASSERT_EQ(placements.size(), 1);
    EXPECT_EQ(placements[0].label(), label);

    // East is the first candidate with the minimum anchor distance.
    EXPECT_EQ(placements[0].top_left(), Point2D(45.0, 65.0));
}

TEST(GreedyPlacementOptimizerTest, EliminatesCollisionBetweenIdenticalTargets) {
    const auto label = make_label();

    const auto chart = make_chart({
        LabelTarget{label, Point2D{50.0, 50.0}},
        LabelTarget{label, Point2D{50.0, 50.0}}
    });

    const GreedyPlacementOptimizer optimizer;
    const PlacementScorer scorer;

    const auto placements = optimizer.optimize(chart);
    const auto result = scorer.score(chart, placements);

    ASSERT_EQ(placements.size(), 2);
    EXPECT_EQ(result.collision_count(), 0);
}

TEST(GreedyPlacementOptimizerTest, ProducesValidSolutionForMultipleLabels) {
    const auto first_label = make_label("First");
    const auto second_label = make_label("Second");

    const auto chart = make_chart({
        LabelTarget{first_label, Point2D{20.0, 80.0}},
        LabelTarget{second_label, Point2D{80.0, 20.0}}
    });

    const GreedyPlacementOptimizer optimizer;
    const PlacementScorer scorer;

    const auto placements = optimizer.optimize(chart);
    const auto result = scorer.score(chart, placements);

    ASSERT_EQ(placements.size(), 2);

    EXPECT_EQ(placements[0].label(), first_label);
    EXPECT_EQ(placements[1].label(), second_label);

    EXPECT_EQ(result.collision_count(), 0);
    EXPECT_DOUBLE_EQ(result.boundary_penalty(), 0.0);
}

TEST(GreedyPlacementOptimizerTest, DoesNotWorsenInitialSolution) {
    const auto label = make_label();

    const auto chart = make_chart({
        LabelTarget{label, Point2D{50.0, 50.0}},
        LabelTarget{label, Point2D{50.0, 50.0}}
    });

    const CandidateGenerator candidate_generator;
    const PlacementScorer scorer;
    const GreedyPlacementOptimizer optimizer;

    std::vector<Placement> initial_placements;

    for (const auto& target : chart.label_targets()) {
        const auto candidates = candidate_generator.generate(target);
        initial_placements.push_back(candidates.front());
    }

    const auto initial_score = scorer.score(chart, initial_placements);

    const auto optimized_placements = optimizer.optimize(chart);
    const auto optimized_score = scorer.score(chart, optimized_placements);

    EXPECT_TRUE(
        optimized_score == initial_score ||
        optimized_score.is_better_than(initial_score)
    );
}

TEST(GreedyPlacementOptimizerTest, ProducesDeterministicResults) {
    const auto first_label = make_label("First");
    const auto second_label = make_label("Second");

    const auto chart = make_chart({
        LabelTarget{first_label, Point2D{50.0, 50.0}},
        LabelTarget{second_label, Point2D{50.0, 50.0}}
    });

    const GreedyPlacementOptimizer optimizer;

    const auto first_result = optimizer.optimize(chart);
    const auto second_result = optimizer.optimize(chart);

    EXPECT_EQ(first_result, second_result);
}

TEST(GreedyPlacementOptimizerTest, DoesNotModifyChart) {
    const auto label = make_label();

    const auto chart = make_chart({
        LabelTarget{label, Point2D{50.0, 50.0}}
    });

    const auto original_chart = chart;

    const GreedyPlacementOptimizer optimizer;

    static_cast<void>(optimizer.optimize(chart));

    EXPECT_EQ(chart, original_chart);
}

TEST(GreedyPlacementOptimizerTest, UsesCustomCandidateGap) {
    const auto label = make_label();

    const auto chart = make_chart({
        LabelTarget{label, Point2D{50.0, 50.0}}
    });

    const GreedyPlacementOptimizer optimizer{10.0};

    const auto placements = optimizer.optimize(chart);

    ASSERT_EQ(placements.size(), 1);

    EXPECT_EQ(placements[0].top_left(), Point2D(45.0, 70.0));
}

TEST(GreedyPlacementOptimizerTest, RejectsNegativeCandidateGap) {
    EXPECT_THROW(
        (GreedyPlacementOptimizer{-1.0}),
        std::invalid_argument
    );
}

TEST(GreedyPlacementOptimizerTest, RejectsNaNCandidateGap) {
    EXPECT_THROW(
        (GreedyPlacementOptimizer{
            std::numeric_limits<double>::quiet_NaN()
        }),
        std::invalid_argument
    );
}

TEST(GreedyPlacementOptimizerTest, RejectsInfiniteCandidateGap) {
    EXPECT_THROW(
        (GreedyPlacementOptimizer{
            std::numeric_limits<double>::infinity()
        }),
        std::invalid_argument
    );
}

}
}
