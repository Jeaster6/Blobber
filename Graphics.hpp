#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "SettingsMenu.hpp"
#include "CurrentDirectory.hpp"

class Graphics {

	public:
		Graphics(const Graphics&) = delete;
		void operator = (const Graphics&) = delete;
		static Graphics& getInstance();
		void renderMainMenu();
		void renderSettings();
		void refreshSettings();
		SDL_Renderer* getRenderer();
		int getScreenWidth();
		int getScreenHeight();
		double getFOV();

	private:
		int screenWidth = 1920;
		int screenHeight = 1080;
		double fieldOfView = 0.65;
		Graphics();
		~Graphics();
		void renderMenuScreen(const std::string&);
		static Graphics graphics;
		SDL_Window* gameWindow;
		SDL_Renderer* gameRenderer;
		SDL_Surface* surface;
		SDL_Texture* texture;
};