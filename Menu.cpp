#include "Menu.hpp"

Menu::Menu() {
    title = "";
    area = { 0, 0, 0, 0 };
    texture = "";
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
    Graphics::getInstance().renderTexture(texture, &area);
}

bool Menu::isOpen() const {
    return opened;
}
