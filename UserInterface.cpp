#include "UserInterface.hpp"

UserInterface::UserInterface() {
    exitButton = Button({ 2510, 0, 50, 50 }, "Button.png", "");
    modal = ModalMenu({ 720, 500, 480, 200 }, "Modal.png");
    inventory = InventoryMenu({ 0, 0, 2560, 1440 }, "Inventory.png");
    forward = Button({ 2440, 1320, 50, 50 }, "Button.png", "");
    backward = Button({ 2440, 1380, 50, 50 }, "Button.png", "");
    left = Button({ 2380, 1380, 50, 50 }, "Button.png", "");
    right = Button({ 2500, 1380, 50, 50 }, "Button.png", "");
}

UserInterface::~UserInterface() {
}

void UserInterface::render() {
    Graphics::getInstance().clearUI();
    exitButton.render();
    forward.render();
    backward.render();
    left.render();
    right.render();

    if (modal.isOpen()) {
        modal.render();
    }
    if (inventory.isOpen()) {
        inventory.render();
    }
}

int UserInterface::processMouseInput(const SDL_Event& mouseEvent, GameState& game) {
    int mouseX = 0;
    int mouseY = 0;
    int buttonDownX = 0;
    int buttonDownY = 0;
    bool mouseReleased = false;
    SDL_Event currentEvent;

    currentEvent.button.button = mouseEvent.button.button;
    SDL_GetMouseState(&buttonDownX, &buttonDownY);

    while (!mouseReleased) {
        SDL_PollEvent(&currentEvent);
        if (currentEvent.button.button == mouseEvent.button.button && currentEvent.type == SDL_MOUSEBUTTONUP) {
            mouseReleased = true;
        }
        SDL_GetMouseState(&mouseX, &mouseY);
    }

    if (modal.isOpen()) {
        if (modal.clickYes(mouseX, mouseY, buttonDownX, buttonDownY)) {
            modal.close();
            return -1;
        }

        if (modal.clickNo(mouseX, mouseY, buttonDownX, buttonDownY)) {
            modal.close();
            return 0;
        }
    }

    else {
        if (exitButton.click(mouseX, mouseY, buttonDownX, buttonDownY)) {
            modal.open();
            return 1;
        }

        if (forward.click(mouseX, mouseY, buttonDownX, buttonDownY)) {
            game.movePlayerForward();
        }

        if (backward.click(mouseX, mouseY, buttonDownX, buttonDownY)) {
            game.movePlayerBackward();
        }

        if (left.click(mouseX, mouseY, buttonDownX, buttonDownY)) {
            game.movePlayerLeft();
        }

        if (right.click(mouseX, mouseY, buttonDownX, buttonDownY)) {
            game.movePlayerRight();
        }
    }

    return 0;
}

int UserInterface::openModalWindow() {
    modal.open();
    return 1;
}

int UserInterface::openInventoryWindow() {
    inventory.open();
    return 2;
}

int UserInterface::closeAllWindows() {
    modal.close();
    inventory.close();
    return 0;
}
