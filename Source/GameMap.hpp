#pragma once
#define SDL_MAIN_HANDLED

#include <vector>
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "SDL.h"
#include "Direction.hpp"
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

        void loadFromVector();
        void init(); 

    public:
        int getHeight() const;
        int getWidth() const;
        Tile getTile(int, int) const;

        void setTileWall(int, int, Direction, const std::string&);
        void setFloorType(int, int, const std::string&);
        void setCeilingType(int, int, const std::string&);
        void setTile(int, int, const Tile&);
        void setTileParameters(int, int, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const MapObject&, const MapTrigger&, bool);
        void saveToVector();
        void saveToFile(const std::string&);
        void loadFromFile(const std::string&);
        void placeObject(int, int, const std::string&);
        void triggerObject(int, int);
        void removeObject(int, int);
        void placeTrigger(int, int, TriggerType, const std::string&, bool);
        void placeTrigger(int, int, TriggerType, const std::string&, int, int, bool);
        void activateTrigger(int, int);
        void removeTrigger(int, int);
        void markTileAsExplored(int, int);
        void addItemToObject(int, int, const std::string&);
        void removeItemFromObject(int, int, int);
        void removeItemFromObject(int, int, const std::string&);

        GameMap();
        GameMap(int, int);
        GameMap(int, int, const std::vector<Tile>&);
        ~GameMap();
};
