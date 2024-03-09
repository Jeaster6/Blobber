#include "Menu.hpp"

Menu::Menu() {
    area = { 0, 0, 0, 0 };
    texture = "";
    buttons.clear();
    opened = false;
}

Menu::Menu(const SDL_Rect& area, const std::string& texture) {
    this->area = area;
    this->texture = texture;
    buttons.clear();
    opened = false;
}

Menu::~Menu() {
}

void Menu::open() {
    opened = true;
}

void Menu::close() {
    opened = false;
}

void Menu::render() const {
    Graphics::getInstance().renderUIElement(texture, &area);
}

bool Menu::isOpen() const {
    return opened;
}
