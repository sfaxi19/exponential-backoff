//
// Created by sfaxi19 on 21.09.17.
//

#ifndef INFO_SYS2_PACKAGE_H
#define INFO_SYS2_PACKAGE_H


#include <cstddef>

class Packet {
private:
    double time_in_system;
public:
    size_t id = 0;
    double in;
    double out;
    Packet(size_t id, double inTime);
};



#endif //INFO_SYS2_PACKAGE_H
