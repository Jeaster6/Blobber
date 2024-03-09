#include "UserInterface.hpp"

UserInterface::UserInterface() {
    button1 = Button({ 0, 0, 50, 50 }, "Button.png");
    button2 = Button({ 2510, 0, 50, 50 }, "Button.png");
    menu1 = Menu({ 250, 250, 500, 500 }, "Menu.png");
}

UserInterface::~UserInterface() {
}

void UserInterface::render() {
    Graphics::getInstance().clearUI();
    button1.render();
    button2.render();
    if (menu1.isOpen()) {
        menu1.render();
    }
}

void UserInterface::processMouseInput(const SDL_Event& mouseEvent) {
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

    if (button1.click(mouseX, mouseY, buttonDownX, buttonDownY)) {
        menu1.open();
    }

    if (button2.click(mouseX, mouseY, buttonDownX, buttonDownY)) {
        menu1.close();
    }
}
