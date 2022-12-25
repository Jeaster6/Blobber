#pragma once

#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "Item.hpp"

class MapObject {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& objectID;
            ar& objectContents;
        }

        std::string objectID;
        std::vector <Item> objectContents;

    public:
        void addItems(std::vector <std::string>);
        void removeItems();
        bool exists();

        MapObject(std::string);
        MapObject();
        ~MapObject();
};
