#include "MapTrigger.hpp"

void MapTrigger::trigger() {
    triggered = true;
}

void MapTrigger::setMapTrigger(const MapTrigger& otherTrigger) {
    this->triggerType = otherTrigger.triggerType;
    this->subject = otherTrigger.subject;
    this->triggered = otherTrigger.triggered;
    this->destinationX = otherTrigger.destinationX;
    this->destinationY = otherTrigger.destinationY;
}

void MapTrigger::removeTrigger() {
    triggerType = TriggerType::Null;
    subject = "";
    triggered = false;
    destinationX = 0;
    destinationY = 0;
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

int MapTrigger::getX() const {
    return destinationX;
}

int MapTrigger::getY() const {
    return destinationY;
}

MapTrigger::MapTrigger() {
    triggerType = TriggerType::Null;
    subject = "";
    triggered = false;
    destinationX = 0;
    destinationY = 0;
}

MapTrigger::MapTrigger(TriggerType triggerType, const std::string& subject, bool triggered) {
    this->triggerType = triggerType;
    this->subject = subject;
    this->triggered = triggered;
    destinationX = 0;
    destinationY = 0;
}

MapTrigger::MapTrigger(TriggerType triggerType, const std::string& subject, int destinationX, int destinationY, bool triggered) {
    this->triggerType = triggerType;
    this->subject = subject;
    this->triggered = triggered;
    this->destinationX = destinationX;
    this->destinationY = destinationY;
}

MapTrigger::~MapTrigger() {
}
