#include "SetupMap.hpp"

std::vector<std::string> readMapsFromDirectory() {
    std::vector<std::string> allFiles;
    std::vector<std::string> mapFiles;
    boost::filesystem::directory_iterator start(getMapsDirectory());
    boost::filesystem::directory_iterator end;
    transform(start, end, back_inserter(allFiles), pathLeafString());

    mapFiles.clear();
    for (unsigned int i = 0; i < allFiles.size(); i++) {
        if (allFiles[i].substr(allFiles[i].length() - 4) == ".dat") {
            mapFiles.push_back(allFiles[i]);
        }
    }

    return mapFiles;
}

void showExistingFiles() {
    std::vector<std::string> files = readMapsFromDirectory();
    for (unsigned int i = 0; i < files.size(); i++) {
        std::cout << i + 1 << ": " << files[i] << "\n";
    }
}

int readUserInput(int& mapWidth, int& mapHeight) {
    std::string userInput;
    bool validInput = false;

    std::cout << "Welcome to map editor!\nDo you want to create a new map(n), edit an existing map(e) or quit(q)?: ";
    std::cin >> userInput;

    while (true) {
        if (userInput.size() == 1) {
            switch (userInput[0]) {
                case 'n':
                    std::cout << "\nPlease enter map size parameters.\n\n";

                    while (!validInput) {
                        std::cout << "Map width: ";
                        while (!(std::cin >> mapWidth)) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "\nWidth must be a number! Try again: ";
                        }

                        if (mapWidth<1 || mapWidth>MAX_MAP_SIZE) {
                            std::cout << "\nMap width must be between 1 and " << MAX_MAP_SIZE << "!\n";
                        }
                        else {
                            validInput = true;
                        }
                    }

                    validInput = false;
                    while (!validInput) {
                        std::cout << "Map height: ";
                        while (!(std::cin >> mapHeight)) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "\nHeight must be a number! Try again: ";
                        }

                        if (mapHeight<1 || mapHeight>MAX_MAP_SIZE) {
                            std::cout << "\nMap height must be between 1 and " << MAX_MAP_SIZE << "!\n";
                        }
                        else {
                            validInput = true;
                        }
                    }
                    return 1;
                    break;

                case 'e':
                    return 2;
                    break;

                case 'q':
                    return 0;
                    break;
            }
        }
        std::cout << "\nPlease enter a valid input: ";
        std::cin >> userInput;
    }
}

std::string selectMapForEditing() {
    std::vector<std::string> files = readMapsFromDirectory();
    int userInput = -1;

    showExistingFiles();
    std::cout << "\nSelect a map to open using its assigned number: ";
    while (userInput < 1 || userInput > (int)files.size()) {
        while (!(std::cin >> userInput)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nYou must enter a valid integer! Try again: ";
        }
        if (userInput < 1 || userInput >(int)files.size()) {
            std::cout << "\nYou must select one of the options! Try again: ";
        }
    }
    return files[userInput - 1];
}

std::string selectMapSaveFile() {
    std::string outputFile = "";
    bool saveFileSelected = false;
    while (!saveFileSelected) {
        std::vector<std::string> files = readMapsFromDirectory();

        std::cout << "\nSaving a map. Map files that currently exist in this folder:\n";
        showExistingFiles();
        std::cout << "\nSave as: ";

        std::cin >> outputFile;
        saveFileSelected = true;

        for (unsigned int i = 0; i < files.size(); i++) {
            if ((outputFile + ".dat") == files[i]) {
                std::string input = "";
                std::cout << "\nFile already exists. Would you like to overwrite it?(y/n): ";
                std::cin >> input;
                if (input != "y") {
                    saveFileSelected = false;
                }
            }
        }
    }
    return outputFile;
}

void saveMap(std::string outputFile, std::shared_ptr<GameMap> gameMap) {
    gameMap->saveToVector();
    std::ofstream ofs(outputFile);
    boost::archive::binary_oarchive oa(ofs);
    oa << gameMap;
}

std::shared_ptr<GameMap> loadMap(std::string inputFile) {
    std::shared_ptr<GameMap> gameMap;

    std::ifstream ifs(getMapsDirectory() + inputFile);
    boost::archive::binary_iarchive ia(ifs);
    ia >> gameMap;

    return gameMap;
}

void setupMap() {
    int mapWidth = 0;
    int mapHeight = 0;
    switch (readUserInput(mapWidth, mapHeight)) {
        case 1: {
            std::shared_ptr<GameMap> gameMap(std::make_shared<GameMap>(mapWidth, mapHeight));
            runMapEditor(gameMap);
            saveMap(getMapsDirectory() + selectMapSaveFile() + ".dat", gameMap);
            break;
        }

        case 2: {
            std::shared_ptr<GameMap> gameMap = loadMap(selectMapForEditing());
            runMapEditor(gameMap);
            saveMap(getMapsDirectory() + selectMapSaveFile() + ".dat", gameMap);
            break;
        }

        case 3:
            break;
    }
}
