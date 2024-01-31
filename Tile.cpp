#include "Tile.hpp"

void Tile::setWall(Direction direction, const std::string& wallType) {
    switch(direction) {

        case Direction::N:
            northWallType = wallType;
            break;

        case Direction::E:
            eastWallType = wallType;
            break;

        case Direction::S:
            southWallType = wallType;
            break;

        case Direction::W:
            westWallType = wallType;
            break;
    }
}

void Tile::setFloorType(const std::string& floorType) {
    this->floorType = floorType;
}

void Tile::setCeilingType(const std::string& ceilingType) {
    this->ceilingType = ceilingType;
}

void Tile::setTile(const std::string& northWallType, const std::string& eastWallType, const std::string& southWallType, const std::string& westWallType, const std::string& floorType, const std::string& ceilingType, MapObject*) {
    this->northWallType = northWallType;
    this->eastWallType = eastWallType;
    this->southWallType = southWallType;
    this->westWallType = westWallType;
    this->floorType = floorType;
    this->ceilingType = ceilingType;
    this->mapObject = mapObject;
}

void Tile::setTile(Tile* targetTile) {
    this->northWallType = targetTile->northWallType;
    this->eastWallType = targetTile->eastWallType;
    this->southWallType = targetTile->southWallType;
    this->westWallType = targetTile->westWallType;
    this->floorType = targetTile->floorType;
    this->ceilingType = targetTile->ceilingType;
    this->mapObject = targetTile->mapObject;
}

void Tile::spawnObject(std::string objectID) {
    mapObject = new MapObject(objectID);
}

void Tile::deSpawnObject() {
    delete mapObject;
    mapObject = nullptr;
}

bool Tile::isWalled(Direction direction) {
    switch(direction) {

        case Direction::N:
            return (this->northWallType != "");
            break;

        case Direction::E:
            return (this->eastWallType != "");
            break;

        case Direction::S:
            return (this->southWallType != "");
            break;

        case Direction::W:
            return (this->westWallType != "");
            break;
    }
    return true;
}

bool Tile::hasFloor() {
    return (this->floorType != "");
}

bool Tile::hasCeiling() {
    return (this->ceilingType != "");
}

bool Tile::containsObject() {
    return this->mapObject->exists();
}

const std::string& Tile::getWallType(Direction direction) {
    switch (direction) {

        case Direction::N:
            return northWallType;

        case Direction::E:
            return eastWallType;

        case Direction::S:
            return southWallType;

        case Direction::W:
            return westWallType;

        default:
            static const std::string& emptyString = "";
            return emptyString;
    }
} 

const std::string& Tile::getFloorType() {
    return floorType;
}

const std::string& Tile::getCeilingType() {
    return ceilingType;
}

const std::string& Tile::getObjectType() {
    return mapObject->getObjectType();
}

std::unordered_set <std::string> Tile::getTextures() {
    textures.clear();
    if (northWallType != "") {
        textures.insert(northWallType);
    }

    if (eastWallType != "") {
        textures.insert(eastWallType);
    }

    if (southWallType != "") {
        textures.insert(southWallType);
    }

    if (westWallType != "") {
        textures.insert(westWallType);
    }

    if (floorType != "") {
        textures.insert(floorType);
    }

    if (ceilingType != "") {
        textures.insert(ceilingType);
    }

    //textures.insert(mapObject->getObjectType());
    return textures;
}

bool Tile::isFullyWalled() {
    if (isWalled(N) && isWalled(E) && isWalled(S) && isWalled(W)) {
        return true;
    }
    else return false;
}

Tile::Tile(const std::string& northWallType, const std::string& eastWallType, const std::string& southWallType, const std::string& westWallType, const std::string& floorType, const std::string& ceilingType, MapObject* mapObject) {
    this->northWallType = northWallType;
    this->eastWallType = eastWallType;
    this->southWallType = southWallType;
    this->westWallType = westWallType;
    this->floorType = floorType;
    this->ceilingType = ceilingType;
    this->mapObject = mapObject;
}

Tile::Tile() {
    northWallType = "";
    eastWallType = "";
    southWallType = "";
    westWallType = "";
    floorType = "";
    ceilingType = "";
    mapObject = nullptr;
}

Tile::~Tile() {
    delete mapObject;
    mapObject = nullptr;
}
