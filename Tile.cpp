#include "Tile.hpp"

void Tile::setWall(Direction direction, bool walled) {
    switch(direction) {

        case Direction::N:
            N = walled;
            break;

        case Direction::E:
            E = walled;
            break;

        case Direction::S:
            S = walled;
            break;

        case Direction::W:
            W = walled;
            break;
    }
}

void Tile::setTile(bool N, bool E, bool S, bool W) {
    this->N = N;
    this->E = E;
    this->S = S;
    this->W = W;
}

void Tile::setTile(Tile* targetTile) {
    this->N = targetTile->N;
    this->E = targetTile->E;
    this->S = targetTile->S;
    this->W = targetTile->W;
    this->mapObject = targetTile->mapObject;
}

void Tile::spawnObject(std::string objectID) {
    mapObject = new MapObject(objectID);
}

void Tile::deSpawnObject() {
    delete mapObject;
    mapObject = NULL;
}

bool Tile::isWalled(Direction direction) {
    switch(direction) {

        case Direction::N:
            return this->N;
            break;

        case Direction::E:
            return this->E;
            break;

        case Direction::S:
            return this->S;
            break;

        case Direction::W:
            return this->W;
            break;
    }
    return true;
}

bool Tile::containsObject() {
    return this->mapObject->exists();
}

Tile::Tile(bool N, bool E, bool S, bool W, MapObject* mapObject) {
    this->N = N;
    this->E = E;
    this->S = S;
    this->W = W;
    this->mapObject = mapObject;
}

Tile::Tile(){
    N = true;
    E = true;
    S = true;
    W = true;
    mapObject = NULL;
}

Tile::~Tile(){
    delete mapObject;
    mapObject = NULL;
}
