#include "SetupMap.hpp"

static void showExistingFiles() {
    std::vector<std::string> files = readMapsFromDirectory();
    for (unsigned int i = 0; i < files.size(); i++) {
        std::cout << i + 1 << ": " << files[i] << "\n";
    }
}

static int readUserInput(int& mapWidth, int& mapHeight) {
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

static std::string selectMapForEditing() {
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

static std::string selectMapSaveFile() {
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

void setupMap() {
    int mapWidth = 0;
    int mapHeight = 0;
    switch (readUserInput(mapWidth, mapHeight)) {
        case 1: {
            GameMap gameMap = GameMap(mapWidth, mapHeight);
            runMapEditor(gameMap);
            gameMap.saveToFile(getMapsDirectory() + selectMapSaveFile() + ".dat");
            break;
        }

        case 2: {
            GameMap gameMap;
            gameMap.loadFromFile(selectMapForEditing());
            runMapEditor(gameMap);
            gameMap.saveToFile(getMapsDirectory() + selectMapSaveFile() + ".dat");
            break;
        }

        case 3:
            break;
    }
}
