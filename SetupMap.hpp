#pragma once

#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include "Editor.hpp"

const int MAX_MAP_SIZE=100;

std::vector<std::string> readMapsFromDirectory();
void showExistingFiles();
int readUserInput(int&, int&);
std::string selectMapForEditing();
std::string selectMapSaveFile();
void saveMap(std::string, std::shared_ptr<GameMap>);
std::shared_ptr<GameMap> loadMap(std::string);
void setupMap();
