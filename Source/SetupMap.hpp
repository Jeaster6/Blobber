#pragma once

#include <iostream>
#include <vector>
#include "Editor.hpp"

const int MAX_MAP_SIZE=100;

static void showExistingFiles();
static int readUserInput(int&, int&);
static std::string selectMapForEditing();
static std::string selectMapSaveFile();
void setupMap();
