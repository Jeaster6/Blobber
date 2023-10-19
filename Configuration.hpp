#pragma once
#define SDL_MAIN_HANDLED

#include <fstream>
#include <sstream>
#include <filesystem>
#include "SDL.h"
#include "CurrentDirectory.hpp"

class Configuration {

	private:

		enum ConfigVariable {
			ScreenWidth,
			ScreenHeight,
			FieldOfView,
			UnrecognizedVariable
		};

		static Configuration configuration;
		float screenWidth = 1920;
		float screenHeight = 1080;
		float fieldOfView = 0.65f;

		Configuration();
		~Configuration();

		ConfigVariable stringToEnum(const std::string&);
		void saveChangesToFile();
		void loadDataFromFile();

	public:
		Configuration(const Configuration&) = delete;
		void operator = (const Configuration&) = delete;

		static Configuration& getInstance();
		float getScreenWidth();
		float getScreenHeight();
		float getFieldOfView();

		void setScreenWidth(float);
		void setScreenHeight(float);
		void setFieldOfView(float);
};