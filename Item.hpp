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
            ar& ID;
            loadItemData();
        }

        std::string ID;
        std::string name;

        void loadItemData();

    public:
        Item(const std::string&);
        Item();
        ~Item();

        std::string getID() const;
        std::string getName() const;
};
