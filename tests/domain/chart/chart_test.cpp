
#include "chart_label_engine/domain/chart/chart.hpp"

#include <gtest/gtest.h>

#include <utility>
#include <vector>

namespace chart_label_engine::domain::chart {

TEST(ChartTest, ConstructsWithValidPlotAreaAndLabelTargets) {
    const geometry::Rectangle plot_area{
        geometry::Point2D{0.0, 500.0},
        800.0,
        500.0
    };

    const std::vector<LabelTarget> label_targets{
        LabelTarget{
            label::Label{"Revenue", 80.0, 20.0},
            geometry::Point2D{100.0, 200.0}
        },
        LabelTarget{
            label::Label{"Expenses", 90.0, 20.0},
            geometry::Point2D{300.0, 150.0}
        }
    };

    const Chart chart{plot_area, label_targets};

    EXPECT_EQ(chart.plot_area(), plot_area);
    EXPECT_EQ(chart.label_targets(), label_targets);
}

TEST(ChartTest, ConstructsWithEmptyLabelTargets) {
    const geometry::Rectangle plot_area{
        geometry::Point2D{0.0, 500.0},
        800.0,
        500.0
    };

    const Chart chart{plot_area, {}};

    EXPECT_TRUE(chart.label_targets().empty());
}

TEST(ChartTest, ReturnsPlotArea) {
    const geometry::Rectangle plot_area{
        geometry::Point2D{10.0, 400.0},
        600.0,
        400.0
    };

    const Chart chart{plot_area, {}};

    EXPECT_EQ(chart.plot_area(), plot_area);
}

TEST(ChartTest, ReturnsLabelTargets) {
    const std::vector<LabelTarget> label_targets{
        LabelTarget{
            label::Label{"Revenue", 80.0, 20.0},
            geometry::Point2D{100.0, 200.0}
        }
    };

    const Chart chart{
        geometry::Rectangle{
            geometry::Point2D{0.0, 500.0},
            800.0,
            500.0
        },
        label_targets
    };

    EXPECT_EQ(chart.label_targets(), label_targets);
}

TEST(ChartTest, PreservesLabelTargetOrder) {
    const Chart chart{
        geometry::Rectangle{
            geometry::Point2D{0.0, 500.0},
            800.0,
            500.0
        },
        {
            LabelTarget{
                label::Label{"First", 50.0, 20.0},
                geometry::Point2D{100.0, 200.0}
            },
            LabelTarget{
                label::Label{"Second", 60.0, 20.0},
                geometry::Point2D{200.0, 150.0}
            },
            LabelTarget{
                label::Label{"Third", 70.0, 20.0},
                geometry::Point2D{300.0, 100.0}
            }
        }
    };

    ASSERT_EQ(chart.label_targets().size(), 3);

    EXPECT_EQ(chart.label_targets()[0].label().text(), "First");
    EXPECT_EQ(chart.label_targets()[1].label().text(), "Second");
    EXPECT_EQ(chart.label_targets()[2].label().text(), "Third");
}

TEST(ChartTest, EqualChartsAreEqual) {
    const Chart first{
        geometry::Rectangle{
            geometry::Point2D{0.0, 500.0},
            800.0,
            500.0
        },
        {
            LabelTarget{
                label::Label{"Revenue", 80.0, 20.0},
                geometry::Point2D{100.0, 200.0}
            }
        }
    };

    const Chart second{
        geometry::Rectangle{
            geometry::Point2D{0.0, 500.0},
            800.0,
            500.0
        },
        {
            LabelTarget{
                label::Label{"Revenue", 80.0, 20.0},
                geometry::Point2D{100.0, 200.0}
            }
        }
    };

    EXPECT_EQ(first, second);
}

TEST(ChartTest, DifferentPlotAreasAreNotEqual) {
    const Chart first{
        geometry::Rectangle{
            geometry::Point2D{0.0, 500.0},
            800.0,
            500.0
        },
        {}
    };

    const Chart second{
        geometry::Rectangle{
            geometry::Point2D{0.0, 500.0},
            600.0,
            400.0
        },
        {}
    };

    EXPECT_NE(first, second);
}

TEST(ChartTest, DifferentLabelTargetsAreNotEqual) {
    const geometry::Rectangle plot_area{
        geometry::Point2D{0.0, 500.0},
        800.0,
        500.0
    };

    const Chart first{
        plot_area,
        {
            LabelTarget{
                label::Label{"Revenue", 80.0, 20.0},
                geometry::Point2D{100.0, 200.0}
            }
        }
    };

    const Chart second{
        plot_area,
        {
            LabelTarget{
                label::Label{"Expenses", 80.0, 20.0},
                geometry::Point2D{100.0, 200.0}
            }
        }
    };

    EXPECT_NE(first, second);
}

TEST(ChartTest, DifferentNumberOfLabelTargetsAreNotEqual) {
    const geometry::Rectangle plot_area{
        geometry::Point2D{0.0, 500.0},
        800.0,
        500.0
    };

    const Chart first{
        plot_area,
        {
            LabelTarget{
                label::Label{"Revenue", 80.0, 20.0},
                geometry::Point2D{100.0, 200.0}
            }
        }
    };

    const Chart second{
        plot_area,
        {
            LabelTarget{
                label::Label{"Revenue", 80.0, 20.0},
                geometry::Point2D{100.0, 200.0}
            },
            LabelTarget{
                label::Label{"Expenses", 90.0, 20.0},
                geometry::Point2D{300.0, 150.0}
            }
        }
    };

    EXPECT_NE(first, second);
}

TEST(ChartTest, OwnsLabelTargetsIndependentlyOfOriginalObjects) {
    std::vector<LabelTarget> original_targets{
        LabelTarget{
            label::Label{"Revenue", 80.0, 20.0},
            geometry::Point2D{100.0, 200.0}
        },
        LabelTarget{
            label::Label{"Expenses", 90.0, 20.0},
            geometry::Point2D{300.0, 150.0}
        }
    };

    const Chart chart{
        geometry::Rectangle{
            geometry::Point2D{0.0, 500.0},
            800.0,
            500.0
        },
        std::move(original_targets)
    };

    ASSERT_EQ(chart.label_targets().size(), 2);

    EXPECT_EQ(chart.label_targets()[0].label().text(), "Revenue");
    EXPECT_EQ(chart.label_targets()[1].label().text(), "Expenses");
}

}
