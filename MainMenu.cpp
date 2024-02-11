#include "MainMenu.hpp"

void renderMainMenu() {
	Graphics::getInstance().renderMenuTexture("Menu.png");
}

void openMainMenu() {
    Graphics::getInstance().init();
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
						gameplay("");
						break;

					case SDLK_l:
						gameplay(getSavedGameFile());
						break;

					case SDLK_s:
						openSettings();
						break;
				}
			}
		}
	}
}

std::string getSavedGameFile() {
    return "quick.sav";
}
