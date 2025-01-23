//
// Created by USER on 23/01/2025.
//

#ifndef FLYWHEELMODEL_H
#define FLYWHEELMODEL_H

#include <random>

#include "../cylinder/Cylinder.h"

class FlywheelModel {
    float R;
    float L;
    float angle;
    float omega;
    float error;

    float x;
    float xt;
    float xtt;
    float l;

    std::unique_ptr<Cylinder> cylinder;

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<float> d;

public:
    FlywheelModel(float R, float L, float angle, float omega);

    void updateR(float newR) { R = newR; }
    void updateL(float newL) { L = newL; }
    void updateAngle(float newAngle) { angle = newAngle; }
    
    float getX() { return x; }
    float getXt() { return xt; }
    float getXtt() { return xtt; }

    void render(Shader& shader);

    void advanceByStep(float dt);
    float addError(float L);
    void reset();

    float getAngle() { return angle; }
    float getL() { return L; }
    float getR() { return R; }

    float getOmega() { return omega; }
    float getError() { return error; }

    void updateOmega(float newOmega) { omega = newOmega; }
    void updateError(float newError) { error = newError; }
};



#endif //FLYWHEELMODEL_H
