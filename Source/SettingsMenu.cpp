#include "SettingsMenu.hpp"

void renderSettingsMenu() {
	Graphics::getInstance().renderMainMenu("Settings.png");
}

bool openSettings() {
	bool quit = false;
    bool ALTF4 = false;
	int screenWidth = Configuration::getInstance().getScreenWidth();
    int screenHeight = Configuration::getInstance().getScreenHeight();
	float fieldOfView = Configuration::getInstance().getFieldOfView();

	SDL_Event userInput;

	while (!quit && !ALTF4) {
		renderSettingsMenu();

		while (SDL_PollEvent(&userInput)) {
			if (userInput.type == SDL_QUIT) {
				quit = true;
                ALTF4 = true;
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

						Graphics::getInstance().init();
						break;
				}
			}
		}
	}

    return ALTF4;
}
