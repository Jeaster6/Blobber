#include "GameMap.hpp"

Tile* GameMap::getTile(int x, int y) {
    return &gameMap[x][y];
}

void GameMap::setTileWall(int x, int y, char direction, bool walled) {
    gameMap[x][y].setWall(direction, walled);
}

void GameMap::setTile(int x, int y, Tile* targetTile) {
    gameMap[x][y].setTile(targetTile);
}

void GameMap::setTile(int x, int y, bool N, bool E, bool S, bool W) {
    gameMap[x][y].setTile(N, E, S, W);
}

const int GameMap::getHeight() {
    return mapHeight;
}
const int GameMap::getWidth() {
    return mapWidth;
}

void GameMap::saveToVector() {
    for (int i=0; i<mapWidth; i++) {
        for (int j=0; j<mapHeight; j++) {
            savedTiles.push_back(gameMap[i][j]);
        }
    }
}

void GameMap::loadFromVector() {
    for (int i=mapWidth-1; i>=0; i--) {
        for (int j=mapHeight-1; j>=0; j--) {
            gameMap[i][j].setTile(&savedTiles.back());
            savedTiles.pop_back();
        }
    }
}

GameMap::GameMap(int mapWidth, int mapHeight) {
    this->mapWidth=mapWidth;
    this->mapHeight=mapHeight;
    gameMap=new Tile*[mapWidth];
    for (int i=0; i<mapWidth; i++) {
        gameMap[i]=new Tile[mapHeight];
    }
}

GameMap::GameMap(int mapWidth, int mapHeight, std::vector <Tile> savedTiles) {
    this->mapWidth=mapWidth;
    this->mapHeight=mapHeight;
    this->savedTiles=savedTiles;
    gameMap=new Tile*[mapWidth];
    for (int i=0; i<mapWidth; i++) {
        gameMap[i]=new Tile[mapHeight];
    }
    loadFromVector();
}

GameMap::~GameMap() {
    for (int i=0; i<mapWidth; i++) {
        delete[] gameMap[i];
    }
    delete[] gameMap;
}
