#include "GameMap.hpp"

Tile* GameMap::getTile(int x, int y) {
    return &map[x][y];
}

void GameMap::setTileWall(int x, int y, Direction direction, bool walled) {
    map[x][y].setWall(direction, walled);
}

void GameMap::setTile(int x, int y, Tile* targetTile) {
    map[x][y].setTile(targetTile);
}

void GameMap::setTile(int x, int y, bool N, bool E, bool S, bool W) {
    map[x][y].setTile(N, E, S, W);
}

int GameMap::getHeight() {
    return height;
}
int GameMap::getWidth() {
    return width;
}

void GameMap::saveToVector() {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            savedTiles.push_back(map[i][j]);
        }
    }
}

void GameMap::loadFromVector() {
    for (int i = width - 1; i >= 0; i--) {
        for (int j = height - 1; j >= 0; j--) {
            map[i][j].setTile(&savedTiles.back());
            savedTiles.pop_back();
        }
    }
}

GameMap::GameMap(int width, int height) {
    this->width = width;
    this->height = height;
    savedTiles.clear();
    map = new Tile*[width];
    for (int i = 0; i < width; i++) {
        map[i] = new Tile[height];
    }
}

GameMap::GameMap(int width, int height, std::vector <Tile> savedTiles) {

    this->width = width;
    this->height = height;
    map = new Tile * [width];
    for (int i = 0; i < width; i++) {
        map[i] = new Tile[height];
    }
    this->savedTiles = savedTiles;
    loadFromVector();
}

GameMap::~GameMap() {
    for (int i = 0; i < width; i++) {
        delete[] map[i];
        map[i] = NULL;
    }
    delete[] map;
    map = NULL;
}
