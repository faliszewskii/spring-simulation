//
// Created by faliszewskii on 08.10.24.
//

#ifndef SPRING_SIMULATION_SPRINGSIMULATION_H
#define SPRING_SIMULATION_SPRINGSIMULATION_H


#include <functional>
#include <glm/detail/type_vec2.hpp>
#include <glm/vec2.hpp>

class SpringSimulation {
    struct State {
        float x;
        float v;
    };

public:

    State startCondition;
    State springState;
    float timeStepMs;
    float weightMass;
    float elasticityCoeff;
    float dampingCoeff;

    std::function<float(float)> fieldFunction;
    std::function<float(float)> anchorFunction;

    std::function<glm::vec2(glm::vec2, float, std::function<float()>)> ode;

    float time;

    SpringSimulation();

    void advanceByStep();
    void reset();

    float xtt();
    float f();
    float g();
    float h();
    float w();
};


#endif //SPRING_SIMULATION_SPRINGSIMULATION_H
