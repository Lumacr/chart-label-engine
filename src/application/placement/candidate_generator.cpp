
#include "chart_label_engine/application/placement/candidate_generator.hpp"

#include "chart_label_engine/domain/geometry/point2d.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace chart_label_engine::application::placement {

CandidateGenerator::CandidateGenerator(double gap)
    : gap_(gap) {

    if (!std::isfinite(gap_)) {
        throw std::invalid_argument("Candidate gap must be finite.");
    }

    if (gap_ < 0.0) {
        throw std::invalid_argument("Candidate gap cannot be negative.");
    }
}

std::vector<domain::placement::Placement> CandidateGenerator::generate(
    const domain::chart::LabelTarget& target
) const {

    const auto& label = target.label();
    const auto anchor = target.anchor();

    const double x = anchor.x();
    const double y = anchor.y();
    const double width = label.width();
    const double height = label.height();

    const double half_width = width / 2.0;
    const double half_height = height / 2.0;

    std::vector<domain::placement::Placement> candidates;
    candidates.reserve(8);

    // North
    candidates.emplace_back(
        label,
        domain::geometry::Point2D{
            x - half_width,
            y + gap_ + height
        }
    );

    // North-East
    candidates.emplace_back(
        label,
        domain::geometry::Point2D{
            x + gap_,
            y + gap_ + height
        }
    );

    // East
    candidates.emplace_back(
        label,
        domain::geometry::Point2D{
            x + gap_,
            y + half_height
        }
    );

    // South-East
    candidates.emplace_back(
        label,
        domain::geometry::Point2D{
            x + gap_,
            y - gap_
        }
    );

    // South
    candidates.emplace_back(
        label,
        domain::geometry::Point2D{
            x - half_width,
            y - gap_
        }
    );

    // South-West
    candidates.emplace_back(
        label,
        domain::geometry::Point2D{
            x - gap_ - width,
            y - gap_
        }
    );

    // West
    candidates.emplace_back(
        label,
        domain::geometry::Point2D{
            x - gap_ - width,
            y + half_height
        }
    );

    // North-West
    candidates.emplace_back(
        label,
        domain::geometry::Point2D{
            x - gap_ - width,
            y + gap_ + height
        }
    );

    return candidates;
}

}
