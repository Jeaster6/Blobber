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
            AnimationDuration,
            AnimationFrames,
            LevelOfDetail,
			UnrecognizedVariable
		};

		static Configuration configuration;
		float screenWidth = 1920.0f;
		float screenHeight = 1080.0f;
		float fieldOfView = 0.65f;
        int animationDuration = 16;
        int animationFrames = 200;
        int levelOfDetail = 8;

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
        int getAnimationDuration();
        int getAnimationFrames();
        int getLevelOfDetail();

		void setScreenWidth(float);
		void setScreenHeight(float);
		void setFieldOfView(float);
        void setAnimationDuration(int);
        void setAnimationFrames(int);
        void setLevelOfDetail(int);
};
