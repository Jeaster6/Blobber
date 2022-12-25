#pragma once

#include <fstream>
#include <iostream>
#include <boost/serialization/vector.hpp>
#include "Tile.hpp"

class GameMap;
namespace boost {
    namespace serialization {
        template<class Archive>
        inline void save_construct_data(Archive& ar, const GameMap* gameMap, const unsigned int file_version);
    }
}

class GameMap {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {}
        template<class Archive> friend void boost::serialization::save_construct_data(Archive& ar, const GameMap* gameMap, const unsigned int version);

        int mapHeight;
        int mapWidth;
        Tile** gameMap;
        std::vector <Tile> savedTiles;

        void loadFromVector();

    public:
        Tile* getTile(int, int);
        const int getHeight();
        const int getWidth();
        void setTileWall(int, int, char, bool);
        void setTile(int, int, Tile*);
        void setTile(int, int, bool, bool, bool, bool);
        void saveToVector();

        GameMap(int, int);
        GameMap(int, int, std::vector <Tile>);
        ~GameMap();
};

namespace boost {
    namespace serialization {

        template<class Archive> inline void save_construct_data(Archive& ar, const GameMap* gameMap, const unsigned int version) {
            ar << gameMap->mapWidth;
            ar << gameMap->mapHeight;
            ar << gameMap->savedTiles;
        }

        template<class Archive> inline void load_construct_data(Archive& ar, GameMap* gameMap, const unsigned int version) {
            int mapHeight;
            int mapWidth;
            std::vector <Tile> savedTiles;

            ar >> mapWidth;
            ar >> mapHeight;
            ar >> savedTiles;

            new(gameMap)GameMap(mapWidth, mapHeight, savedTiles);
        }
    }
}
