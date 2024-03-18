#include "MapObject.hpp"

void MapObject::addItems(const std::vector<std::string>& objectContents) {
    for (std::string item : objectContents) {
        this->contents.push_back(Item(item));
    }
}

void MapObject::removeItem(int itemIndex) {
    contents.erase(contents.begin() + itemIndex);
}

void MapObject::removeItem(const std::string& itemID) {
    for (int i = 0; i < contents.size(); i++) {
        if (contents[i].getID() == itemID) {
            contents.erase(contents.begin() + i);
            break;
        }
    }
}

std::string MapObject::getType() const {
    return type;
}

std::vector<Item> MapObject::getContents() const {
    return contents;
}

void MapObject::setObjectData(const MapObject& otherObject) {
    this->ID = otherObject.ID;
    this->contents = otherObject.contents;
    this->triggered = otherObject.triggered;
    this->type = otherObject.type;
}

void MapObject::removeObject() {
    triggered = false;
    ID = "";
    contents.clear();
    type = "";
}

void MapObject::triggerObject() {
    triggered = !triggered;
}

MapObject::MapObject(const std::string& objectID) {
    triggered = false;
    this->ID = objectID;
    addItems({ "item1", "item3" });
    type = "Object";
}

MapObject::MapObject() {
    triggered = false;
    ID = "";
    contents.clear();
    type = "";
}

MapObject::~MapObject() {
}
