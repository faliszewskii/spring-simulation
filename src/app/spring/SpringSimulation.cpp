//
// Created by faliszewskii on 08.10.24.
//

#include <glm/vec2.hpp>
#include "SpringSimulation.h"

SpringSimulation::SpringSimulation() {
    time = 0;
    startCondition = { 0.5f, 0.f };
    springState = startCondition;
    timeStepMs = 2;
    weightMass = 1;
    elasticityCoeff = 10;
    dampingCoeff = 0.1;
    fieldFunction = [](float _){return 0;};
    anchorFunction = [](float _){return 0;};
    ode = [](glm::vec2 y, float dt, std::function<float()> xtt) {
        return dt * glm::vec2(y[1], xtt());  // dx/dt = v, dv/dt = a
    };
}

void SpringSimulation::reset() {
    time = 0;
    springState = startCondition;
}

void SpringSimulation::advanceByStep() {
    glm::vec2 y(springState.x, springState.v);
    float dt = timeStepMs / 1000.f;
    time += dt;
    y += ode(y, dt, [this](){ return xtt(); });
    springState = {y[0], y[1]};
}

float SpringSimulation::xtt() {
    return (f() + g() + h()) / weightMass;
}

float SpringSimulation::f() {
    return elasticityCoeff * (w() - springState.x);
}

float SpringSimulation::g() {
    return - dampingCoeff * springState.v;
}

float SpringSimulation::h() {
    return fieldFunction(time);
}

float SpringSimulation::w() {
    return anchorFunction(time);
}
