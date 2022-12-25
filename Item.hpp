#pragma once

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
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
        Item(std::string);
        Item();
        ~Item();

};
