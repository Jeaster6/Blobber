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
		int screenWidth;
		int screenHeight;
		float fieldOfView;
        int animationDuration;
        int animationFrames;
        int levelOfDetail;

		Configuration();
		~Configuration();

		ConfigVariable stringToEnum(const std::string&);
		void saveChangesToFile();
		void loadDataFromFile();

	public:
		Configuration(const Configuration&) = delete;
		void operator = (const Configuration&) = delete;

		static Configuration& getInstance();
        int getScreenWidth() const;
        int getScreenHeight() const;
		float getFieldOfView() const;
        int getAnimationDuration() const;
        int getAnimationFrames() const;
        int getLevelOfDetail() const;

        void setScreenWidth(int);
        void setScreenHeight(int);
		void setFieldOfView(float);
        void setAnimationDuration(int);
        void setAnimationFrames(int);
        void setLevelOfDetail(int);
};
