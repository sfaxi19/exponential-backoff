//
// Created by sfaxi19 on 21.09.17.
//

#include "MyRandom.h"
#include "math.h"

size_t MyRandom::getPuassonValue(double lambda) {
    double z = getRand();
    size_t n = 0;
    double s;
    double r;
    s = r = exp(-lambda);
    while (z > s) {
        n++;
        r = r * (lambda / n);
        s = s + r;
    }
    return n;
}

double MyRandom::getRand() {
    return (double) rand() / RAND_MAX;
}
