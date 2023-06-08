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
	return UnrecognizedVariable;
}

void Configuration::loadDataFromFile() {
	std::ifstream configFile;
	configFile.open(getRootDirectory() + "config.cfg");

//if file doesn't exist, create populate it with default values

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

void Configuration::saveChangesToFile() {
	std::ofstream configFile;
	configFile.open(getRootDirectory() + "config.cfg");
	configFile << "screenWidth=" + std::to_string(screenWidth) + "\n";
	configFile << "screenHeight=" + std::to_string(screenHeight) + "\n";
	configFile << "fieldOfView=" + std::to_string(fieldOfView) + "\n";
	configFile.close();
}

int Configuration::getScreenWidth() {
	return screenWidth;
}

int Configuration::getScreenHeight() {
	return screenHeight;
}

double Configuration::getFieldOfView() {
	return fieldOfView;
}

void Configuration::setScreenWidth(int screenWidth) {
	this->screenWidth = screenWidth;
	saveChangesToFile();
}

void Configuration::setScreenHeight(int screenHeight) {
	this->screenHeight = screenHeight;
	saveChangesToFile();
}

void Configuration::setFieldOfView(double fieldOfView) {
	this->fieldOfView = fieldOfView;
	saveChangesToFile();
}

Configuration Configuration::configuration;