#include "GameState.hpp"

GameState::GameState() {
    player = Player();
    initMap();
}

GameState::~GameState() {
}

void GameState::initMap() {
    loadCurrentMap();
    applyChangesToWorld();
}

void GameState::loadCurrentMap() {
    std::ifstream ifs(getMapsDirectory() + player.getCurrentMapFileName());
    boost::archive::binary_iarchive boostArchive(ifs);
    boostArchive >> gameMap;
}

void GameState::markTileAsExplored() {
    if (!gameMap->getTile(player.getX(), player.getY()).isExplored()) {
        gameMap->getTile(player.getX(), player.getY()).markAsExplored();
        addToListOfChanges(player.getCurrentMapFileName(), player.getX(), player.getY(), ChangeType::TileExplored, "Tile");
    }
}

void GameState::movePlayerForward() {
    if (!gameMap->getTile(player.getX(), player.getY()).isWalled(player.getDirection())) {
        player.moveForward();
        gameMap->animateForwardMovement(player);
        markTileAsExplored();
    }
}

void GameState::movePlayerBackward() {
    Direction targetDirection = player.getDirection();
    targetDirection++;
    targetDirection++;
    if (!gameMap->getTile(player.getX(), player.getY()).isWalled(targetDirection)) {
        player.moveBackward();
        gameMap->animateBackwardMovement(player);
        markTileAsExplored();
    }
}

void GameState::movePlayerLeft() {
    Direction targetDirection = player.getDirection();
    targetDirection--;
    if (!gameMap->getTile(player.getX(), player.getY()).isWalled(targetDirection)) {
        player.moveLeft();
        gameMap->animateSidestepLeft(player);
        markTileAsExplored();
    }
}

void GameState::movePlayerRight() {
    Direction targetDirection = player.getDirection();
    targetDirection++;
    if (!gameMap->getTile(player.getX(), player.getY()).isWalled(targetDirection)) {
        player.moveRight();
        gameMap->animateSidestepRight(player);
        markTileAsExplored();
    }
}

void GameState::turnPlayerLeft() {
    player.turnLeft();
    gameMap->animateLeftRotation(player);
}

void GameState::turnPlayerRight() {
    player.turnRight();
    gameMap->animateRightRotation(player);
}

void GameState::renderPlayerView() {
    gameMap->renderVisibleArea(player);
    gameMap->makeScreenSnapshot(player);
}

void GameState::addToListOfChanges(const std::string& affectedMapName, int locationX, int locationY, ChangeType changeType, const std::string& subject) {
    listOfChanges.push_back(WorldChange(affectedMapName, locationX, locationY, changeType, subject));
}

void GameState::applyChangesToWorld() {
    for (WorldChange change : listOfChanges) {
        if (change.getAffectedMapName() == player.getCurrentMapFileName() && change.getLocationX() < gameMap->getWidth() && change.getLocationY() < gameMap->getHeight()) {
            switch (change.getChangeType()) {
                case ChangeType::AddItem:
                    break;

                case ChangeType::RemoveItem:
                    break;

                case ChangeType::ObjectTriggered:
                    if (gameMap->getTile(change.getLocationX(), change.getLocationY()).getObject().getObjectType() != "") {
                        gameMap->getTile(change.getLocationX(), change.getLocationY()).getObject().triggerObject();
                    }
                    break;

                case ChangeType::TileExplored:
                    gameMap->getTile(change.getLocationX(), change.getLocationY()).markAsExplored();
                    break;

                default:
                    break;
            }
        }
    }
}

void GameState::loadGame(const std::string& saveFile) {
    std::ifstream ifs(getSaveFileDirectory() + saveFile);
    boost::archive::binary_iarchive boostArchive(ifs);
    boostArchive >> *this;
    this->initMap();
}

void GameState::saveGame(const std::string& saveFile) const {
    std::ofstream ofs(getSaveFileDirectory() + saveFile);
    boost::archive::binary_oarchive boostArchive(ofs);
    boostArchive << *this;
}
