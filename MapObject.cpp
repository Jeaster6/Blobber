#include "MapObject.hpp"

void MapObject::addItems(const std::vector<std::string>& objectContents) {
    for (std::string item : objectContents) {
        Item newItem(item);
        this->objectContents.push_back(newItem);
    }
}

void MapObject::removeItems() {
    objectContents.clear();
}

std::string MapObject::getObjectType() const {
    return type;
}

void MapObject::triggerObject() {
    triggered = !triggered;
}

MapObject::MapObject(const std::string& objectID) {
    triggered = false;
    this->ID = objectID;
    addItems({"item1"});
    type = "Object";
}

MapObject::MapObject() {
    triggered = false;
    ID = "";
    objectContents.clear();
    type = "";
}

MapObject::~MapObject() {
}
