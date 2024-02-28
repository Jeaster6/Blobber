#include "Button.hpp"

Button::Button() {
    title = "";
    area = { 0, 0, 0, 0 };
    texture = "";
}

Button::Button(const std::string& title, const SDL_Rect& area, const std::string& texture, const std::function<void()>& action) {
    this->title = title;
    this->area = area;
    this->texture = texture;
    this->action = action;
}

Button::~Button() {
}

void Button::click() {
    action();
}

void Button::render() {
    Graphics::getInstance().renderUIElement(texture, &area);
}


SDL_Rect Button::getArea() const {
    return area;
}
