#pragma once

#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

enum TriggerType { Null, MapExit, DisplayMessage };

class MapTrigger {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& triggerType;
            ar& subject;
            ar& triggered;
            ar& destinationX;
            ar& destinationY;
        }

    protected:
        TriggerType triggerType;
        std::string subject;
        bool triggered;
        int destinationX;
        int destinationY;

    public:
        MapTrigger();
        MapTrigger(TriggerType, const std::string&, bool);
        MapTrigger(TriggerType, const std::string&, int, int, bool);
        ~MapTrigger();

        void trigger();
        void setMapTrigger(const MapTrigger&);
        void removeTrigger();

        TriggerType getType() const;
        std::string getSubject() const;
        bool isTriggered() const;
        int getX() const;
        int getY() const;
};
