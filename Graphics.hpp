#pragma once
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <array>
#include <map>
#include <unordered_set>
#include "FontProvider.hpp"
#include "Direction.hpp"
#include "GameMap.hpp"
#include "Player.hpp"
#include "Configuration.hpp"
#include "SettingsMenu.hpp"
#include "CurrentDirectory.hpp"

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
		SDL_Window* window;
		SDL_Renderer* renderer;
        SDL_Texture* previousScreenTexture;
        SDL_Texture* currentScreenTexture;
        SDL_Texture* UIOverlayTexture;
        FontProvider fontProvider;
        std::map<std::string, SDL_Texture*> currentMapTextures;

		Graphics();
		~Graphics();
        void generateScreenTexture(const GameMap&, const Player&, SDL_Texture*);
        void generateScreenTexture(const GameMap&, const Player&, SDL_Texture*, float);
        void renderTextureUsingVertices(SDL_Texture*, const std::array<std::pair<float, float>, 4>&, int);
        bool isTextureInView(const std::array<std::pair<float, float>, 4>&);
        void renderUIOverlay();
        int playerDistanceFromMapEdge(const GameMap&, const Player&, Direction);

	public:
		Graphics(const Graphics&) = delete;
		void operator = (const Graphics&) = delete;
		static Graphics& getInstance();

        void init();
        void loadMapTextures(const GameMap&);
		void renderMainMenu(const SDL_Rect*, const std::string&); // TODO: redo main menu to render background texture with standard UI elements, as well as settings menu being a UI element
        void renderUIElement(const SDL_Rect*, const std::string&);
        void renderTextMessage(int, int, const std::string&, int, int, int, int); // render text message at given coordinates with a specified font size, target area width and heigh has to be the same as the generated texture, otherwise it gets streched
        void animateLeftRotation(const GameMap&, const Player&);
        void animateRightRotation(const GameMap&, const Player&);
        void animateForwardMovement(const GameMap&, const Player&);
        void animateBackwardMovement(const GameMap&, const Player&);
        void animateSidestepLeft(const GameMap&, const Player&);
        void animateSidestepRight(const GameMap&, const Player&);
        void renderPlayerView(const GameMap&, const Player&);
        void clearUI();
};
