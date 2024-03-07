#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <boost/filesystem.hpp>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "CurrentDirectory.hpp"
#include "Direction.hpp"
#include "GameMap.hpp"

const int SCREEN_WIDTH=2560;
const int SCREEN_HEIGHT=1440;
const int GRID_TILE_SIZE=40;
const int TILE_SIZE=(int)(GRID_TILE_SIZE*0.96);
const int WALL_THICKNESS=(int)(GRID_TILE_SIZE*0.16);

struct pathLeafString {
    std::string operator() (const boost::filesystem::directory_entry& entry) const {
        return entry.path().leaf().string();
    }
};

void processMouseAction(GameMap&, const SDL_Event&, int, SDL_Renderer*, SDL_Texture*, const std::string&);
void runMapEditor(GameMap&);
std::vector<std::pair<std::string, SDL_Texture*>> loadTexturesFromDirectory(SDL_Renderer*);
