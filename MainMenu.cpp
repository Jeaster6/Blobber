#include "MainMenu.hpp"

void openMainMenu() {
	bool quit = false;
	SDL_Event userInput;
	while (!quit) {

		Graphics::getInstance().renderMainMenu();

		while (SDL_PollEvent(&userInput)) {

			if (userInput.type == SDL_QUIT) {
				exit(0);
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