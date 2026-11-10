#include "chart_label_engine/rendering/svg/svg_renderer.hpp"

#include "chart_label_engine/domain/geometry/point2d.hpp"
#include "chart_label_engine/domain/geometry/rectangle.hpp"
#include "chart_label_engine/domain/label/label.hpp"
#include "chart_label_engine/domain/placement/placement.hpp"

#include <gtest/gtest.h>

#include <limits>
#include <stdexcept>
#include <string>
#include <vector>
#include <utility>

namespace chart_label_engine::rendering::svg {
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

Chart make_chart(std::vector<LabelTarget> label_targets) {
    return Chart{
        make_plot_area(),
        std::move(label_targets)
    };
}

TEST(SvgRendererTest, RendersPlotAreaForChartWithoutTargets) {
    const auto chart = make_chart({});
    const std::vector<Placement> placements;

    const SvgRenderer renderer;

    const auto svg = renderer.render(chart, placements);

    EXPECT_NE(svg.find("<svg "), std::string::npos);
    EXPECT_NE(svg.find("viewBox=\"-10 -110 120 120\""), std::string::npos);
    EXPECT_NE(svg.find("class=\"plot-area\""), std::string::npos);
    EXPECT_NE(svg.find("</svg>"), std::string::npos);

    EXPECT_EQ(svg.find("class=\"anchor\""), std::string::npos);
    EXPECT_EQ(svg.find("class=\"label-bounds\""), std::string::npos);
}

TEST(SvgRendererTest, RendersPlacementWithCorrectCoordinateTransformation) {
    const auto label = make_label("Revenue", 20.0, 10.0);

    const auto chart = make_chart({
        LabelTarget{label, Point2D{20.0, 30.0}}
    });

    const std::vector<Placement> placements{
        Placement{label, Point2D{25.0, 55.0}}
    };

    const SvgRenderer renderer;

    const auto svg = renderer.render(chart, placements);

    EXPECT_NE(
        svg.find("x=\"0\" y=\"-100\" width=\"100\" height=\"100\""),
        std::string::npos
    );

    EXPECT_NE(
        svg.find("x1=\"20\" y1=\"-30\" x2=\"35\" y2=\"-50\""),
        std::string::npos
    );

    EXPECT_NE(
        svg.find("cx=\"20\" cy=\"-30\""),
        std::string::npos
    );

    EXPECT_NE(
        svg.find("x=\"25\" y=\"-55\" width=\"20\" height=\"10\""),
        std::string::npos
    );

    EXPECT_NE(
        svg.find("x=\"35\" y=\"-50\""),
        std::string::npos
    );

    EXPECT_NE(svg.find(">Revenue</text>"), std::string::npos);
}

TEST(SvgRendererTest, EscapesXmlSpecialCharactersInLabelText) {
    const auto label = make_label("A&B <C> \"D\" 'E'", 80.0, 10.0);

    const auto chart = make_chart({
        LabelTarget{label, Point2D{50.0, 50.0}}
    });

    const std::vector<Placement> placements{
        Placement{label, Point2D{10.0, 60.0}}
    };

    const SvgRenderer renderer;

    const auto svg = renderer.render(chart, placements);

    EXPECT_NE(
        svg.find("A&amp;B &lt;C&gt; &quot;D&quot; &apos;E&apos;"),
        std::string::npos
    );

    EXPECT_EQ(svg.find("A&B <C>"), std::string::npos);
}

TEST(SvgRendererTest, HighlightsCollidingPlacements) {
    const auto label = make_label();

    const auto chart = make_chart({
        LabelTarget{label, Point2D{20.0, 20.0}},
        LabelTarget{label, Point2D{80.0, 80.0}}
    });

    const std::vector<Placement> placements{
        Placement{label, Point2D{10.0, 30.0}},
        Placement{label, Point2D{10.0, 30.0}}
    };

    const SvgRenderer renderer;

    const auto svg = renderer.render(chart, placements);

    const std::string collision_fill = "fill=\"#fee2e2\"";
    const std::string collision_stroke = "stroke=\"#dc2626\"";

    std::size_t fill_count = 0;
    std::size_t position = 0;

    while ((position = svg.find(collision_fill, position)) != std::string::npos) {
        ++fill_count;
        position += collision_fill.size();
    }

    EXPECT_EQ(fill_count, 2);
    EXPECT_NE(svg.find(collision_stroke), std::string::npos);
}

TEST(SvgRendererTest, HighlightsPlacementsOutsidePlotArea) {
    const auto label = make_label();

    const auto chart = make_chart({
        LabelTarget{label, Point2D{20.0, 50.0}}
    });

    const std::vector<Placement> placements{
        Placement{label, Point2D{-5.0, 60.0}}
    };

    const SvgRenderer renderer;

    const auto svg = renderer.render(chart, placements);

    EXPECT_NE(svg.find("stroke=\"#ea580c\""), std::string::npos);
    EXPECT_NE(svg.find("fill=\"#dbeafe\""), std::string::npos);
}

TEST(SvgRendererTest, RejectsDifferentNumberOfPlacementsAndTargets) {
    const auto label = make_label();

    const auto chart = make_chart({
        LabelTarget{label, Point2D{50.0, 50.0}}
    });

    const std::vector<Placement> placements;

    const SvgRenderer renderer;

    EXPECT_THROW(
    static_cast<void>(renderer.render(chart, placements)),
    std::invalid_argument
);
}

TEST(SvgRendererTest, RejectsPlacementWithMismatchedLabel) {
    const auto target_label = make_label("Target");
    const auto placement_label = make_label("Different");

    const auto chart = make_chart({
        LabelTarget{target_label, Point2D{50.0, 50.0}}
    });

    const std::vector<Placement> placements{
        Placement{placement_label, Point2D{10.0, 60.0}}
    };

    const SvgRenderer renderer;

    EXPECT_THROW(
    static_cast<void>(renderer.render(chart, placements)),
    std::invalid_argument
);
}

TEST(SvgRendererTest, IncludesPlacementsAndAnchorsInViewport) {
    const auto label = make_label("Label", 10.0, 10.0);

    const auto chart = make_chart({
        LabelTarget{label, Point2D{150.0, 150.0}}
    });

    const std::vector<Placement> placements{
        Placement{label, Point2D{120.0, 180.0}}
    };

    const SvgRenderer renderer;

    const auto svg = renderer.render(chart, placements);

    EXPECT_NE(
    svg.find("viewBox=\"-10 -190 170 200\""),
    std::string::npos
);
}

TEST(SvgRendererTest, ProducesDeterministicOutput) {
    const auto label = make_label("Deterministic", 20.0, 10.0);

    const auto chart = make_chart({
        LabelTarget{label, Point2D{50.0, 50.0}}
    });

    const std::vector<Placement> placements{
        Placement{label, Point2D{40.0, 60.0}}
    };

    const SvgRenderer renderer;

    const auto first_result = renderer.render(chart, placements);
    const auto second_result = renderer.render(chart, placements);

    EXPECT_EQ(first_result, second_result);
}

TEST(SvgRendererTest, DoesNotModifyChartOrPlacements) {
    const auto label = make_label();

    const auto chart = make_chart({
        LabelTarget{label, Point2D{50.0, 50.0}}
    });

    const std::vector<Placement> placements{
        Placement{label, Point2D{40.0, 60.0}}
    };

    const auto original_chart = chart;
    const auto original_placements = placements;

    const SvgRenderer renderer;

    static_cast<void>(renderer.render(chart, placements));

    EXPECT_EQ(chart, original_chart);
    EXPECT_EQ(placements, original_placements);
}

TEST(SvgRendererTest, RejectsViewportWithNonFiniteDimensions) {
    const auto label = make_label();

    const double maximum = std::numeric_limits<double>::max();

    const Chart chart{
        Rectangle{
            Point2D{-maximum, 100.0},
            10.0,
            100.0
        },
        {
            LabelTarget{label, Point2D{maximum, 50.0}}
        }
    };

    const std::vector<Placement> placements{
        Placement{label, Point2D{0.0, 60.0}}
    };

    const SvgRenderer renderer;

    EXPECT_THROW(
    static_cast<void>(renderer.render(chart, placements)),
    std::overflow_error
);
}

} // namespace
} // namespace chart_label_engine::rendering::svg
