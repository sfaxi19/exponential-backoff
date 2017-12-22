
#ifndef INFO_SYS2_SYSTEM_H
#define INFO_SYS2_SYSTEM_H

#include <iostream>
#include "../MyRandom.h"

class System {
protected:
    size_t m_time_scale = 0;

public:

    System();

    virtual void nextTime() = 0;

    virtual void test_suite(int time_limit) = 0;

    virtual size_t getTime();

    virtual ~System();

    virtual size_t getCountSentPackets() = 0;
};

#endif //INFO_SYS2_SYSTEM_H
