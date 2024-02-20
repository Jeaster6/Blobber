#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <iostream>
#include "CurrentDirectory.hpp"
#include "Graphics.hpp"
#include "GameState.hpp"

// load and run a game from a provided save file, or a new game if empty string

void gameplay(const std::string&);
