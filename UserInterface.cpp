#include "UserInterface.hpp"

UserInterface::UserInterface() {
    buttons.clear();
    menus.clear();
    buttons.push_back(Button("Test", { 0, 0, 50, 50 }, "Button.png", []() { std::cout << "test"; }));
}

UserInterface::~UserInterface() {
}

void UserInterface::render() {
    for (Button button : buttons) {
        button.render();
    }

    for (Menu menu : menus) {
        if (menu.isOpen()) {
            menu.render();
        }
    }
}

void UserInterface::processMouseInput(const SDL_Event& mouseEvent) {
    int mouseCursorX = 0, mouseCursorY = 0;
    int mouseCursorOnButtonDownX = 0, mouseCursorOnButtonDownY = 0;
    bool mouseReleased = false;
    SDL_Event currentEvent;

    currentEvent.button.button = mouseEvent.button.button;
    SDL_GetMouseState(&mouseCursorOnButtonDownX, &mouseCursorOnButtonDownY);

    while (!mouseReleased) {
        SDL_PollEvent(&currentEvent);
        if (currentEvent.button.button == mouseEvent.button.button && currentEvent.type == SDL_MOUSEBUTTONUP) {
            mouseReleased = true;
        }
        SDL_GetMouseState(&mouseCursorX, &mouseCursorY);
    }

    for (Button button : buttons) {
        if (mouseCursorX >= button.getArea().x && mouseCursorX <= button.getArea().w + button.getArea().x && mouseCursorY >= button.getArea().y && mouseCursorY <= button.getArea().h + button.getArea().y) {
            if (mouseCursorOnButtonDownX >= button.getArea().x && mouseCursorOnButtonDownX <= button.getArea().w + button.getArea().x && mouseCursorOnButtonDownY >= button.getArea().y && mouseCursorOnButtonDownY <= button.getArea().h + button.getArea().y) {
                button.click();
            }
        }
    }
}
