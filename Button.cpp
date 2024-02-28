#include "Button.hpp"

Button::Button() {
    title = "";
    area = { 0, 0, 0, 0 };
    texture = "";
}

Button::~Button() {
}

void Button::click() {
    action();
}

void Button::render() {
    Graphics::getInstance().renderTexture(texture, &area);
}
