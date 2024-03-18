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

void UserInterface::render(const GameState& game) {
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
        std::vector<Item> partyInventory = game.getPlayer().getPartyInventory();
        std::vector<Item> objectContents;
        std::vector<std::string> partyInventoryItemNames;
        std::vector<std::string> objectContentsItemNames;

        if (inventory.getMode() == Mode::Looting) {
            if (game.isTileInFrontOfPlayerFree()) {
                objectContents = game.getTileInFrontOfPlayer().getObject().getContents();
            }

            for (Item item : objectContents) {
                objectContentsItemNames.push_back(item.getName());
            }
        }

        for (Item item : partyInventory) {
            partyInventoryItemNames.push_back(item.getName());
        }

        inventory.render(partyInventoryItemNames, objectContentsItemNames);
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

    // TODO: add party inventory code here
    if (inventory.isOpen()) {
        if (inventory.getMode() == Mode::Looting) {
            int playerInventorySize = (int)game.getPlayer().getPartyInventory().size();
            int objectContentsSize = (int)game.getTileInFrontOfPlayer().getObject().getContents().size();

            for (int i = 0; i < playerInventorySize; i++) {
                if (inventory.clickButton(i, mouseX, mouseY, buttonDownX, buttonDownY)) {
                    std::string itemID = game.getPlayer().getPartyInventory()[i].getID();
                    int x = 0;
                    int y = 0;
                    game.getCoordinatesOfTileInFrontOfPlayer(x, y);
                    game.removeItemFromPartyInventory(i);
                    game.addItemToObject(x, y, itemID);
                    game.addToListOfChanges(game.getPlayer().getCurrentMapFileName(), x, y, ChangeType::ItemAddedToObject, itemID);
                    return 2;
                }
            }

            for (int i = 0; i < objectContentsSize; i++) {
                if (inventory.clickButton(i + playerInventorySize, mouseX, mouseY, buttonDownX, buttonDownY)) {
                    std::string itemID = game.getTileInFrontOfPlayer().getObject().getContents()[i].getID();
                    int x = 0;
                    int y = 0;
                    game.getCoordinatesOfTileInFrontOfPlayer(x, y);
                    game.removeItemFromObject(x, y, i);
                    game.addToListOfChanges(game.getPlayer().getCurrentMapFileName(), x, y, ChangeType::ItemRemovedFromObject, itemID);
                    game.addItemToPartyInventory(itemID);
                    return 2;
                }
            }
        }

        return 2;
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

int UserInterface::openInventoryWindow(Mode mode) {
    inventory.open(mode);
    return 2;
}

int UserInterface::closeAllWindows() {
    modal.close();
    inventory.close();
    return 0;
}
