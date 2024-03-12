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
            ar& type;
        }

        std::string ID;
        std::vector<Item> objectContents;
        std::string type;
        bool triggered;

    public:
        void addItems(const std::vector<std::string>&);
        void removeItems();
        void triggerObject();
        void setObjectData(const MapObject&);
        void removeObject();

        const std::string& getType() const;

        MapObject(const std::string&);
        MapObject();
        ~MapObject();
};
