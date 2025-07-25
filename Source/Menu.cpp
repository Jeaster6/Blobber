#include "Menu.hpp"

Menu::Menu() {
    area = { 0, 0, 0, 0 };
    texture = "";
    opened = false;
}

Menu::Menu(const SDL_Rect& area, const std::string& texture) {
    this->area = area;
    this->texture = texture;
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

bool Menu::isOpen() const {
    return opened;
}
