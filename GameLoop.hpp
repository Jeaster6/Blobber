#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <iostream>
#include "CurrentDirectory.hpp"
#include "GameState.hpp"
#include "UserInterface.hpp"

// load and run a game from a provided save file, or a new game if empty string
// returns true if ALT+F4 was pressed
bool gameplay(const std::string&);
