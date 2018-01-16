//
// Created by sfaxi19 on 06.12.17.
//

#ifndef LOG_H
#define LOG_H

#include <string>
#include <fstream>
#define LOG   if(isLog)

namespace log2file {
    extern const std::string path;
    extern std::ofstream out;
    extern bool isLog;
    void flush();
}
#endif //LOG_H
