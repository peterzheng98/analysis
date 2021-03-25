#include <iostream>
#include <string>
#include <chrono>
#include <Analysis.h>

static int debug_on;
std::chrono::steady_clock::time_point t1;

void help(){
    std::cout << "Usage: program <AnalysisFile> <ReducedELF> <Debug>" << std::endl;
    exit(0);
}

int main(int argc, char** argv) {
    if(argc != 4) help();
    t1 = std::chrono::steady_clock::now();
    std::string analysis_file(argv[1]), reduced_elf(argv[2]);
    AnalysisReport analysisReport(analysis_file);
    debug_on = atoi(argv[3]);
    analysisReport.buildGraph();
    return 0;
}
