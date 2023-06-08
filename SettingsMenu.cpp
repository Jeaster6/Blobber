#include "SettingsMenu.hpp"

void openSettings() {
	bool quit = false;
	SDL_Event userInput;
	while (!quit) {

		Graphics::getInstance().renderSettings();

		while (SDL_PollEvent(&userInput)) {

			if (userInput.type == SDL_QUIT) {
				exit(0);
			}

			if (userInput.type == SDL_KEYDOWN) {

				switch (userInput.key.keysym.sym) {

				case SDLK_ESCAPE:
					quit = true;
					break;

				case SDLK_RETURN:
					quit = true;
					break;
				}
			}
		}
	}
}