#include "MapObject.hpp"

void MapObject::addItems(std::vector <std::string> objectContents) {
    while (!objectContents.empty()) {
        Item newItem(objectContents.back());
        this->objectContents.push_back(newItem);
        objectContents.pop_back();
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

MapObject::MapObject(std::string objectID) {
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
