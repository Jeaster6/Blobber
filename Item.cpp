#include "Item.hpp"


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

        if (row[0]==itemID) {
            itemName=row[1];
            break;
        }
    }

    itemsFile.close();
}

Item::Item(std::string itemID) {
    this->itemID=itemID;
    loadItemData();
}

Item::Item() {
}

Item::~Item() {
}
