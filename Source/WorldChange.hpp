#pragma once

#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

enum ChangeType {
    ItemAddedToObject,
    ItemRemovedFromObject,
    ObjectTriggered,
    TriggerActivated,
    TileExplored
};

class WorldChange {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& affectedMapName;
            ar& locationX;
            ar& locationY;
            ar& changeType;
            ar& changeSubject;
        }

        std::string affectedMapName;
        int locationX;
        int locationY;
        ChangeType changeType;
        std::string changeSubject;
    
    public:
        WorldChange();
        WorldChange(const std::string&, int, int, ChangeType, const std::string&);
        ~WorldChange();

        std::string getAffectedMapName() const;
        int getLocationX() const;
        int getLocationY() const;
        ChangeType getChangeType() const;
        std::string getChangeSubject() const;
};
