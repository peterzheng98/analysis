//
// Created by Wenxin Zheng on 2021/3/22.
//

#include <RecordEvent.h>

AtomicEvent::AtomicEvent(unsigned long pc, int threadIdentifier, unsigned long address, BaseEvent *relatedMemoryEvent, int memoryConsistencyLevel) : BaseEvent(pc, threadIdentifier), address(address), related_memory_event(relatedMemoryEvent), memory_consistency_level((ConsistencyType) memoryConsistencyLevel) {
    setEventType(CONST_ATOMIC);
}
BaseEvent *AtomicEvent::getRelatedMemoryEvent() const {
    return related_memory_event;
}
void AtomicEvent::setRelatedMemoryEvent(BaseEvent *relatedMemoryEvent) {
    related_memory_event = relatedMemoryEvent;
}
AtomicEvent::AtomicEvent(unsigned long pc, int threadIdentifier, unsigned long address, int memoryConsistencyLevel) : BaseEvent(pc, threadIdentifier, CONST_ATOMIC), address(address), memory_consistency_level((ConsistencyType)memoryConsistencyLevel) {}

ThreadBranchEvent::ThreadBranchEvent(unsigned long pc, int threadIdentifier, int branchCondition) : BaseEvent(pc, threadIdentifier, CONST_THREAD_BRANCH), branch_condition(branchCondition) {
    setEventType(CONST_THREAD_BRANCH);
}

ThreadBlockEvent::ThreadBlockEvent(unsigned long pc, int threadIdentifier, int valueTaken) : BaseEvent(pc, threadIdentifier, CONST_THREAD_BLOCK), value_taken(valueTaken) {
    setEventType(CONST_THREAD_BLOCK);
}

MemoryAccessEvent::MemoryAccessEvent(unsigned long pc, int threadIdentifier, int accessSize, unsigned long address, int flag) : BaseEvent(pc, threadIdentifier, CONST_MEM_ACCESS), access_size(accessSize), address(address) {
    if(flag & MEM_ACCESS_ATOMIC) isAtomic = true; else isAtomic = false;
    if(flag & MEM_ACCESS_WRITE) isWrite = true; else isWrite = false;
    setEventType(CONST_MEM_ACCESS);
}
int MemoryAccessEvent::getAccessSize() const {
    return access_size;
}
void MemoryAccessEvent::setAccessSize(int accessSize) {
    access_size = accessSize;
}
unsigned long MemoryAccessEvent::getAddress() const {
    return address;
}
void MemoryAccessEvent::setAddress(unsigned long address) {
    MemoryAccessEvent::address = address;
}
bool MemoryAccessEvent::isWrite1() const {
    return isWrite;
}
void MemoryAccessEvent::setIsWrite(bool isWrite) {
    MemoryAccessEvent::isWrite = isWrite;
}
bool MemoryAccessEvent::isAtomic1() const {
    return isAtomic;
}
void MemoryAccessEvent::setIsAtomic(bool isAtomic) {
    MemoryAccessEvent::isAtomic = isAtomic;
}
