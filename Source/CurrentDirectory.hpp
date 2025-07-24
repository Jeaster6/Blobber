#pragma once
#define NOMINMAX

#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <boost/filesystem.hpp>

struct pathLeafString {
    std::string operator() (const boost::filesystem::directory_entry& entry) const {
        return entry.path().filename().string();
    }
};

std::string getRootDirectory();
std::string getMapsDirectory();
std::string getUITexturesDirectory();
std::string getObjectTexturesDirectory();
std::string getEnvironmentTexturesDirectory();
std::string getDataDirectory();
std::string getSaveFileDirectory();
std::vector<std::string> readMapsFromDirectory();
