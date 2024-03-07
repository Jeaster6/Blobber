#include "UserInterface.hpp"

UserInterface::UserInterface() {
    buttons.clear();
    menus.clear();
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

    for (Button button : buttons) {
        if (button.click(mouseX, mouseY, buttonDownX, buttonDownY)) {
            menus[0].open();
        }
    }
}

void UserInterface::addButton(const std::string& title, const SDL_Rect& area, const std::string& texture) {
    for (Button button : buttons) {
        if (button.getTitle() == title) {
            throw std::runtime_error("Button with this title already exists!");
        }
    }
    buttons.push_back(Button(title, area, texture));
}

void UserInterface::removeButton(const std::string& title) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].getTitle() == title) {
            buttons.erase(buttons.begin() + i);
        }
    }
}

void UserInterface::addMenu(const std::string& title, const SDL_Rect& area, const std::string& texture) {
    for (Menu menu : menus) {
        if (menu.getTitle() == title) {
            throw std::runtime_error("Menu with this title already exists!");
        }
    }
    menus.push_back(Menu(title, area, texture));
}

Menu UserInterface::getMenuByTitle(const std::string& title) const {
    for (Menu menu : menus) {
        if (menu.getTitle() == title) {
            return menu;
        }
    }
    throw std::runtime_error("Menu with specified title not found");
}
