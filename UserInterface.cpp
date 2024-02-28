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
