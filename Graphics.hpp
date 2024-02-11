#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <array>
#include "Configuration.hpp"
#include "SettingsMenu.hpp"
#include "CurrentDirectory.hpp"

class Graphics {

	private:
		static Graphics graphics;
		float screenWidth;
		float screenHeight;
		float fieldOfView;
        int animationFrames;
        int animationDuration;
        int levelOfDetail;
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
		float getScreenWidth() const;
		float getScreenHeight() const;
		float getFOV() const;
        int getAnimationFrames() const;
        int getAnimationDuration() const;
		void init();
		void renderMenuTexture(const std::string&);
		void renderTextureUsingVertices(SDL_Texture*, const std::array<std::pair<float, float>, 4>&, int);
};
