//
// Created by Wenxin Zheng on 2021/3/22.
//

#include <Analysis.h>
#include <Constant.h>
#include <utilities.h>
#include <RecordEvent.h>

#include <fstream>
#include <iostream>
#include <utility>

AnalysisReport::AnalysisReport(const std::string &filename) : filename(filename){
    std::cerr << getTimeString() << "Reading analysis report from file: " << filename << std::endl;
    std::fstream analysis;
    analysis.open(filename.c_str(), std::ios::in);
    int opcode;
    while (analysis >> opcode){
        std::cerr << getTimeString() << "Opcode: " << opcode << " ";
        switch (opcode) {
            case CONST_MEM_ACCESS:{
                int thr, access_size, flag;
                unsigned long pc, access;
                analysis >> thr >> pc >> access >> access_size >> flag;
                auto* ptr = new MemoryAccessEvent(pc, thr, access_size, access, flag);
                event_thread_map[thr].push_back(ptr);
                std::cerr << std::endl;
                break;
            }
            case CONST_THREAD_BLOCK:{
                int thr, cond;
                unsigned long pc;
                analysis >> thr >> pc >> cond;
                auto* ptr = new ThreadBlockEvent(pc, thr, cond);
                event_thread_map[thr].push_back(ptr);
                std::cerr << std::endl;
                break;
            }
            case CONST_THREAD_BRANCH:{
                int thr, cond;
                unsigned long pc;
                analysis >> thr >> pc >> cond;
                auto *ptr = new ThreadBranchEvent(pc, thr, cond);
                event_thread_map[thr].push_back(ptr);
                std::cerr << std::endl;
                break;
            }
            case CONST_ATOMIC:{
                int thr, consistency;
                unsigned long pc, address;
                analysis >> thr >> pc >> address >> consistency;
                auto *ptr = new AtomicEvent(pc, thr, address, (ConsistencyType)consistency);
                event_thread_map[thr].push_back(ptr);
                std::cerr << std::endl;
                break;
            }
            default:
                std::cerr << "Unrecognized the operation code" << std::endl;
        }
    }
    std::cerr << getTimeString() << "Fetch length: " << [&]()->int{
        int sum = 0;
        for(auto &iter_begin: event_thread_map){
            sum += iter_begin.second.size();
        }
        return sum;
    }() << "\n";
}

long AnalysisReport::_calculateRealDistance(const std::vector<BaseEvent*> &thr, const int &end_idx, const int &first_idx) {
    long pc_distance = 0;
    for(int idx = first_idx; idx < end_idx; ++idx){
        pc_distance += calculateDistance(thr[idx + 1], thr[idx]);
    }
    for(int idx = first_idx; idx < end_idx; ++idx){
        if(thr[idx + 1]->getEventType() == CONST_THREAD_BLOCK && thr[idx]->getEventType() == CONST_THREAD_BRANCH) pc_distance -= calculateDistance(thr[idx + 1], thr[idx]);
    }
    return pc_distance;
}

void AnalysisReport::buildGraph() {
    for(auto &iter_begin: event_thread_map){
        for(auto & iter_event_list : iter_begin.second){
            pc_list.push_back(iter_event_list->getPc());
        }
    }
    std::sort(pc_list.begin(), pc_list.end());
    pc_list.erase(std::unique(pc_list.begin(), pc_list.end()), pc_list.end());
    for(auto & iter_begin : event_thread_map){
        for(auto iter_event_list = iter_begin.second.begin(), iter_event_list_next = ++iter_begin.second.begin();
             iter_event_list_next != iter_begin.second.end();
             ++iter_event_list_next){
            if((*iter_event_list)->getEventType() == CONST_THREAD_BRANCH){
                while((iter_event_list_next) != iter_begin.second.end() &&
                       (*iter_event_list_next)->getEventType() != CONST_THREAD_BLOCK) iter_event_list_next++;
                jump_list.push_back(std::make_pair<unsigned long, unsigned long>((*iter_event_list)->getPc(), (*iter_event_list_next)->getPc()));
                iter_event_list = iter_event_list_next;
            } else iter_event_list++;
        }
    }

    std::cerr << getTimeString() << "jump_list: " << jump_list.size() << std::endl;
    for(auto &iter_begin: event_thread_map){
        // pass: merge the atomic and its corresponding memory access
        {
            int terminate_size = iter_begin.second.size();
            for(int i = 0; i < terminate_size; ++i){
                if(iter_begin.second[i]->getEventType() == CONST_ATOMIC){
                    assert(iter_begin.second[i + 1]->getEventType() == CONST_MEM_ACCESS);
                    dynamic_cast<MemoryAccessEvent*>(iter_begin.second[i + 1])->setMemoryConsistencyLevel(dynamic_cast<AtomicEvent*>(iter_begin.second[i])->getMemoryConsistencyLevel());
                    iter_begin.second.erase(iter_begin.second.begin() + i);
                    terminate_size--;
                }
            }
        }
        // pass: find the length of memory access;
        {
            int total_size = iter_begin.second.size();
            int i = 0;
            while (iter_begin.second[i]->getEventType() != CONST_MEM_ACCESS) i++;
            for (; i < total_size - 1; ++i) {
                int next_target = i + 1;
                long pc_distance = 0;
                bool inside_jump = false;
                int prev_target = i;
                while (next_target < total_size && iter_begin.second[next_target]->getEventType() != CONST_MEM_ACCESS) next_target++;
                if (next_target >= total_size) break;
                pc_distance = _calculateRealDistance(iter_begin.second, next_target, i);
                if (iter_begin.second[next_target]->getEventType() == CONST_MEM_ACCESS) {
                    std::cerr << getTimeString() << "Distance on thread " << iter_begin.first << ": " << pc_distance << " insts is "
                              << (dynamic_cast<MemoryAccessEvent *>(iter_begin.second[i]))->getPcHex() << " "
                              << (dynamic_cast<MemoryAccessEvent *>(iter_begin.second[next_target]))->getPcHex() << std::endl;
                    i = next_target;
                    continue;
                }
            }
        }
    }
}