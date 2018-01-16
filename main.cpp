#include <iostream>
#include <cmath>
#include "AdvancedSystem/AdvancedSystem.h"
#include "log.h"

bool isLog = false;

void saveResults(std::vector<asys::AdvancedResult> results, const char *comment) {
    if (results.size() < 1) return;
    //FILE *fileDelay = fopen("/home/sfaxi19/Projects/CLionProjects/backoff/delays/delay_inrev.csv","w");
    //FILE *fileStream = fopen("/home/sfaxi19/Projects/CLionProjects/backoff/streams/stream_interv.csv","w");
    FILE *fileDelay = fopen((std::string("advancedSystem/delays/subs_")
            .append(std::to_string(results[0].subscribers))
            .append("[").append(comment).append("]")
            .append(".csv")).c_str(), "w");
    FILE *fileStream = fopen((std::string("advancedSystem/streams/subs_")
            .append(std::to_string(results[0].subscribers) + "_")
            .append("[").append(comment).append("]")
            .append(".csv")).c_str(), "w");        
    for (int i = 0; i < results.size(); i++) {
        fprintf(fileDelay, "%5.2f,", results[i].lambda);
        fprintf(fileStream, "%5.2f,", results[i].lambda);
    }
    fprintf(fileDelay, "\n");
    fprintf(fileStream, "\n");
    for (int i = 0; i < results.size(); i++) {
        fprintf(fileDelay, "%5.2f,", results[i].delay);
        fprintf(fileStream, "%5.2f,", results[i].out_stream);
    }
    fclose(fileDelay);
    fclose(fileStream);
}

std::vector<asys::AdvancedResult> testALOHA(size_t width, size_t subs) {
    std::vector<asys::AdvancedResult> results;
    size_t min = 1;
    size_t max = 100;
    size_t ranges = max;
    size_t step = 1;
    for (size_t i = min; i < max; i += step) {
        if ((double) i / ranges > 0.4) continue;
        LOG log2file::out << "Test ALOHA lambda = " << std::to_string((double) i / ranges) << std::endl;
        asys::AdvancedSystem system(subs, width, (double) i / ranges, false);
        system.test_suite(100000);
        results.push_back(system.getResult());
    }
    return results;
}

std::vector<asys::AdvancedResult> testBinBackoff(size_t width_min, size_t width_max, size_t subs, int time_limit) {
    std::vector<asys::AdvancedResult> results;
    double min = 0.01;
    double max = 0.38;
    double step = 0.01;
    for (double i = min; i <= max; i += step) {
        LOG log2file::out << "Test Binary backoff lambda = " << std::to_string(i) << std::endl;
        asys::AdvancedSystem system(subs, width_min, width_max, i, false);
        system.test_suite(time_limit);
        results.push_back(system.getResult());
    }
    return results;
}

void startModeling() {
    system("mkdir -p advancedSystem/delays");
    system("mkdir -p advancedSystem/streams");
    std::cout << "log file: " << log2file::path << std::endl;
    //=============================================================
    //                        Simple test
    //=============================================================
    {
        asys::AdvancedSystem system(4, 1, 8, 0.3, true);
        system.test_suite(100);
        asys::AdvancedResult res = system.getResult();
        asys::print_results(res);
    }
    //=============================================================
    //                         ALOHA test
    //=============================================================
    //{
    //    std::vector<asys::AdvancedResult> res;
    //    res = testALOHA(32, 32);
    //    asys::print_results(res);
    //    saveResults(res, "ALOHA W-32");
    //}
    //=============================================================
    //                     Binary backoff test
    //=============================================================
    {
        std::vector<asys::AdvancedResult> res;
        res = testBinBackoff(1, 32, 16, 100000);
        asys::print_results(res);
        saveResults(res, "ALOHA Wmin-1 Wmax-32");
    }
    //system("csv_charts -xy /home/sfaxi19/Projects/CLionProjects/backoff/delays/ &");
    //system("csv_charts -xy /home/sfaxi19/Projects/CLionProjects/backoff/streams/ &");
}

int main() {
    startModeling();
    return 0;
}
