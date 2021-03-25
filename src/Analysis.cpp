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
                auto *ptr = new AtomicEvent(pc, thr, address, consistency);
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
        int total_size = iter_begin.second.size();
        int i = 0;
        while(iter_begin.second[i]->getEventType() != CONST_MEM_ACCESS) i++;
        for(; i < total_size - 1; ++i){
            int next_target = i + 1;
            while(iter_begin.second[next_target]->getEventType() != CONST_MEM_ACCESS) next_target++;
            if(iter_begin.second[next_target]->getEventType() == CONST_MEM_ACCESS){
                std::cerr << getTimeString() << "Distance: " << calculateDistance(iter_begin.second[next_target], iter_begin.second[i]) << " insts is "
                          << (dynamic_cast<MemoryAccessEvent*>(iter_begin.second[i]))->getAddress() << " "
                          << (dynamic_cast<MemoryAccessEvent*>(iter_begin.second[next_target]))->getAddress() << std::endl;
                i = next_target;
                continue;
            }

//            int pc_distance =

        }
    }
}