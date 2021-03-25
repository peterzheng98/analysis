//
// Created by Wenxin Zheng on 2021/3/22.
//

#ifndef ANALYSIS_UTILITIES_H
#define ANALYSIS_UTILITIES_H

#include <string>
#include <BaseEvent.h>
#include <Constant.h>
#include <RecordEvent.h>

extern std::chrono::steady_clock::time_point t1;

_LIBCPP_ALWAYS_INLINE inline std::string getTimeString(){
    return std::string("[") + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count() / 1000000.0) + std::string("] ");
}

_LIBCPP_ALWAYS_INLINE inline long calculateDistance(BaseEvent* back_one, BaseEvent* front_one){
    return ((long)back_one->getPc() - (long)front_one->getPc()) / CONST_WORD_SIZE;
}


#endif//ANALYSIS_UTILITIES_H
