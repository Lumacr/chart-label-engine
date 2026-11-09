
#include "chart_label_engine/domain/label/label.hpp"

#include <cmath>
#include <stdexcept>
#include <utility>

namespace chart_label_engine::domain::label {

    Label::Label(std::string text, const double width, const double height)
        : text_(std::move(text)), width_(width), height_(height) {

        if (text_.empty()) {
            throw std::invalid_argument("Label text must not be empty.");
        }

        if (!std::isfinite(width_) || !std::isfinite(height_)) {
            throw std::invalid_argument("Label dimensions must be finite.");
        }

        if (width_ <= 0.0 || height_ <= 0.0) {
            throw std::invalid_argument("Label dimensions must be positive.");
        }
    }

    const std::string& Label::text() const noexcept {
        return text_;
    }

    double Label::width() const noexcept {
        return width_;
    }

    double Label::height() const noexcept {
        return height_;
    }

}
