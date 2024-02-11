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

void Tile::setTile(const std::string& northWallType, const std::string& eastWallType, const std::string& southWallType, const std::string& westWallType, const std::string& floorType, const std::string& ceilingType, MapObject*, bool explored) {
    this->northWallType = northWallType;
    this->eastWallType = eastWallType;
    this->southWallType = southWallType;
    this->westWallType = westWallType;
    this->floorType = floorType;
    this->ceilingType = ceilingType;
    this->mapObject = mapObject;
    this->explored = explored;
}

void Tile::setTile(Tile* targetTile) {
    this->northWallType = targetTile->northWallType;
    this->eastWallType = targetTile->eastWallType;
    this->southWallType = targetTile->southWallType;
    this->westWallType = targetTile->westWallType;
    this->floorType = targetTile->floorType;
    this->ceilingType = targetTile->ceilingType;
    this->mapObject = targetTile->mapObject;
    this->explored = targetTile->explored;
}

void Tile::spawnObject(const std::string& objectID) {
    mapObject = new MapObject(objectID);
}

void Tile::deSpawnObject() {
    delete mapObject;
}

bool Tile::isWalled(Direction direction) const {
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

bool Tile::hasFloor() const {
    return (this->floorType != "");
}

bool Tile::hasCeiling() const {
    return (this->ceilingType != "");
}

bool Tile::containsObject() const {
    return this->mapObject->exists();
}

std::string Tile::getWallType(Direction direction) const {
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

std::string Tile::getFloorType() const {
    return floorType;
}

std::string Tile::getCeilingType() const {
    return ceilingType;
}

MapObject* Tile::getObject() const {
    return mapObject;
}

std::unordered_set <std::string> Tile::getTextures() const {
    std::unordered_set <std::string> textures;
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

bool Tile::isFullyWalled() const {
    if (isWalled(N) && isWalled(E) && isWalled(S) && isWalled(W)) {
        return true;
    }
    else return false;
}

void Tile::markAsExplored() {
    explored = true;
}

bool Tile::isExplored() const {
    return explored;
}

Tile::Tile(const std::string& northWallType, const std::string& eastWallType, const std::string& southWallType, const std::string& westWallType, const std::string& floorType, const std::string& ceilingType, MapObject* mapObject, bool explored) {
    this->northWallType = northWallType;
    this->eastWallType = eastWallType;
    this->southWallType = southWallType;
    this->westWallType = westWallType;
    this->floorType = floorType;
    this->ceilingType = ceilingType;
    this->mapObject = mapObject;
    this->explored = explored;
}

Tile::Tile() {
    northWallType = "";
    eastWallType = "";
    southWallType = "";
    westWallType = "";
    floorType = "";
    ceilingType = "";
    mapObject = nullptr;
    explored = false;
}

Tile::~Tile() {
    delete mapObject;
}
