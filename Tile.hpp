#pragma once

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <string>
#include <unordered_set>
#include "MapObject.hpp"
#include "Direction.hpp"

class Tile {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& northWallType;
            ar& eastWallType;
            ar& southWallType;
            ar& westWallType;
            ar& floorType;
            ar& ceilingType;
            ar& mapObject;
        }

        std::string northWallType;
        std::string eastWallType;
        std::string southWallType;
        std::string westWallType;
        std::string floorType;
        std::string ceilingType;
        MapObject* mapObject;
        std::unordered_set <std::string> textures;

    public:
        void setWall(Direction, const std::string&);
        void setTile(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, MapObject*);
        void setTile(Tile*);
        void spawnObject(std::string);
        void deSpawnObject();
        bool isWalled(Direction);
        bool hasFloor();
        bool hasCeiling();
        bool containsObject();
        const std::string& getWallType(Direction);
        const std::string& getFloorType();
        const std::string& getCeilingType();
        const std::string& getObjectType();
        std::unordered_set <std::string> getTextures();

        Tile(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, MapObject*);
        Tile();
        ~Tile();
};
