#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include "CurrentDirectory.hpp"
#include "GameMap.hpp"

const int SCREEN_WIDTH=2560;
const int SCREEN_HEIGHT=1440;
const int GAME_WIDTH=3*SCREEN_WIDTH/4;
const double FOV=0.65;

void gameplay();
