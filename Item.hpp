#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include "CurrentDirectory.hpp"

class Item {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& itemID;
        }

        std::string itemID;
        std::string itemName;

        void loadItemData();

    public:
        Item(const std::string&);
        Item();
        ~Item();

};
