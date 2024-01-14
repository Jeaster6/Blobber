#include "Configuration.hpp"

Configuration& Configuration::getInstance() {
	return configuration;
}

Configuration::Configuration() {
	loadDataFromFile();
}

Configuration::~Configuration() {
}

Configuration::ConfigVariable Configuration::stringToEnum(const std::string& parameterName) {
	if (parameterName == "screenWidth") return ScreenWidth;
	if (parameterName == "screenHeight") return ScreenHeight;
	if (parameterName == "fieldOfView") return FieldOfView;
    if (parameterName == "animationDuration") return AnimationDuration;
    if (parameterName == "animationFrames") return AnimationFrames;
    if (parameterName == "levelOfDetail") return LevelOfDetail;
	return UnrecognizedVariable;
}

void Configuration::loadDataFromFile() {
	std::ifstream configFile;
	configFile.open(getRootDirectory() + "config.cfg");

//if file doesn't exist, create and populate it with default values

	if (!configFile || configFile.peek() == std::ifstream::traits_type::eof()) {
		saveChangesToFile();
	}

	ConfigVariable parameterName;
	std::string parameterValue;
	std::string line = "";

	while (std::getline(configFile, line)) {
		std::string::size_type pos = std::string(line).find_last_of("=");
		parameterName = stringToEnum(std::string(line).substr(0, pos));
		parameterValue = std::string(line).substr(pos + 1);

		switch (parameterName) {

		case ScreenWidth:
			screenWidth = std::stof(parameterValue);
			break;

		case ScreenHeight:
			screenHeight = std::stof(parameterValue);
			break;

		case FieldOfView:
			fieldOfView = std::stof(parameterValue);
			break;

        case AnimationDuration:
            animationDuration = (int)std::stof(parameterValue);
            break;

        case AnimationFrames:
            animationFrames = (int)std::stof(parameterValue);
            break;

        case LevelOfDetail:
            levelOfDetail = (int)std::stof(parameterValue);
            break;

		case UnrecognizedVariable:
			break;
		}
	}
	configFile.close();
}

void Configuration::saveChangesToFile() {
	std::ofstream configFile;
	configFile.open(getRootDirectory() + "config.cfg");
	configFile << "screenWidth=" + std::to_string(screenWidth) + "\n";
	configFile << "screenHeight=" + std::to_string(screenHeight) + "\n";
	configFile << "fieldOfView=" + std::to_string(fieldOfView) + "\n";
    configFile << "animationDuration=" + std::to_string(animationDuration) + "\n";
    configFile << "animationFrames=" + std::to_string(animationFrames) + "\n";
    configFile << "levelOfDetail=" + std::to_string(levelOfDetail) + "\n";
	configFile.close();
}

float Configuration::getScreenWidth() {
	return screenWidth;
}

float Configuration::getScreenHeight() {
	return screenHeight;
}

float Configuration::getFieldOfView() {
	return fieldOfView;
}

int Configuration::getAnimationDuration() {
    return animationDuration;
}

int Configuration::getAnimationFrames() {
    return animationFrames;
}

int Configuration::getLevelOfDetail() {
    return levelOfDetail;
}

void Configuration::setScreenWidth(float screenWidth) {
	this->screenWidth = screenWidth;
	saveChangesToFile();
}

void Configuration::setScreenHeight(float screenHeight) {
	this->screenHeight = screenHeight;
	saveChangesToFile();
}

void Configuration::setFieldOfView(float fieldOfView) {
	this->fieldOfView = fieldOfView;
	saveChangesToFile();
}

void Configuration::setAnimationDuration(int animationDuration) {
    this->animationDuration = animationDuration;
    saveChangesToFile();
}

void Configuration::setAnimationFrames(int animationFrames) {
    this->animationFrames = animationFrames;
    saveChangesToFile();
}

void Configuration::setLevelOfDetail(int levelOfDetail) {
    this->levelOfDetail = levelOfDetail;
    saveChangesToFile();
}

Configuration Configuration::configuration;
