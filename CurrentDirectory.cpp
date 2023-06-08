#include "CurrentDirectory.hpp"

std::string getRootDirectory() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos) + "/";
}

std::string getMapsDirectory() {
	return getRootDirectory() + "Maps/";
}

std::string getTexturesDirectory() {
	return getRootDirectory() + "Textures/";
}

std::string getDataDirectory() {
	return getRootDirectory() + "Data/";
}