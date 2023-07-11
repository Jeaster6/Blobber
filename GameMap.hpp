#pragma once
#define SDL_MAIN_HANDLED

#include <vector>
#include <unordered_set>
#include <map>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "SDL.h"
#include "Graphics.hpp"
#include "Direction.hpp"
#include "Player.hpp"
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
        std::map <std::string, SDL_Texture*> textures;
        SDL_Texture* previousScreenTexture;
        SDL_Texture* currentScreenTexture;

        void loadFromVector();
        void loadTextures();
        void generateScreenTexture(Player, SDL_Texture*);

    public:
        Tile* getTile(int, int);
        int getHeight();
        int getWidth();
        void setTileWall(int, int, Direction, const std::string&);
        void setTile(int, int, Tile*);
        void setTileWalls(int, int, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, MapObject*);
        void saveToVector();
        void renderVisibleArea(Player);
        void animateLeftRotation(Player);
        void animateRightRotation(Player);
        void animateForwardMovement(Player);
        void animateBackwardMovement(Player);
        void animateSidestepLeft(Player);
        void animateSidestepRight(Player);
        void makeScreenSnapshot(Player);

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
