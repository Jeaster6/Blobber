#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "CurrentDirectory.hpp"

class Graphics {

	public:
		Graphics(const Graphics&) = delete;
		void operator = (const Graphics&) = delete;
		static Graphics& getInstance();
		void renderMainMenu();
		void renderSettings();
		SDL_Renderer* getRenderer();
		int getScreenWidth();
		int getScreenHeight();
		double getFOV();

	private:
		int screenWidth = 2560;
		int screenHeight = 1440;
		double fieldOfView = 0.65;
		Graphics();
		~Graphics();
		void renderMenuScreen(std::string);
		static Graphics graphics;
		SDL_Window* gameWindow;
		SDL_Renderer* gameRenderer;
		SDL_Surface* surface;
		SDL_Texture* texture;
};