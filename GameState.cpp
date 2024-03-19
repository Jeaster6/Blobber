#include "GameState.hpp"

GameState::GameState() {
    messageDisplayed = false;
    loadGame("");
}

GameState::GameState(const std::string& saveFileName) {
    messageDisplayed = false;
    loadGame(saveFileName);
}

GameState::~GameState() {
}

void GameState::initMap() {
    loadCurrentMap();
    applyChangesToWorld();
    Graphics::getInstance().loadMapTextures(gameMap);
}

void GameState::loadCurrentMap() {
    std::ifstream ifs(getMapsDirectory() + player.getCurrentMapFileName());
    boost::archive::binary_iarchive boostArchive(ifs);
    boostArchive >> gameMap;
}

void GameState::markTileAsExplored() {
    if (!gameMap.getTile(player.getX(), player.getY()).isExplored()) {
        gameMap.markTileAsExplored(player.getX(), player.getY());
        addToListOfChanges(player.getCurrentMapFileName(), player.getX(), player.getY(), ChangeType::TileExplored, "Tile");
    }
}

void GameState::checkAndHandleTrigger() {
    if (gameMap.getTile(player.getX(), player.getY()).containsActiveTrigger()) {
        switch (gameMap.getTile(player.getX(), player.getY()).getTrigger().getType()) {

            case TriggerType::DisplayMessage:
                Graphics::getInstance().displayMessage(gameMap.getTile(player.getX(), player.getY()).getTrigger().getSubject());
                messageDisplayed = true;
                gameMap.activateTrigger(player.getX(), player.getY());
                addToListOfChanges(player.getCurrentMapFileName(), player.getX(), player.getY(), ChangeType::TriggerActivated, "Tile");
                break;

            case TriggerType::MapExit:
                player.teleportToCoordinates(0, 0, gameMap.getTile(player.getX(), player.getY()).getTrigger().getSubject());
                initMap();
                break;

            case TriggerType::Null:
                break;
        }
    }
}

void GameState::movePlayerForward() {
    if (!gameMap.getTile(player.getX(), player.getY()).isWalled(player.getDirection())) {
        player.moveForward();
        markTileAsExplored();
        Graphics::getInstance().animateForwardMovement(gameMap, player);
        checkAndHandleTrigger();
    }
}

void GameState::movePlayerBackward() {
    Direction targetDirection = player.getDirection();
    targetDirection++;
    targetDirection++;
    if (!gameMap.getTile(player.getX(), player.getY()).isWalled(targetDirection)) {
        player.moveBackward();
        markTileAsExplored();
        Graphics::getInstance().animateBackwardMovement(gameMap, player);
        checkAndHandleTrigger();
    }
}

void GameState::movePlayerLeft() {
    Direction targetDirection = player.getDirection();
    targetDirection--;
    if (!gameMap.getTile(player.getX(), player.getY()).isWalled(targetDirection)) {
        player.moveLeft();
        markTileAsExplored();
        Graphics::getInstance().animateSidestepLeft(gameMap, player);
        checkAndHandleTrigger();
    }
}

void GameState::movePlayerRight() {
    Direction targetDirection = player.getDirection();
    targetDirection++;
    if (!gameMap.getTile(player.getX(), player.getY()).isWalled(targetDirection)) {
        player.moveRight();
        markTileAsExplored();
        Graphics::getInstance().animateSidestepRight(gameMap, player);
        checkAndHandleTrigger();
    }
}

void GameState::turnPlayerLeft() {
    player.turnLeft();
    Graphics::getInstance().animateLeftRotation(gameMap, player);
}

void GameState::turnPlayerRight() {
    player.turnRight();
    Graphics::getInstance().animateRightRotation(gameMap, player);
}

void GameState::renderPlayerView() {
    Graphics::getInstance().renderPlayerView(gameMap, player);
}

void GameState::addToListOfChanges(const std::string& affectedMapName, int locationX, int locationY, ChangeType changeType, const std::string& subject) {
    bool changeDeleted = false;
    for (int i = 0; i < listOfChanges.size(); i++) {
        if (listOfChanges[i].getAffectedMapName() == affectedMapName && listOfChanges[i].getLocationX() == locationX && listOfChanges[i].getLocationY() == locationY && listOfChanges[i].getChangeSubject() == subject) {
            if ((listOfChanges[i].getChangeType() == ChangeType::ItemAddedToObject && changeType == ChangeType::ItemRemovedFromObject) || (listOfChanges[i].getChangeType() == ChangeType::ItemRemovedFromObject && changeType == ChangeType::ItemAddedToObject)) {
                listOfChanges.erase(listOfChanges.begin() + i);
                changeDeleted = true;
                break;
            }
        }
    }

    if (!changeDeleted) {
        listOfChanges.push_back(WorldChange(affectedMapName, locationX, locationY, changeType, subject));
    }
}

void GameState::applyChangesToWorld() {
    for (WorldChange change : listOfChanges) {
        if (change.getAffectedMapName() == player.getCurrentMapFileName() && change.getLocationX() < gameMap.getWidth() && change.getLocationY() < gameMap.getHeight()) {

            switch (change.getChangeType()) {
                case ChangeType::ItemAddedToObject:
                    if (gameMap.getTile(change.getLocationX(), change.getLocationY()).containsObject()) {
                        gameMap.addItemToObject(change.getLocationX(), change.getLocationY(), change.getChangeSubject());
                    }
                    break;

                case ChangeType::ItemRemovedFromObject:
                    if (gameMap.getTile(change.getLocationX(), change.getLocationY()).containsObject()) {
                        gameMap.removeItemFromObject(change.getLocationX(), change.getLocationY(), change.getChangeSubject());
                    }
                    break;

                case ChangeType::ObjectTriggered:
                    if (gameMap.getTile(change.getLocationX(), change.getLocationY()).getObject().getType() != "") {
                        gameMap.triggerObject(change.getLocationX(), change.getLocationY());
                    }
                    break;

                case ChangeType::TriggerActivated:
                    if (gameMap.getTile(change.getLocationX(), change.getLocationY()).getTrigger().getType() != TriggerType::Null) {
                        gameMap.activateTrigger(change.getLocationX(), change.getLocationY());
                    }
                    break;

                case ChangeType::TileExplored:
                    gameMap.markTileAsExplored(change.getLocationX(), change.getLocationY());
                    break;

                default:
                    break;
            }
        }
    }
}

Player GameState::getPlayer() const {
    return player;
}

GameMap GameState::getMap() const {
    return gameMap;
}

void GameState::closeMessage() {
    messageDisplayed = false;
}

bool GameState::isMessageDisplayed() const {
    return messageDisplayed;
}

bool GameState::isTileInFrontOfPlayerFree() const {
    switch (player.getDirection()) {

        case Direction::N:
            return player.getY() > 0;

        case Direction::E:
            return player.getX() < gameMap.getWidth() - 1;

        case Direction::S:
            return player.getY() < gameMap.getHeight() - 1;

        case Direction::W:
            return player.getX() > 0;

        default:
            return false;
    }
}

Tile GameState::getTileInFrontOfPlayer() const {
    switch (player.getDirection()) {

        case Direction::N:
            if (player.getY() > 0) {
                return gameMap.getTile(player.getX(), player.getY() - 1);
            }
            break;

        case Direction::E:
            if (player.getX() < gameMap.getWidth() - 1) {
                return gameMap.getTile(player.getX() + 1, player.getY());
            }
            break;

        case Direction::S:
            if (player.getY() < gameMap.getHeight() - 1) {
                return gameMap.getTile(player.getX(), player.getY() + 1);
            }
            break;

        case Direction::W:
            if (player.getX() > 0) {
                return gameMap.getTile(player.getX() - 1, player.getY());
            }
            break;
    }

    throw std::runtime_error("Player is facing the edge of map!");
}

void GameState::getCoordinatesOfTileInFrontOfPlayer(int& x, int& y) {
    switch (player.getDirection()) {

        case Direction::N:
            if (player.getY() > 0) {
                x = player.getX();
                y = player.getY() - 1;
            }
            else throw std::runtime_error("Player is facing the edge of map!");
            break;

        case Direction::E:
            if (player.getX() < gameMap.getWidth() - 1) {
                x = player.getX() + 1;
                y = player.getY();
            }
            else throw std::runtime_error("Player is facing the edge of map!");
            break;

        case Direction::S:
            if (player.getY() < gameMap.getHeight() - 1) {
                x = player.getX();
                y = player.getY() + 1;
            }
            else throw std::runtime_error("Player is facing the edge of map!");
            break;

        case Direction::W:
            if (player.getX() > 0) {
                x = player.getX() - 1;
                y = player.getY();
            }
            else throw std::runtime_error("Player is facing the edge of map!");
            break;
    }
}

void GameState::addItemToPartyInventory(const std::string& itemID) {
    player.addToPartyInventory(itemID);
}

void GameState::removeItemFromPartyInventory(int itemIndex) {
    player.removeFromPartyInventory(itemIndex);
}

void GameState::addItemToObject(int x, int y, const std::string& itemID) {
    gameMap.addItemToObject(x, y, itemID);
}

void GameState::removeItemFromObject(int x, int y, int itemIndex) {
    gameMap.removeItemFromObject(x, y, itemIndex);
}

void GameState::loadGame(const std::string& saveFile) {
    if (saveFile != "") {
        std::ifstream ifs(getSaveFileDirectory() + saveFile);
        boost::archive::binary_iarchive boostArchive(ifs);
        boostArchive >> *this;
    }

    initMap();
}

void GameState::saveGame(const std::string& saveFile) const {
    std::ofstream ofs(getSaveFileDirectory() + saveFile);
    boost::archive::binary_oarchive boostArchive(ofs);
    boostArchive << *this;
}
