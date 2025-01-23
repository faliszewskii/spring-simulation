//
// Created by USER on 23/01/2025.
//

#include "FlywheelModel.h"

#include <random>

FlywheelModel::FlywheelModel(float R, float L, float angle, float omega) : R(R), L(L), angle(angle), omega(omega) {
    cylinder = std::make_unique<Cylinder>(1, 1,30);
    x = L + R;
    xt = 0;
    xtt = 0;
    l = addError(L);


    std::random_device rd{};
    std::mt19937 gen{rd()};
}

void FlywheelModel::render(Shader &shader) {
    // Wheel
    auto model = glm::identity<glm::mat4>();
    model = glm::rotate(model, angle, glm::vec3(0,0,1));
    model = glm::rotate(model, static_cast<float>(std::numbers::pi/2), glm::vec3(1,0,0));
    model = glm::scale(model, glm::vec3(R+0.11,0.05,R+0.11));

    shader.setUniform("model", model);
    shader.setUniform("material.albedo", glm::vec4(1.f, 1.f, 1.f, 1.0f));
    cylinder->render();

    // Rotor
    model = glm::identity<glm::mat4>();
    model = glm::rotate(model, static_cast<float>(std::numbers::pi/2), glm::vec3(1,0,0));
    model = glm::scale(model, glm::vec3(0.1,0.1,0.1));

    shader.setUniform("model", model);
    shader.setUniform("material.albedo", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    cylinder->render();

    // handle
    model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(0, 0, 0.1));
    model = glm::rotate(model, angle, glm::vec3(0,0,1));
    model = glm::rotate(model, static_cast<float>(std::numbers::pi/2), glm::vec3(1,0,0));
    model = glm::translate(model, glm::vec3(R, 0, 0));
    model = glm::scale(model, glm::vec3(0.1,0.15,0.1));

    shader.setUniform("model", model);
    shader.setUniform("material.albedo", glm::vec4(0.5f, 1.f, 1.f, 1.0f));
    cylinder->render();

    // Piston
    model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(x, 0, 0.2));
    model = glm::rotate(model, static_cast<float>(std::numbers::pi/2), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(0.1,0.1,0.1));

    shader.setUniform("model", model);
    shader.setUniform("material.albedo", glm::vec4(0.5f, 1.f, 1.f, 1.0f));
    cylinder->render();

    // Piston arm
    float py = std::sin(angle) * R;
    float s =  py / l;
    model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(x, 0, 0.15));
    model = glm::rotate(model, static_cast<float>(std::numbers::pi/2 -std::asin(s)), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(0.02,l,0.02));
    model = glm::translate(model, glm::vec3(0, 1.f/2, 0));

    shader.setUniform("model", model);
    shader.setUniform("material.albedo", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    cylinder->render();

    // Slider
    model = glm::identity<glm::mat4>();
    model = glm::translate(model, glm::vec3(0, 0, 0.2));
    model = glm::rotate(model, -static_cast<float>(std::numbers::pi/2), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(0.02,R+L+0.2,0.02));
    model = glm::translate(model, glm::vec3(0, 1/2.f, 0));

    shader.setUniform("model", model);
    shader.setUniform("material.albedo", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    cylinder->render();
}

void FlywheelModel::advanceByStep(float dt) {
    float oldX = x;
    float oldXt = xt;

    angle += omega * dt / 1000;
    angle = std::fmod(angle, 2*std::numbers::pi);

    l = addError(L);

    float px = std::cos(angle) * R;
    float py = std::sin(angle) * R;
    float s =  py / l;
    x = std::sqrt(1 - s*s) * l + px;

    xt = (x - oldX) / (dt/1000);
    xtt = (xt - oldXt) / (dt/1000);
}

float FlywheelModel::addError(float L) {
    std::normal_distribution d{0.0f, error};
    return d(gen) + L;
}

void FlywheelModel::reset() {
    angle = 0;
    x = L + R;
    l = addError(L);
    xt = 0;
    xtt = 0;
}
