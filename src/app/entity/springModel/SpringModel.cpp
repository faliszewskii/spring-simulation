//
// Created by faliszewskii on 07.10.24.
//

#include "SpringModel.h"

SpringModel::SpringModel(float newHeight, float newLength) {
    height = newHeight;
    length = newLength;
    x = 0;

    anchorPointMesh = std::make_unique<Sphere>();
    springMesh = std::make_unique<Cylinder>();
    weightMesh = std::make_unique<Cylinder>();
}

void SpringModel::render(Shader& shader) {
    auto anchorModel = glm::identity<glm::mat4>();
    anchorModel = glm::translate(anchorModel, glm::vec3(0, height, 0));
    anchorModel = glm::scale(anchorModel, glm::vec3(0.02f));

    shader.setUniform("model", anchorModel);
    shader.setUniform("material.albedo", glm::vec4(.2f, .1f, 0.1f, 1.0f));
    anchorPointMesh->render();

    auto springModel = glm::identity<glm::mat4>();
    float thickness = 0.03f * (1.f - 0.5f * std::atan((length + x) / length));
    springModel = glm::translate(springModel, glm::vec3(0, height - (length + x) / 2.f, 0));
    springModel = glm::scale(springModel, glm::vec3(thickness, (length + x) / length, thickness));
    shader.setUniform("model", springModel);
    shader.setUniform("material.albedo", glm::vec4(.2f, .2f, 0.2f, 1.0f));
    shader.setUniform("material.shininess", 256.f);
    springMesh->render();

    auto weightModel = glm::identity<glm::mat4>();
    weightModel = glm::translate(weightModel, glm::vec3(0, height - length - x, 0));
    weightModel = glm::scale(weightModel, glm::vec3(0.1f, 0.1, 0.1f));
    shader.setUniform("model", weightModel);
    shader.setUniform("material.albedo", glm::vec4(.1f, .1f, 0.2f, 1.0f));
    weightMesh->render();

}

void SpringModel::updateX(float newX) {
    x = newX;
}

void SpringModel::updateHeight(float newHeight) {
    height = newHeight;
}
