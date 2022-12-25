#pragma once

#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include "Editor.hpp"

const int MAX_MAP_SIZE=100;

struct pathLeafString {
    std::string operator() (const boost::filesystem::directory_entry& entry) const {
        return entry.path().leaf().string();
    }
};

std::vector<std::string> readMapsFromDirectory();
void showExistingFiles();
int readUserInput(int&, int&);
std::string selectMapForEditing();
std::string selectMapSaveFile();
void saveMap(std::string, GameMap*);
GameMap* loadMap(std::string);
void setupMap();
