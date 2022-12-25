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
    return (objectID!="");
}

MapObject::MapObject(std::string objectID) {
    this->objectID=objectID;
    addItems({"item1"});
}

MapObject::MapObject() {
    objectID="";
    objectContents.clear();
}

MapObject::~MapObject() {
}
