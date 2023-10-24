#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "Direction.hpp"
#include "GameMap.hpp"

const int SCREEN_WIDTH=1920;
const int SCREEN_HEIGHT=1080;
const int GRID_TILE_SIZE=40;
const int TILE_SIZE=(int)(GRID_TILE_SIZE*0.96);
const int WALL_THICKNESS=(int)(GRID_TILE_SIZE*0.16);

void processMouseAction(GameMap*, SDL_Event);
void runMapEditor(GameMap*);
