#ifndef COLORPICKERWIDGET_HPP
#define COLORPICKERWIDGET_HPP

#include <TGUI/TGUI.hpp>

class ColorPickerWidget : public tgui::Group {
public:
    typedef std::shared_ptr<ColorPickerWidget> Ptr;
    typedef std::shared_ptr<const ColorPickerWidget> ConstPtr;

    ColorPickerWidget();

    static ColorPickerWidget::Ptr create(std::string labelName, std::vector<sf::Color>);
    static ColorPickerWidget::Ptr copy(ColorPickerWidget::ConstPtr other);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    // Color change signal - optional parameter: selected color
    tgui::SignalUInt onColorChange = {"ColorChanged"};

protected:
    tgui::Signal& getSignal(std::string signalName) override;
    tgui::Widget::Ptr clone() const override;

private:
    void selectColor(const sf::Color&);
};

#endif // COLORPICKERWIDGET_HPP
