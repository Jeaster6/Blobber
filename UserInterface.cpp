#include "UserInterface.hpp"

UserInterface::UserInterface() {
    exitButton = Button({ 2510, 0, 50, 50 }, "Button.png");
    modal = ModalMenu({ 250, 250, 500, 500 }, "Menu.png");
    forward = Button({ 2440, 1320, 50, 50 }, "Button.png");;
    backward = Button({ 2440, 1380, 50, 50 }, "Button.png");;
    left = Button({ 2380, 1380, 50, 50 }, "Button.png");;
    right = Button({ 2500, 1380, 50, 50 }, "Button.png");;
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

    if (modal.clickYes(mouseX, mouseY, buttonDownX, buttonDownY)) {
        modal.close();
        return -1;
    }

    if (modal.clickNo(mouseX, mouseY, buttonDownX, buttonDownY)) {
        modal.close();
        return 0;
    }

    return 0;
}
