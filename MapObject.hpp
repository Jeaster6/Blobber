#pragma once

#include <vector>
#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "Item.hpp"

class MapObject {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& ID;
            ar& objectContents;
        }

        std::string ID;
        std::vector <Item> objectContents;
        std::string type;

    public:
        void addItems(std::vector <std::string>);
        void removeItems();
        bool exists();
        const std::string& getObjectType();

        MapObject(std::string);
        MapObject();
        ~MapObject();
};
