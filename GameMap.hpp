#pragma once

#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "Direction.hpp"
#include "Tile.hpp"

class GameMap;
namespace boost {
    namespace serialization {
        template<class Archive>
        inline void save_construct_data(Archive& ar, const GameMap* map, const unsigned int file_version);
    }
}

class GameMap {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {}
        template<class Archive> friend void boost::serialization::save_construct_data(Archive& ar, const GameMap* map, const unsigned int version);

        int width;
        int height;
        Tile** map;
        std::vector <Tile> savedTiles;

        void loadFromVector();

    public:
        Tile* getTile(int, int);
        int getHeight();
        int getWidth();
        void setTileWall(int, int, Direction, bool);
        void setTile(int, int, Tile*);
        void setTile(int, int, bool, bool, bool, bool);
        void saveToVector();

        GameMap(int, int);
        GameMap(int, int, std::vector <Tile>);
        ~GameMap();
};

namespace boost {
    namespace serialization {

        template<class Archive> inline void save_construct_data(Archive& ar, const GameMap* map, const unsigned int version) {
            ar << map->width;
            ar << map->height;
            ar << map->savedTiles;
        }

        template<class Archive> inline void load_construct_data(Archive& ar, GameMap* map, const unsigned int version) {
            int width ;
            int height;
            std::vector <Tile> savedTiles;

            ar >> width;
            ar >> height;
            ar >> savedTiles;

            new(map)GameMap(width, height, savedTiles);
        }
    }
}
