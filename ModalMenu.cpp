#include "ModalMenu.hpp"

ModalMenu::ModalMenu() {
}

ModalMenu::ModalMenu(const SDL_Rect& area, const std::string& texture) {
    this->area = area;
    this->texture = texture;
    opened = false;
    yesButton = Button({ area.x + 30, area.y + area.h - 80, 50, 50 }, "Yes.png", "");
    noButton = Button({ area.x + area.w - 80, area.y + area.h - 80, 50, 50 }, "No.png", "");
}

ModalMenu::~ModalMenu() {
}


void ModalMenu::render() const {
    Graphics::getInstance().renderUIElement(&area, texture);
    yesButton.render();
    noButton.render();
}

bool ModalMenu::clickYes(int mouseX, int mouseY, int buttonDownX, int buttonDownY) const {
    return yesButton.click(mouseX, mouseY, buttonDownX, buttonDownY);
}

bool ModalMenu::clickNo(int mouseX, int mouseY, int buttonDownX, int buttonDownY) const {
    return noButton.click(mouseX, mouseY, buttonDownX, buttonDownY);
}
