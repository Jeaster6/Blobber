#include "MainMenu.hpp"

void openMainMenu() {
    Graphics::getInstance().init();
	bool quit = false;
	SDL_Event userInput;

	while (!quit) {
        Graphics::getInstance().renderTexture("Menu.png", nullptr);

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

static std::string getSavedGameFile() {
    return "quick.sav";
}
