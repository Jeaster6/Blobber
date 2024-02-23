#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <array>
#include <map>
#include <unordered_set>
#include "Configuration.hpp"
#include "SettingsMenu.hpp"
#include "CurrentDirectory.hpp"
#include "GameState.hpp"

class Graphics {

	private:
		static Graphics graphics;
        int screenWidth;
        int screenHeight;
        int gameWidth;
		float fieldOfView;
        int animationFrames;
        int animationDuration;
        int levelOfDetail;
		SDL_Window* gameWindow;
		SDL_Renderer* gameRenderer;
		SDL_Surface* surface;
        SDL_Texture* previousScreenTexture;
        SDL_Texture* currentScreenTexture;
        std::map<std::string, SDL_Texture*> currentMapTextures;

		Graphics();
		~Graphics();
        void generateScreenTexture(const GameState&, SDL_Texture*);
        void generateScreenTexture(const GameState&, SDL_Texture*, float);
        void renderTextureUsingVertices(SDL_Texture*, const std::array<std::pair<float, float>, 4>&, int);
        bool isTextureInView(const std::array<std::pair<float, float>, 4>&);
        void renderBackground();
        void renderSideBars();
        int playerDistanceFromMapEdge(const GameState&, Direction);

	public:
		Graphics(const Graphics&) = delete;
		void operator = (const Graphics&) = delete;
		static Graphics& getInstance();

		void init();
        void loadMapTextures(const GameMap&);
		void renderMenuTexture(const std::string&);
        void animateLeftRotation(const GameState&);
        void animateRightRotation(const GameState&);
        void animateForwardMovement(const GameState&);
        void animateBackwardMovement(const GameState&);
        void animateSidestepLeft(const GameState&);
        void animateSidestepRight(const GameState&);
        void renderPlayerView(const GameState&);
};
