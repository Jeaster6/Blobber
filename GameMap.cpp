#include "GameMap.hpp"

Tile GameMap::getTile(int x, int y) const {
    return map[x][y];
}

void GameMap::setTileWall(int x, int y, Direction direction, const std::string& wallType) {
    map[x][y].setWall(direction, wallType);
}

void GameMap::setFloorType(int x, int y, const std::string& floorType) {
    map[x][y].setFloorType(floorType);
}

void GameMap::setCeilingType(int x, int y, const std::string& ceilingType) {
    map[x][y].setCeilingType(ceilingType);
}

void GameMap::setTile(int x, int y, const Tile& targetTile) {
    map[x][y].setTile(targetTile);
}

void GameMap::setTileParameters(int x, int y, const std::string& northWall, const std::string& eastWall, const std::string& southWall, const std::string& westWall, const std::string& floor, const std::string& ceiling, const MapObject& mapObject, const MapTrigger& mapTrigger, bool explored) {
    map[x][y].setTile(northWall, eastWall, southWall, westWall, floor, ceiling, mapObject, mapTrigger, explored);
}

int GameMap::getHeight() const {
    return height;
}

int GameMap::getWidth() const {
    return width;
}

void GameMap::saveToVector() {
    savedTiles.clear();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            savedTiles.push_back(map[i][j]);
        }
    }
}

void GameMap::loadFromVector() {
    for (int i = width - 1; i >= 0; i--) {
        for (int j = height - 1; j >= 0; j--) {
            map[i][j].setTile(savedTiles.back());
            savedTiles.pop_back();
        }
    }
    savedTiles.clear();
}

void GameMap::saveToFile(const std::string& outputFile) {
    saveToVector();
    std::ofstream ofs(outputFile);
    boost::archive::binary_oarchive boostArchive(ofs);
    boostArchive << *this;
}

void GameMap::loadFromFile(const std::string& inputFile) {
    std::ifstream ifs(getMapsDirectory() + inputFile);
    boost::archive::binary_iarchive boostArchive(ifs);
    boostArchive >> *this;
}

void GameMap::placeObject(int x, int y, const std::string& objectID) {
    map[x][y].placeObject(objectID);
}

void GameMap::triggerObject(int x, int y) {
    map[x][y].triggerObject();
}

void GameMap::removeObject(int x, int y) {
    map[x][y].removeObject();
}

void GameMap::placeTrigger(int x, int y, TriggerType triggerType, const std::string& subject, bool triggered) {
    map[x][y].placeTrigger(triggerType, subject, triggered);
}

void GameMap::activateTrigger(int x, int y) {
    map[x][y].activateTrigger();
}

void GameMap::removeTrigger(int x, int y) {
    map[x][y].removeTrigger();
}

void GameMap::markTileAsExplored(int x, int y) {
    map[x][y].markAsExplored();
}

void GameMap::addItemToObject(int x, int y, const std::string& itemID) {
    map[x][y].addItemToObject(itemID);
}

void GameMap::removeItemFromObject(int x, int y, int itemIndex) {
    map[x][y].removeItemFromObject(itemIndex);
}

void GameMap::removeItemFromObject(int x, int y, const std::string& itemID) {
    map[x][y].removeItemFromObject(itemID);
}

// constructs a map object based on size variables and populates it with data from savedTiles vector
void GameMap::init() {
    map = std::make_shared<std::shared_ptr<Tile[]>[]>(width);
    for (int i = 0; i < width; i++) {
        map[i] = std::make_shared<Tile[]>(height);
    }
    loadFromVector();
}

GameMap::GameMap() {
    height = 0;
    width = 0;
}

GameMap::GameMap(int width, int height) {
    this->width = width;
    this->height = height;
    savedTiles.clear();
    map = std::make_shared<std::shared_ptr<Tile[]>[]>(width);
    for (int i = 0; i < width; i++) {
        map[i] = std::make_shared<Tile[]>(height);
    }
}

GameMap::GameMap(int width, int height, const std::vector<Tile>& savedTiles) {
    this->width = width;
    this->height = height;
    this->savedTiles.clear();
    map = std::make_shared<std::shared_ptr<Tile[]>[]>(width);
    for (int i = 0; i < width; i++) {
        map[i] = std::make_shared<Tile[]>(height);
    }
    this->savedTiles = savedTiles;
    loadFromVector();
}

GameMap::~GameMap() {
}
