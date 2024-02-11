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
        bool explored;

    public:
        Tile(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, MapObject*, bool);
        Tile();
        ~Tile();

        void setWall(Direction, const std::string&);
        void setFloorType(const std::string&);
        void setCeilingType(const std::string&);
        void setTile(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, MapObject*, bool);
        void setTile(Tile*);
        void spawnObject(const std::string&);
        void deSpawnObject();
        void markAsExplored();

        bool isExplored() const;
        bool isWalled(Direction) const;
        bool hasFloor() const;
        bool hasCeiling() const;
        bool containsObject() const;
        bool isFullyWalled() const;
        std::string getWallType(Direction) const;
        std::string getFloorType() const;
        std::string getCeilingType() const;
        MapObject* getObject() const;
        std::unordered_set <std::string> getTextures() const;
};
