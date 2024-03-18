#include "Item.hpp"

Item::Item(const std::string& itemID) {
    this->ID = itemID;
    loadItemData();
}

Item::Item() {
    ID = "";
    name = "";
}

Item::~Item() {
}

void Item::loadItemData() {
    std::ifstream itemsFile(getDataDirectory() + "Items.csv");

    std::vector <std::string> row;
    std::string word, line;

    while (getline (itemsFile, line)) {
        row.clear();
        std::stringstream sline(line);

        while (getline(sline, word, ',')) {
            row.push_back(word);
        }

        if (row[0] == ID) {
            name = row[1];
            break;
        }
    }
}

std::string Item::getID() const {
    return ID;
}

std::string Item::getName() const {
    return name;
}
