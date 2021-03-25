//
// Created by Wenxin Zheng on 2021/3/22.
//

#include <BaseEvent.h>
#include <sstream>
#include <string>

BaseEvent::BaseEvent(unsigned long pc, int thread_identifier) : pc(pc), thread_identifier(thread_identifier){ }
unsigned long BaseEvent::getPc() const {
    return pc;
}
void BaseEvent::setPc(unsigned long pc) {
    BaseEvent::pc = pc;
}
int BaseEvent::getEventType() const {
    return event_type;
}
void BaseEvent::setEventType(int eventType) {
    event_type = eventType;
}
BaseEvent::BaseEvent(unsigned long pc, int threadIdentifier, int eventType) : pc(pc), thread_identifier(threadIdentifier), event_type(eventType) {}
BaseEvent::~BaseEvent() { }
std::string BaseEvent::getPcHex() const {
    std::stringstream stream;
    stream << std::hex << "0x" << this->pc;
    std::string result( stream.str() );
    return result;
}
