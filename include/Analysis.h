//
// Created by Wenxin Zheng on 2021/3/22.
//

#ifndef ANALYSIS_ANALYSIS_H
#define ANALYSIS_ANALYSIS_H

#include "BaseEvent.h"
#include <map>
#include <string>
#include <vector>
class AnalysisReport{
private:
    std::string filename;
    std::map<int, std::vector<BaseEvent*>> event_thread_map;
    std::vector<std::vector<unsigned long>> memory_map;
    std::map<unsigned long, int> memory_index;
    std::vector<unsigned long> pc_list;
    std::vector<std::pair<unsigned long, unsigned long>> jump_list;

public:
    AnalysisReport(const std::string& filename);
    void buildGraph();
};

#endif //ANALYSIS_ANALYSIS_H
