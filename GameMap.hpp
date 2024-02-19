#pragma once
#define SDL_MAIN_HANDLED

#include <vector>
#include <unordered_set>
#include <map>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include "SDL.h"
#include "Graphics.hpp"
#include "Direction.hpp"
#include "Player.hpp"
#include "Tile.hpp"

class GameMap {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& width;
            ar& height;
            ar& savedTiles;
            init();
        }

        int width;
        int height;
        std::shared_ptr<std::shared_ptr<Tile[]>[]> map;
        std::vector<Tile> savedTiles;
        std::map<std::string, SDL_Texture*> textures;
        SDL_Texture* previousScreenTexture;
        SDL_Texture* currentScreenTexture;

        void loadFromVector();
        void loadTextures();
        void generateScreenTexture(std::shared_ptr<Player>, SDL_Texture*);
        void generateScreenTexture(std::shared_ptr<Player>, SDL_Texture*, float);
        bool isTextureInView(std::array<std::pair<float, float>, 4>);
        void init();

    public:
        int getHeight() const;
        int getWidth() const;
        Tile& getTile(int, int);
        void setTileWall(int, int, Direction, const std::string&);
        void setFloorType(int, int, const std::string&);
        void setCeilingType(int, int, const std::string&);
        void setTile(int, int, Tile);
        void setTileParameters(int, int, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, std::shared_ptr<MapObject>, std::shared_ptr<MapTrigger>, bool);
        void saveToVector();
        void renderVisibleArea(std::shared_ptr<Player>);
        void animateLeftRotation(std::shared_ptr<Player>);
        void animateRightRotation(std::shared_ptr<Player>);
        void animateForwardMovement(std::shared_ptr<Player>);
        void animateBackwardMovement(std::shared_ptr<Player>);
        void animateSidestepLeft(std::shared_ptr<Player>);
        void animateSidestepRight(std::shared_ptr<Player>);
        void makeScreenSnapshot(std::shared_ptr<Player>);

        GameMap();
        GameMap(int, int);
        GameMap(int, int, std::vector<Tile>);
        ~GameMap();
};
