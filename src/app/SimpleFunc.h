//
// Created by faliszewskii on 08.10.24.
//

#ifndef SPRING_SIMULATION_SIMPLEFUNC_H
#define SPRING_SIMULATION_SIMPLEFUNC_H

#include<cmath>

struct SimpleFunc{

    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    enum Type {
        None = 0,
        Constant = 1,
        Step = 2,
        Signum = 3,
        Sin = 4
    } type = None;

    float constant = 0.5;
    float stepTime = 0;
    float angleSpeed = 1;
    float phase = 0;

    float f(float time) {
        switch(type) {
            case Constant:
                return constant;
            case Step:
                return time > stepTime ? constant : 0;
            case Signum:
                return sgn(constant * sin(angleSpeed * time + phase));
            case Sin:
                return constant * sin(angleSpeed * time + phase);
            case None:
                break;
        }
        return 0;
    }
};

#endif //SPRING_SIMULATION_SIMPLEFUNC_H
