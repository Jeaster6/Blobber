#include "SettingsMenu.hpp"

enum ConfigVariable {
	ScreenWidth,
	ScreenHeight,
	FieldOfView,
	UnrecognizedVariable
};

void openSettings() {
	bool quit = false;
	int screenWidth = 0;
	int screenHeight = 0;
	double fieldOfView = 0;
	loadConfigFileData(screenWidth, screenHeight, fieldOfView);
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
					saveChanges(screenWidth, screenHeight, fieldOfView);
					Graphics::getInstance().refreshSettings();
					break;
				}
			}
		}
	}
}

ConfigVariable stringToVariable(const std::string& parameterName) {
	if (parameterName == "screenWidth") return ScreenWidth;
	if (parameterName == "screenHeight") return ScreenHeight;
	if (parameterName == "fieldOfView") return FieldOfView;
	return UnrecognizedVariable;
}

void loadConfigFileData(int& screenWidth, int& screenHeight, double& fieldOfView) {
	std::ifstream configFile(getRootDirectory() + "config.cfg");
	ConfigVariable parameterName;
	std::string parameterValue;
	std::string line = "";
	while (std::getline(configFile, line)) {
		std::string::size_type pos = std::string(line).find_last_of("=");
		parameterName = stringToVariable(std::string(line).substr(0, pos));
		parameterValue = std::string(line).substr(pos + 1);
		switch (parameterName) {
			case ScreenWidth:
				screenWidth = std::stoi(parameterValue);
				break;
			case ScreenHeight:
				screenHeight = std::stoi(parameterValue);
				break;
			case FieldOfView:
				fieldOfView = std::stod(parameterValue);
				break;
			case UnrecognizedVariable:
				break;
		}
	}
	configFile.close();
}

void saveChanges(int screenWidth, int screenHeight, double fieldOfView) {
	std::ofstream configFile(getRootDirectory() + "config.cfg");
	screenWidth = 2560;
	screenHeight = 1440;
	fieldOfView = 0.65;
	configFile << "screenWidth=" + std::to_string(screenWidth) + "\n";
	configFile << "screenHeight=" + std::to_string(screenHeight) + "\n";
	configFile << "fieldOfView=" + std::to_string(fieldOfView) + "\n";
	configFile.close();
}