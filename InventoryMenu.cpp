#include "InventoryMenu.hpp"

InventoryMenu::InventoryMenu() {
}

InventoryMenu::InventoryMenu(const SDL_Rect& area, const std::string& texture) {
    this->area = area;
    this->texture = texture;
    opened = false;
}

InventoryMenu::~InventoryMenu() {
}

void InventoryMenu::render() const {
    Graphics::getInstance().renderUIElement(&area, texture);
}
