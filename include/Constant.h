//
// Created by Wenxin Zheng on 2021/3/22.
//

#ifndef ANALYSIS_CONSTANT_H
#define ANALYSIS_CONSTANT_H
#include <cstdio>
#include <chrono>

extern std::chrono::steady_clock::time_point t1;

#define CONST_MEM_ACCESS        0
#define CONST_THREAD_BLOCK      1
#define CONST_THREAD_BRANCH     2
#define CONST_ATOMIC            3
#define MEM_ACCESS_WRITE        (1 << 1)
#define MEM_ACCESS_ATOMIC       1
#define CONST_WORD_SIZE               8



#endif//ANALYSIS_CONSTANT_H
