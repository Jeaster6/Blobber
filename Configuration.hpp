#pragma once
#define SDL_MAIN_HANDLED

#include <fstream>
#include <sstream>
#include <filesystem>
#include "SDL.h"
#include "CurrentDirectory.hpp"

class Configuration {

	enum ConfigVariable {
		ScreenWidth,
		ScreenHeight,
		FieldOfView,
		UnrecognizedVariable
	};

	public:
		Configuration(const Configuration&) = delete;
		void operator = (const Configuration&) = delete;

		static Configuration& getInstance();
		int getScreenWidth();
		int getScreenHeight();
		double getFieldOfView();

		void setScreenWidth(int);
		void setScreenHeight(int);
		void setFieldOfView(double);

	private:
		static Configuration configuration;
		int screenWidth = 1920;
		int screenHeight = 1080;
		double fieldOfView = 0.65;

		Configuration();
		~Configuration(); 
		
		ConfigVariable stringToEnum(const std::string&);
		void saveChangesToFile();
		void loadDataFromFile();
};