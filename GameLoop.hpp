#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <iostream>
#include "CurrentDirectory.hpp"
#include "Graphics.hpp"
#include "GameState.hpp"

void gameplay(const std::string&);
static std::shared_ptr<GameState> loadGame(const std::string&);
static void quickSave(std::shared_ptr<GameState>, const std::string&);
