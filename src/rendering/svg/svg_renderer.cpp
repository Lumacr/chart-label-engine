
#include "chart_label_engine/rendering/svg/svg_renderer.hpp"

#include "chart_label_engine/application/placement/collision_detector.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <locale>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace chart_label_engine::rendering::svg {
    namespace {
        constexpr double VIEWPORT_MARGIN = 10.0;
        constexpr double ANCHOR_RADIUS = 2.5;

        struct ViewportBounds {
            double min_x;
            double min_y;
            double max_x;
            double max_y;

            void include_x(double x) {
                min_x = std::min(min_x, x);
                max_x = std::max(max_x, x);
            }

            void include_y(double y) {
                min_y = std::min(min_y, y);
                max_y = std::max(max_y, y);
            }

            void include_point(
                const domain::geometry::Point2D& point
            ) {
                include_x(point.x());
                include_y(-point.y());
            }

            void include_rectangle(
                const domain::geometry::Rectangle& rectangle
            ) {
                include_x(rectangle.left());
                include_x(rectangle.right());

                include_y(-rectangle.top());
                include_y(-rectangle.bottom());
            }
        };

        [[nodiscard]] std::string format_number(double value) {
            std::ostringstream stream;
            stream.imbue(std::locale::classic());
            stream << std::setprecision(
                std::numeric_limits<double>::max_digits10
            ) << value;

            return stream.str();
        }

        void validate_view_box(
            double x,
            double y,
            double width,
            double height
        ) {
            if (
                !std::isfinite(x)
                || !std::isfinite(y)
                || !std::isfinite(width)
                || !std::isfinite(height)
                || width <= 0.0
                || height <= 0.0
            ) {
                throw std::overflow_error(
                    "The SVG viewport cannot be represented with finite dimensions."
                );
            }
        }

        [[nodiscard]] bool is_valid_xml_text(
            std::string_view text
        ) {
            std::size_t index = 0;

            while (index < text.size()) {
                const auto first_byte =
                    static_cast<unsigned char>(text[index]);

                std::uint32_t code_point = 0;
                std::size_t sequence_length = 0;

                if (first_byte <= 0x7F) {
                    code_point = first_byte;
                    sequence_length = 1;
                } else if (
                    first_byte >= 0xC2
                    && first_byte <= 0xDF
                ) {
                    code_point = first_byte & 0x1F;
                    sequence_length = 2;
                } else if (
                    first_byte >= 0xE0
                    && first_byte <= 0xEF
                ) {
                    code_point = first_byte & 0x0F;
                    sequence_length = 3;
                } else if (
                    first_byte >= 0xF0
                    && first_byte <= 0xF4
                ) {
                    code_point = first_byte & 0x07;
                    sequence_length = 4;
                } else {
                    return false;
                }

                if (sequence_length > text.size() - index) {
                    return false;
                }

                for (
                    std::size_t offset = 1;
                    offset < sequence_length;
                    ++offset
                ) {
                    const auto continuation_byte =
                        static_cast<unsigned char>(
                            text[index + offset]
                        );

                    if ((continuation_byte & 0xC0) != 0x80) {
                        return false;
                    }

                    code_point =
                        (code_point << 6)
                        | (continuation_byte & 0x3F);
                }

                if (
                    (sequence_length == 2 && code_point < 0x80)
                    || (sequence_length == 3 && code_point < 0x800)
                    || (sequence_length == 4 && code_point < 0x10000)
                ) {
                    return false;
                }

                if (code_point > 0x10FFFF) {
                    return false;
                }

                if (
                    code_point >= 0xD800
                    && code_point <= 0xDFFF
                ) {
                    return false;
                }

                const bool valid_xml_character =
                    code_point == 0x09
                    || code_point == 0x0A
                    || code_point == 0x0D
                    || (code_point >= 0x20 && code_point <= 0xD7FF)
                    || (code_point >= 0xE000 && code_point <= 0xFFFD)
                    || (code_point >= 0x10000 && code_point <= 0x10FFFF);

                if (!valid_xml_character) {
                    return false;
                }

                index += sequence_length;
            }

            return true;
        }

        void validate_xml_text(std::string_view text) {
            if (!is_valid_xml_text(text)) {
                throw std::invalid_argument(
                    "Label text contains invalid UTF-8 or XML 1.0 characters"
                );
            }
        }

        [[nodiscard]] std::string escape_xml(
            const std::string& text
        ) {
            std::string escaped;
            escaped.reserve(text.size());

            for (const char character : text) {
                switch (character) {
                    case '\r':
                        escaped += "&#13;";
                        break;
                    case '&':
                        escaped += "&amp;";
                        break;
                    case '<':
                        escaped += "&lt;";
                        break;
                    case '>':
                        escaped += "&gt;";
                        break;
                    case '"':
                        escaped += "&quot;";
                        break;
                    case '\'':
                        escaped += "&apos;";
                        break;
                    default:
                        escaped += character;
                        break;
                }
            }

            return escaped;
        }

        [[nodiscard]] bool extends_beyond_plot_area(
            const domain::geometry::Rectangle& bounds,
            const domain::geometry::Rectangle& plot_area
        ) {
            return bounds.left() < plot_area.left()
                   || bounds.right() > plot_area.right()
                   || bounds.top() > plot_area.top()
                   || bounds.bottom() < plot_area.bottom();
        }

        void validate_inputs(
            const domain::chart::Chart& chart,
            const std::vector<domain::placement::Placement>& placements
        ) {
            const auto& targets = chart.label_targets();

            if (placements.size() != targets.size()) {
                throw std::invalid_argument(
                    "The number of placements must match the number of label targets."
                );
            }

            for (std::size_t i = 0; i < placements.size(); ++i) {
                if (placements[i].label() != targets[i].label()) {
                    throw std::invalid_argument(
                        "Each placement must contain the label of its corresponding target."
                    );
                }
            }
        }
    }

    std::string SvgRenderer::render(
        const domain::chart::Chart& chart,
        const std::vector<domain::placement::Placement>& placements
    ) const {
        validate_inputs(chart, placements);

        for (const auto& placement : placements) {
            validate_xml_text(placement.label().text());
        }

        const auto& plot_area = chart.plot_area();
        const auto& targets = chart.label_targets();

        ViewportBounds viewport{
            plot_area.left(),
            -plot_area.top(),
            plot_area.right(),
            -plot_area.bottom()
        };

        for (std::size_t i = 0; i < placements.size(); ++i) {
            viewport.include_rectangle(placements[i].bounds());
            viewport.include_point(targets[i].anchor());
        }

        const double view_box_x =
            viewport.min_x - VIEWPORT_MARGIN;

        const double view_box_y =
            viewport.min_y - VIEWPORT_MARGIN;

        const double view_box_width =
            viewport.max_x - viewport.min_x + 2.0 * VIEWPORT_MARGIN;

        const double view_box_height =
            viewport.max_y - viewport.min_y + 2.0 * VIEWPORT_MARGIN;

        validate_view_box(
            view_box_x,
            view_box_y,
            view_box_width,
            view_box_height
        );

        application::placement::CollisionDetector collision_detector;
        const auto collisions = collision_detector.detect(placements);

        std::vector<bool> colliding(placements.size(), false);

        for (const auto& collision : collisions) {
            colliding[collision.first_index] = true;
            colliding[collision.second_index] = true;
        }

        std::ostringstream svg;
        svg.imbue(std::locale::classic());

        svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" "
            << "viewBox=\""
            << format_number(view_box_x) << " "
            << format_number(view_box_y) << " "
            << format_number(view_box_width) << " "
            << format_number(view_box_height)
            << "\">\n";

        // Render the chart plot area.
        svg << "  <rect class=\"plot-area\" "
            << "x=\"" << format_number(plot_area.left()) << "\" "
            << "y=\"" << format_number(-plot_area.top()) << "\" "
            << "width=\"" << format_number(plot_area.width()) << "\" "
            << "height=\"" << format_number(plot_area.height()) << "\" "
            << "fill=\"#f8fafc\" "
            << "stroke=\"#334155\" "
            << "stroke-width=\"0.5\"/>\n";

        // Render connections between anchors and label centers.
        for (std::size_t i = 0; i < placements.size(); ++i) {
            const auto& anchor = targets[i].anchor();
            const auto center = placements[i].bounds().center();

            svg << "  <line class=\"anchor-connection\" "
                << "x1=\"" << format_number(anchor.x()) << "\" "
                << "y1=\"" << format_number(-anchor.y()) << "\" "
                << "x2=\"" << format_number(center.x()) << "\" "
                << "y2=\"" << format_number(-center.y()) << "\" "
                << "stroke=\"#94a3b8\" "
                << "stroke-width=\"0.3\"/>\n";
        }

        // Render anchor points.
        for (std::size_t i = 0; i < placements.size(); ++i) {
            const auto& anchor = targets[i].anchor();

            svg << "  <circle class=\"anchor\" "
                << "cx=\"" << format_number(anchor.x()) << "\" "
                << "cy=\"" << format_number(-anchor.y()) << "\" "
                << "r=\"" << format_number(ANCHOR_RADIUS) << "\" "
                << "fill=\"#2563eb\"/>\n";
        }

        // Render label placements.
        for (std::size_t i = 0; i < placements.size(); ++i) {
            const auto bounds = placements[i].bounds();

            const bool outside_plot = extends_beyond_plot_area(
                bounds,
                plot_area
            );

            const std::string fill =
                colliding[i] ? "#fee2e2" : "#dbeafe";

            const std::string stroke =
                outside_plot
                    ? "#ea580c"
                    : (colliding[i] ? "#dc2626" : "#2563eb");

            const auto center = bounds.center();

            svg << "  <g class=\"label-placement\">\n";

            svg << "    <rect class=\"label-bounds\" "
                << "x=\"" << format_number(bounds.left()) << "\" "
                << "y=\"" << format_number(-bounds.top()) << "\" "
                << "width=\"" << format_number(bounds.width()) << "\" "
                << "height=\"" << format_number(bounds.height()) << "\" "
                << "fill=\"" << fill << "\" "
                << "stroke=\"" << stroke << "\" "
                << "stroke-width=\"0.5\"/>\n";

            svg << "    <text class=\"label-text\" "
                << "x=\"" << format_number(center.x()) << "\" "
                << "y=\"" << format_number(-center.y()) << "\" "
                << "text-anchor=\"middle\" "
                << "dominant-baseline=\"middle\" "
                << "font-family=\"sans-serif\" "
                << "font-size=\"3\">"
                << escape_xml(placements[i].label().text())
                << "</text>\n";

            svg << "  </g>\n";
        }

        svg << "</svg>\n";

        return svg.str();
    }
}
