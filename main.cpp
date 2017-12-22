#include <iostream>
#include <cmath>
#include "AdvancedSystem/AdvancedSystem.h"
#include "log.h"

bool isLog = false;

void saveResults(std::vector<asys::AdvancedResult> results, const char *comment) {
    if (results.size() < 1) return;
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

std::vector<asys::AdvancedResult> testBinBackoff(size_t width_min, size_t width_max, size_t subs) {
    std::vector<asys::AdvancedResult> results;

    size_t min = 1;
    size_t max = 100;
    size_t ranges = max;
    size_t step = 1;
    for (size_t i = min; i < max; i += step) {
        if ((double) i / ranges > 0.4) continue;
        LOG log2file::out << "Test Binary backoff lambda = " << std::to_string((double) i / ranges) << std::endl;
        asys::AdvancedSystem system(subs, width_min, width_max, (double) i / ranges, false);
        system.test_suite(100000);
        results.push_back(system.getResult());
    }
    return results;
}

void startModeling() {
    system("mkdir -p advancedSystem/delays");
    system("mkdir -p advancedSystem/streams");
    std::cout << "lof file: " << log2file::path << std::endl;
    //=============================================================
    //                        Simple test
    //=============================================================
    {
        asys::AdvancedSystem system(5, 1, 0.8, true);
        system.test_suite(10);
        asys::AdvancedResult res = system.getResult();
        asys::print_results(res);
    }
    //=============================================================
    //                         ALOHA test
    //=============================================================
    {
        std::vector<asys::AdvancedResult> res;
        res = testALOHA(64, 64);
        asys::print_results(res);
        saveResults(res, "ALOHA W-150");
    }
    //=============================================================
    //                     Binary backoff test
    //=============================================================
    {
        std::vector<asys::AdvancedResult> res;
        res = testBinBackoff(1, 255, 128);
        asys::print_results(res);
        saveResults(res, "ALOHA Wmin-1 Wmax-255");
    }
}

int main() {
    startModeling();
    return 0;
}
