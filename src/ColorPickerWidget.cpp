#include "ColorPickerWidget.hpp"

ColorPickerWidget::ColorPickerWidget() {
    m_type = "ColorPickerWidget";
}

ColorPickerWidget::Ptr ColorPickerWidget::create(std::string labelName, std::vector<sf::Color> colors) {
    auto colorPicker = std::make_shared<ColorPickerWidget>();

    auto label = tgui::Label::create(labelName);
    colorPicker->add(label, "label");

    constexpr auto offset = 30;
    constexpr auto size = "25";
    constexpr auto hoverTint = 40;
    const auto markerColor = sf::Color(0x1e, 0x08, 0x0b);

    for (size_t i = 0; i < colors.size(); i++) {
        auto colorRadioButton = tgui::RadioButton::create();
        colorRadioButton->setPosition({std::to_string(i * offset), "label.bottom + 10"});
        colorRadioButton->setSize({size, size});

        auto radioButtonRenderer = colorRadioButton->getRenderer();
        radioButtonRenderer->setBackgroundColor(colors[i]);
        radioButtonRenderer->setBackgroundColorHover(sf::Color(
            std::min(colors[i].r + hoverTint, 255),
            std::min(colors[i].g + hoverTint, 255),
            std::min(colors[i].b + hoverTint, 255)));

        radioButtonRenderer->setBorderColor(sf::Color::Transparent);
        radioButtonRenderer->setBorderColorHover(sf::Color::Transparent);
        radioButtonRenderer->setBorderColorChecked(sf::Color::Transparent);

        radioButtonRenderer->setTextColor(markerColor);

        if (i == colors.size() - 1) {
            colorPicker->add(colorRadioButton, "colorRadioButton");

            // Resize group for relative positioning
            colorPicker->setSize({"colorRadioButton.right", "colorRadioButton.bottom"});
        } else {
            colorPicker->add(colorRadioButton);
        }

        if (i == 0) {
            colorRadioButton->setChecked(true);
        }

        colorRadioButton->connect("checked", &ColorPickerWidget::selectColor, colorPicker.get(), colors[i]);
    }

    return colorPicker;
}

ColorPickerWidget::Ptr ColorPickerWidget::copy(ColorPickerWidget::ConstPtr other) {
    if (other) {
        return std::static_pointer_cast<ColorPickerWidget>(other->clone());
    }

    return nullptr;
}

void ColorPickerWidget::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform.translate(getPosition());
    drawWidgetContainer(&target, states);
}

tgui::Signal& ColorPickerWidget::getSignal(std::string signalName) {
    if (signalName == tgui::toLower(onColorChange.getName())) {
        return onColorChange;
    } else {
        return tgui::Group::getSignal(std::move(signalName));
    }
}

tgui::Widget::Ptr ColorPickerWidget::clone() const {
    return std::make_shared<ColorPickerWidget>(*this);
}

void ColorPickerWidget::selectColor(const sf::Color& color) {
    onColorChange.emit(this, color.toInteger());
}