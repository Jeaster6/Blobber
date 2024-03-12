#pragma once

#include <string>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

enum TriggerType { Null, MapExit };

class MapTrigger {

    private:
        friend class boost::serialization::access;
        template<class Archive> void serialize(Archive& ar, const unsigned int version) {
            ar& triggerType;
            ar& subject;
            ar& triggered;
        }

        TriggerType triggerType;
        std::string subject;
        bool triggered;

    public:
        MapTrigger();
        MapTrigger(TriggerType, const std::string&, bool);
        ~MapTrigger();

        void trigger();
        void setMapTrigger(const MapTrigger&);
        void removeTrigger();

        TriggerType getType() const;
        bool isTriggered() const;
};
