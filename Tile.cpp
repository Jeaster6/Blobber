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

void Tile::setTile(const std::string& northWallType, const std::string& eastWallType, const std::string& southWallType, const std::string& westWallType, const std::string& floorType, const std::string& ceilingType, std::shared_ptr<MapObject> mapObject, std::shared_ptr<MapTrigger> mapTrigger, bool explored) {
    this->northWallType = northWallType;
    this->eastWallType = eastWallType;
    this->southWallType = southWallType;
    this->westWallType = westWallType;
    this->floorType = floorType;
    this->ceilingType = ceilingType;
    this->mapObject = mapObject;
    this->mapTrigger = mapTrigger;
    this->explored = explored;
}

void Tile::setTile(Tile targetTile) {
    this->northWallType = targetTile.northWallType;
    this->eastWallType = targetTile.eastWallType;
    this->southWallType = targetTile.southWallType;
    this->westWallType = targetTile.westWallType;
    this->floorType = targetTile.floorType;
    this->ceilingType = targetTile.ceilingType;
    this->mapObject = targetTile.mapObject;
    this->mapTrigger = targetTile.mapTrigger;
    this->explored = targetTile.explored;
}

void Tile::placeObject(const std::string& objectID) {
    mapObject = std::make_shared<MapObject>(objectID);
}

void Tile::removeObject() {
    mapObject = nullptr;
}

void Tile::placeTrigger(TriggerType triggerType, const std::string& subject, bool triggered) {
    mapTrigger = std::make_shared<MapTrigger>(triggerType, subject, triggered);
}

void Tile::removeTrigger() {
    mapTrigger = nullptr;
}

bool Tile::isWalled(Direction direction) const {
    switch(direction) {

        case Direction::N:
            return (northWallType != "");
            break;

        case Direction::E:
            return (eastWallType != "");
            break;

        case Direction::S:
            return (southWallType != "");
            break;

        case Direction::W:
            return (westWallType != "");
            break;
    }
    return true;
}

bool Tile::hasFloor() const {
    return (floorType != "");
}

bool Tile::hasCeiling() const {
    return (ceilingType != "");
}

bool Tile::containsObject() const {
    return mapObject != nullptr;
}

bool Tile::containsTrigger() const {
    return mapTrigger != nullptr;
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

std::shared_ptr<MapObject> Tile::getObject() const {
    return mapObject;
}

std::shared_ptr<MapTrigger> Tile::getTrigger() const {
    return mapTrigger;
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

    if (mapObject != nullptr) {
        textures.insert(mapObject->getObjectType());
    }

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

Tile::Tile(const std::string& northWallType, const std::string& eastWallType, const std::string& southWallType, const std::string& westWallType, const std::string& floorType, const std::string& ceilingType, std::shared_ptr<MapObject> mapObject, std::shared_ptr<MapTrigger> mapTrigger, bool explored) {
    this->northWallType = northWallType;
    this->eastWallType = eastWallType;
    this->southWallType = southWallType;
    this->westWallType = westWallType;
    this->floorType = floorType;
    this->ceilingType = ceilingType;
    this->mapObject = mapObject;
    this->mapTrigger = mapTrigger;
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
    mapTrigger = nullptr;
    explored = false;
}

Tile::~Tile() {
}
