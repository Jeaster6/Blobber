#include "Button.hpp"

Button::Button() {
    title = "";
    area = { 0, 0, 0, 0 };
    texture = "";
}

Button::Button(const std::string& title, const SDL_Rect& area, const std::string& texture) {
    this->title = title;
    this->area = area;
    this->texture = texture;
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
    Graphics::getInstance().renderUIElement(texture, &area);
}

std::string Button::getTitle() const {
    return title;
}
