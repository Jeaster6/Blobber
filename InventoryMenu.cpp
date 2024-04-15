#include "InventoryMenu.hpp"

InventoryMenu::InventoryMenu() {
    mode = Mode::Party;
}

InventoryMenu::InventoryMenu(const SDL_Rect& area, const std::string& texture) {
    this->area = Graphics::getInstance().scaleTargetArea(area);
    this->texture = texture;
    opened = false;
    mode = Mode::Party;
}

InventoryMenu::~InventoryMenu() {
}

void InventoryMenu::render(const std::vector<std::string>& partyInventory, const std::vector<std::string>& objectInventory) {
    buttons.clear();
    Graphics::getInstance().renderUIElement(&area, texture);

    for (int i = 0; i < partyInventory.size(); i++) {
        Graphics::getInstance().renderTextMessage(100, 100 * (i + 1), partyInventory[i], 30, 255, 255, 255);
        if (mode == Mode::Looting) {
            buttons.push_back(Button({ 900, 100 * (i + 1), 50, 50 }, "Yes.png", ""));
        }
    }

    if (mode == Mode::Looting) {
        for (int i = 0; i < objectInventory.size(); i++) {
            buttons.push_back(Button({ 1900, 100 * (i + 1), 50, 50 }, "Yes.png", ""));
            Graphics::getInstance().renderTextMessage(1100, 100 * (i + 1), objectInventory[i], 30, 255, 255, 255);
        }
    }

    for (Button button : buttons) {
        button.render();
    }
}

void InventoryMenu::open(Mode mode) {
    opened = true;
    this->mode = mode;
}

Mode InventoryMenu::getMode() const {
    return mode;
}

bool InventoryMenu::clickButton(int i, int mouseX, int mouseY, int buttonDownX, int buttonDownY) const {
    return buttons[i].click(mouseX, mouseY, buttonDownX, buttonDownY);
}
