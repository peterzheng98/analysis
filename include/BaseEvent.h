//
// Created by Wenxin Zheng on 2021/3/22.
//

#ifndef ANALYSIS_BASEEVENT_H
#define ANALYSIS_BASEEVENT_H

class BaseEvent{
private:
    unsigned long pc = 0;
    int thread_identifier = 0;

protected:
    int event_type = -1;

public:
    unsigned long getPc() const;
    void setPc(unsigned long pc);
    int getEventType() const;
    void setEventType(int eventType);
    BaseEvent(unsigned long pc, int thread_identifier);
    BaseEvent(unsigned long pc, int threadIdentifier, int eventType);
    virtual ~BaseEvent();
};

typedef enum Consistency{
    Relaxed, Consume, Acquire, Release, Acq_rel, Seq_cst
} ConsistencyType;

#endif//ANALYSIS_BASEEVENT_H
