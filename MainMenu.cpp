#include "MainMenu.hpp"

void renderMainMenu() {
	Graphics::getInstance().renderFullscreenTexture("Menu.png");
}

void openMainMenu() {
	bool quit = false;
	SDL_Event userInput;

	while (!quit) {
		renderMainMenu();

		while (SDL_PollEvent(&userInput)) {
			if (userInput.type == SDL_QUIT) {
				quit = true;
			}

			if (userInput.type == SDL_KEYDOWN) {

				switch (userInput.key.keysym.sym) {

					case SDLK_ESCAPE:
						quit = true;
						break;

					case SDLK_n:
						gameplay();
						break;

					case SDLK_l:
						gameplay();
						break;

					case SDLK_s:
						openSettings();
						break;
				}
			}
		}
	}
}