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
            ar& contents;
            ar& triggered;
            ar& type;
        }

        std::string ID;
        std::vector<Item> contents;
        std::string type;
        bool triggered;

    public:
        void addItems(const std::vector<std::string>&);
        void removeItem(int);
        void removeItem(const std::string&);
        void triggerObject();
        void setObjectData(const MapObject&);
        void removeObject();

        std::string getType() const;
        std::vector<Item> getContents() const;

        MapObject(const std::string&);
        MapObject();
        ~MapObject();
};
