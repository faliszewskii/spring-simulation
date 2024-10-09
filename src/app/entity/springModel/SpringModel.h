//
// Created by faliszewskii on 07.10.24.
//

#ifndef SPRING_SIMULATION_SPRINGMODEL_H
#define SPRING_SIMULATION_SPRINGMODEL_H


#include <memory>
#include "../sphere/Sphere.h"
#include "../cylinder/Cylinder.h"

class SpringModel {
    float height;
    float length;
    float x;

    std::unique_ptr<Sphere> anchorPointMesh;
    std::unique_ptr<Cylinder> springMesh;
    std::unique_ptr<Cylinder> weightMesh;

public:
    SpringModel(float newHeight, float newLength);

    void updateX(float newLength);
    void render(Shader& shader);

    void updateHeight(float newHeight);
};


#endif //SPRING_SIMULATION_SPRINGMODEL_H
