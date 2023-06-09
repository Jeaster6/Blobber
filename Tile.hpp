#pragma once

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "MapObject.hpp"
#include "Direction.hpp"

class Tile {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& N;
            ar& E;
            ar& S;
            ar& W;
            ar& mapObject;
        }

        bool N;
        bool E;
        bool S;
        bool W;
        MapObject mapObject;

    public:
        void setWall(char, bool);
        void setTile(bool, bool, bool, bool);
        void setTile(Tile*);
        void spawnObject(std::string);
        void deSpawnObject();
        bool isWalled(Direction);
        bool containsObject();

        Tile(bool, bool, bool, bool, MapObject);
        Tile();
        ~Tile();
};
