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
        void generateScreenTexture(const Player&, SDL_Texture*);
        void generateScreenTexture(const Player&, SDL_Texture*, float);
        bool isTextureInView(const std::array<std::pair<float, float>, 4>&);
        void init(); 

    public:
        int getHeight() const;
        int getWidth() const;
        const Tile& getTile(int, int) const;
        void setTileWall(int, int, Direction, const std::string&);
        void setFloorType(int, int, const std::string&);
        void setCeilingType(int, int, const std::string&);
        void setTile(int, int, const Tile&);
        void setTileParameters(int, int, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const MapObject&, const MapTrigger&, bool);
        void saveToVector();
        void renderVisibleArea(const Player&);
        void animateLeftRotation(const Player&);
        void animateRightRotation(const Player&);
        void animateForwardMovement(const Player&);
        void animateBackwardMovement(const Player&);
        void animateSidestepLeft(const Player&);
        void animateSidestepRight(const Player&);
        void makeScreenSnapshot(const Player&);
        void saveToFile(const std::string&);
        void loadFromFile(const std::string&);
        void placeObject(int, int, const std::string&);
        void triggerObject(int, int);
        void removeObject(int, int);
        void placeTrigger(int, int, TriggerType, const std::string&, bool);
        void removeTrigger(int, int);
        void markTileAsExplored(int, int);

        GameMap();
        GameMap(int, int);
        GameMap(int, int, const std::vector<Tile>&);
        ~GameMap();
};
