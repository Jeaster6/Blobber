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
        Graphics::getInstance().displayMessage("Test message!");
        messageDisplayed = true;
        gameMap.activateTrigger(player.getX(), player.getY());
        addToListOfChanges(player.getCurrentMapFileName(), player.getX(), player.getY(), ChangeType::TriggerActivated, "Tile");
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
    listOfChanges.push_back(WorldChange(affectedMapName, locationX, locationY, changeType, subject));
}

void GameState::applyChangesToWorld() {
    for (WorldChange change : listOfChanges) {
        if (change.getAffectedMapName() == player.getCurrentMapFileName() && change.getLocationX() < gameMap.getWidth() && change.getLocationY() < gameMap.getHeight()) {
            switch (change.getChangeType()) {
                case ChangeType::AddItem:
                    break;

                case ChangeType::RemoveItem:
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
