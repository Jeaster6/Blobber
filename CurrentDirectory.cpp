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

std::vector<std::string> readMapsFromDirectory() {
    std::vector<std::string> allFiles;
    std::vector<std::string> mapFiles;
    boost::filesystem::directory_iterator start(getMapsDirectory());
    boost::filesystem::directory_iterator end;
    transform(start, end, back_inserter(allFiles), pathLeafString());

    mapFiles.clear();
    for (unsigned int i = 0; i < allFiles.size(); i++) {
        if (allFiles[i].substr(allFiles[i].length() - 4) == ".dat") {
            mapFiles.push_back(allFiles[i]);
        }
    }

    return mapFiles;
}
