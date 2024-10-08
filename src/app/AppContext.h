//
// Created by faliszewskii on 16.06.24.
//

#ifndef OPENGL_TEMPLATE_APPCONTEXT_H
#define OPENGL_TEMPLATE_APPCONTEXT_H

#include <memory>
#include "../opengl/framebuffer/FrameBufferManager.h"
#include "../interface/camera/BaseCamera.h"
#include "entity/quad/Quad.h"
#include "../opengl/shader/Shader.h"
#include "entity/light/PointLight.h"
#include "entity/point/Point.h"
#include "entity/sphere/Sphere.h"
#include "entity/cylinder/Cylinder.h"
#include "entity/springModel/SpringModel.h"
#include "spring/SpringSimulation.h"
#include "plotting/RollingBuffer.h"
#include "plotting/ScrollingBuffer.h"
#include "SimpleFunc.h"

struct AppContext {
    AppContext() = default;

    std::pair<int, std::array<float, 100>> rollingMspf;

    std::unique_ptr<BaseCamera> camera;
    std::unique_ptr<FrameBufferManager> frameBufferManager;

    // Shaders
    std::unique_ptr<Shader> phongShader;
    std::unique_ptr<Shader> pointShader;

    std::unique_ptr<PointLight> light;
    std::unique_ptr<Point> lightBulb;
    std::unique_ptr<Quad> quad;

    std::unique_ptr<SpringModel> springModel;
    std::unique_ptr<SpringSimulation> springSimulation;

    float lastFrameTimeMs;
    bool running;

    RollingBuffer plotF;
    RollingBuffer plotG;
    RollingBuffer plotH;
    RollingBuffer plotW;
    float functionPlotHistoricalMax;

    RollingBuffer plotX;
    RollingBuffer plotXt;
    RollingBuffer plotXtt;
    float xPlotHistoricalMax;

    ScrollingBuffer plotTrajectory;
    float trajectoryHistoricalMax;

    SimpleFunc hFunc;
    SimpleFunc wFunc;
};

#endif //OPENGL_TEMPLATE_APPCONTEXT_H
