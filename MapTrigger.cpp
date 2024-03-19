#include "MapTrigger.hpp"

void MapTrigger::trigger() {
    triggered = true;
}

void MapTrigger::setMapTrigger(const MapTrigger& otherTrigger) {
    this->triggerType = otherTrigger.triggerType;
    this->subject = otherTrigger.subject;
    this->triggered = otherTrigger.triggered;
}

void MapTrigger::removeTrigger() {
    triggerType = TriggerType::Null;
    subject = "";
    triggered = false;
}

TriggerType MapTrigger::getType() const {
    return triggerType;
}

bool MapTrigger::isTriggered() const {
    return triggered;
}

std::string MapTrigger::getSubject() const {
    return subject;
}

MapTrigger::MapTrigger() {
    triggerType = TriggerType::Null;
    subject = "";
    triggered = false;
}

MapTrigger::MapTrigger(TriggerType triggerType, const std::string& subject, bool triggered) {
    this->triggerType = triggerType;
    this->subject = subject;
    this->triggered = triggered;
}

MapTrigger::~MapTrigger() {
}
