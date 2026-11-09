#pragma once

#include <string>

namespace chart_label_engine::domain::label {

    class Label {
    public:
        Label(std::string text, double width, double height);

        [[nodiscard]] const std::string& text() const noexcept;
        [[nodiscard]] double width() const noexcept;
        [[nodiscard]] double height() const noexcept;

        [[nodiscard]] bool operator==(const Label& other) const noexcept = default;

    private:
        std::string text_;
        double width_;
        double height_;
    };

}
