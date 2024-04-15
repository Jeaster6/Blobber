#include "Button.hpp"

Button::Button() {
    area = { 0, 0, 0, 0 };
    texture = "";
    label = "";
}

Button::Button(const SDL_Rect& area, const std::string& texture, const std::string& label) {
    this->area = Graphics::getInstance().scaleTargetArea(area);
    this->texture = texture;
    this->label = label;
}

Button::~Button() {
}

bool Button::click (int mouseX, int mouseY, int buttonDownX, int buttonDownY) const {
    if (mouseX >= area.x && mouseX <= area.w + area.x && mouseY >= area.y && mouseY <= area.h + area.y) {
        if (buttonDownX >= area.x && buttonDownX <= area.w + area.x && buttonDownY >= area.y && buttonDownY <= area.h + area.y) {
            return true;
        }
    }
    return false;
}

void Button::render() const {
    Graphics::getInstance().renderUIElement(&area, texture);
    Graphics::getInstance().renderTextMessage(area.x, area.y, label, 25, 255, 255, 0);
}
