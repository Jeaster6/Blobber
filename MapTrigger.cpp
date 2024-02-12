#include "MapTrigger.hpp"

void MapTrigger::activateTrigger() {
    triggered = true;
}

MapTrigger::MapTrigger() {
}

MapTrigger::MapTrigger(TriggerType triggerType, const std::string& subject, bool triggered) {
    this->triggerType = triggerType;
    this->subject = subject;
    this->triggered = triggered;
}

MapTrigger::~MapTrigger() {
}
