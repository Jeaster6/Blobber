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
            ar& triggered;
        }

        std::string ID;
        std::vector <Item> objectContents;
        std::string type;
        bool triggered;

    public:
        void addItems(std::vector <std::string>);
        void removeItems();
        bool exists();
        void triggerObject();
        std::string getObjectType() const;

        MapObject(std::string);
        MapObject();
        ~MapObject();
};
