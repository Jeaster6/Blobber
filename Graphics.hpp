#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "Configuration.hpp"
#include "SettingsMenu.hpp"
#include "CurrentDirectory.hpp"

class Graphics {

	private:
		static Graphics graphics;
		int screenWidth;
		int screenHeight;
		double fieldOfView;
		SDL_Window* gameWindow;
		SDL_Renderer* gameRenderer;
		SDL_Surface* surface;
		SDL_Texture* texture;

		Graphics();
		~Graphics();

	public:
		Graphics(const Graphics&) = delete;
		void operator = (const Graphics&) = delete;

		static Graphics& getInstance();
		SDL_Renderer* getRenderer();
		int getScreenWidth();
		int getScreenHeight();
		double getFOV();
		void refreshSettings();
		void renderFullscreenTexture(const std::string&);
};