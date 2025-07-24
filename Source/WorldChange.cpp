#include "WorldChange.hpp"

WorldChange::WorldChange() {
    affectedMapName = "";
    locationX = 0;
    locationY = 0;
    changeType = TileExplored;
    changeSubject = "";
}

WorldChange::WorldChange(const std::string& affectedMapName, int locationX, int locationY, ChangeType changeType, const std::string& changeSubject) {
    this->affectedMapName = affectedMapName;
    this->locationX = locationX;
    this->locationY = locationY;
    this->changeType = changeType;
    this->changeSubject = changeSubject;
}

WorldChange::~WorldChange() {
}

std::string WorldChange::getAffectedMapName() const {
    return affectedMapName;
}

int WorldChange::getLocationX() const {
    return locationX;
}

int WorldChange::getLocationY() const {
    return locationY;
}

ChangeType WorldChange::getChangeType() const {
    return changeType;
}

std::string WorldChange::getChangeSubject() const {
    return changeSubject;
}
