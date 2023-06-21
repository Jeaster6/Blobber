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

bool MapObject::exists() {
    return (this != nullptr);
}

const std::string& MapObject::getObjectType() {
    return type;
}

MapObject::MapObject(std::string objectID) {
    this->ID = objectID;
    addItems({"item1"});
    type = "Object";
}

MapObject::MapObject() {
    ID = "";
    objectContents.clear();
    type = "";
}

MapObject::~MapObject() {
}
