#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include "Graphics.hpp"
#include "SettingsMenu.hpp"
#include "GameLoop.hpp"
#include "CurrentDirectory.hpp"

void openMainMenu();
static std::string getSavedGameFile();
