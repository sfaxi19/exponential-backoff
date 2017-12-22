//
// Created by sfaxi19 on 08.11.17.
//
#include "System.h"

System::System() {
    MyRandom::init();
}

size_t System::getTime() {
    return m_time_scale;
}

System::~System() {

}
