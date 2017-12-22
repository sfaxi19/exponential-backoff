//
// Created by sfaxi19 on 06.12.17.
//
#include "log.h"

namespace log2file {
    void flush() { out.flush(); }

    namespace // detail
    {
        std::string path_to_session_log_file() {
            static const std::string log_dir = "";
            static const std::string log_file_name = "log.txt";
            return log_dir + log_file_name;
        }
    }

    const std::string path = path_to_session_log_file();
    std::ofstream out = std::ofstream(path);
}

