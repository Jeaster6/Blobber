#include "CurrentDirectory.hpp"

std::string getRootDirectory() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos) + "/";
}

std::string getMapsDirectory() {
	return getRootDirectory() + "Maps/";
}

std::string getUITexturesDirectory() {
	return getRootDirectory() + "Textures/UI/";
}

std::string getObjectTexturesDirectory() {
    return getRootDirectory() + "Textures/Objects/";
}

std::string getEnvironmentTexturesDirectory() {
    return getRootDirectory() + "Textures/Environment/";
}

std::string getDataDirectory() {
	return getRootDirectory() + "Data/";
}

std::string getSaveFileDirectory() {
    return getRootDirectory() + "Save/";
}
