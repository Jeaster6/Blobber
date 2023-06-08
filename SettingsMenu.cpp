#include "SettingsMenu.hpp"

void openSettings() {
	bool quit = false;
	int screenWidth = Configuration::getInstance().getScreenWidth();
	int screenHeight = Configuration::getInstance().getScreenHeight();
	double fieldOfView = Configuration::getInstance().getFieldOfView();

	SDL_Event userInput;

	while (!quit) {
		Graphics::getInstance().renderSettings();

		while (SDL_PollEvent(&userInput)) {
			if (userInput.type == SDL_QUIT) {
				quit = true;
			}

			if (userInput.type == SDL_KEYDOWN) {
				switch (userInput.key.keysym.sym) {

				case SDLK_ESCAPE:
					quit = true;
					break;

				case SDLK_1:
					screenWidth = 1600;
					screenHeight = 900;
					break;

				case SDLK_2:
					screenWidth = 1920;
					screenHeight = 1080;
					break;

				case SDLK_3:
					screenWidth = 2560;
					screenHeight = 1440;
					break;

				case SDLK_RETURN:
					quit = true;

					Configuration::getInstance().setScreenWidth(screenWidth);
					Configuration::getInstance().setScreenHeight(screenHeight);
					Configuration::getInstance().setFieldOfView(fieldOfView);

					Graphics::getInstance().refreshSettings();
					break;
				}
			}
		}
	}
}