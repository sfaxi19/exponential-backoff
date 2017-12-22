//
// Created by sfaxi19 on 21.09.17.
//

#ifndef INFO_SYS2_MYRANDOM_H
#define INFO_SYS2_MYRANDOM_H

#include "iostream"

class MyRandom {
public:
    static size_t getPuassonValue(double);

    static double getRand();

    static void init(){
        srand((uint) time(NULL));
    }
};


#endif //INFO_SYS2_MYRANDOM_H
