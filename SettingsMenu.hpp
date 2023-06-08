#pragma once

#include <fstream>
#include <sstream>
#include "SDL.h"
#include "Graphics.hpp"
#include "CurrentDirectory.hpp"

void openSettings();
void loadConfigFileData(int&, int&, double&);
void saveChanges(int, int, double);