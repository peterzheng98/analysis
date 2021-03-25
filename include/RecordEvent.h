//
// Created by Wenxin Zheng on 2021/3/22.
//

#ifndef ANALYSIS_RECORDEVENT_H
#define ANALYSIS_RECORDEVENT_H

#include <Constant.h>
#include <BaseEvent.h>

class MemoryAccessEvent : public BaseEvent{
private:
    int access_size;
    unsigned long address;
    bool isWrite, isAtomic;

public:
    MemoryAccessEvent(unsigned long pc, int threadIdentifier, int accessSize, unsigned long address, int flag);
    int getAccessSize() const;
    void setAccessSize(int accessSize);
    unsigned long getAddress() const;
    void setAddress(unsigned long address);
    bool isWrite1() const;
    void setIsWrite(bool isWrite);
    bool isAtomic1() const;
    void setIsAtomic(bool isAtomic);
};

class ThreadBlockEvent : public BaseEvent{
public:
    ThreadBlockEvent(unsigned long pc, int threadIdentifier, int valueTaken);
private:
    int value_taken;
};

class ThreadBranchEvent : public BaseEvent{
public:
    ThreadBranchEvent(unsigned long pc, int threadIdentifier, int branchCondition);

private:
    int branch_condition;
};

class AtomicEvent : public BaseEvent{
private:
    unsigned long address;
    BaseEvent* related_memory_event;
    ConsistencyType memory_consistency_level;

public:
    BaseEvent *getRelatedMemoryEvent() const;
    void setRelatedMemoryEvent(BaseEvent *relatedMemoryEvent);
    AtomicEvent(unsigned long pc, int threadIdentifier, unsigned long address, int memoryConsistencyLevel);

public:
    AtomicEvent(unsigned long pc, int threadIdentifier, unsigned long address, BaseEvent *relatedMemoryEvent, int memoryConsistencyLevel);
};

#endif//ANALYSIS_RECORDEVENT_H
