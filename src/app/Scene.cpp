//
// Created by faliszewskii on 16.06.24.
//

#include "Scene.h"
#include "../interface/camera/CameraAnchor.h"
#include "entity/springModel/SpringModel.h"

Scene::Scene(AppContext &appContext) : appContext(appContext) {
    appContext.camera = std::make_unique<CameraAnchor>(1920, 1080, glm::vec3(0.0f, 3.0f, 3.0f), glm::vec3(0.f), glm::vec3(-M_PI / 4, 0, 0));
    appContext.frameBufferManager = std::make_unique<FrameBufferManager>();
    appContext.frameBufferManager->create_buffers(appContext.camera->screenWidth, appContext.camera->screenHeight);

    // TODO --- Initialization of the app state goes here.

    appContext.phongShader = std::make_unique<Shader>(Shader::createTraditionalShader(
            "../res/shaders/phong/phong.vert", "../res/shaders/phong/phong.frag"));
    appContext.pointShader = std::make_unique<Shader>(Shader::createTraditionalShader(
            "../res/shaders/point/point.vert", "../res/shaders/point/point.frag"));

    appContext.quad = std::make_unique<Quad>();
    appContext.light = std::make_unique<PointLight>();
    appContext.light->position = {0.5f , 0.5f, 0.5f};
    appContext.lightBulb = std::make_unique<Point>();

    appContext.springModel = std::make_unique<SpringModel>(0.5, 1);
    appContext.springSimulation = std::make_unique<SpringSimulation>();
    appContext.springSimulation->fieldFunction = [&](float time){ return appContext.hFunc.f(time); };
    appContext.springSimulation->anchorFunction = [&](float time){ return appContext.wFunc.f(time); };

    appContext.lastFrameTimeMs = glfwGetTime() * 1000;
    appContext.running = false;

    appContext.springModel->update(appContext.springSimulation->springState.x);

    appContext.functionPlotHistoricalMax = 0;
    appContext.xPlotHistoricalMax = 0;
    appContext.trajectoryHistoricalMax = 0;
}

void Scene::update() {
    appContext.lightBulb->position = appContext.light->position;
    appContext.lightBulb->color = glm::vec4(appContext.light->color, 1);

    float timeMs = glfwGetTime() * 1000;
    int loopsToDo = static_cast<int>((timeMs - appContext.lastFrameTimeMs) / appContext.springSimulation->timeStepMs);
    appContext.lastFrameTimeMs += loopsToDo * appContext.springSimulation->timeStepMs;

    if(appContext.running) {
        for(int i = 0; i < loopsToDo; i++)
            appContext.springSimulation->advanceByStep();
        appContext.springModel->update(appContext.springSimulation->springState.x);

        appContext.plotF.AddPoint(appContext.springSimulation->time, appContext.springSimulation->f());
        if(std::abs(appContext.springSimulation->f()) > appContext.functionPlotHistoricalMax)
            appContext.functionPlotHistoricalMax =std::abs(appContext.springSimulation->f());
        appContext.plotG.AddPoint(appContext.springSimulation->time, appContext.springSimulation->g());
        if(std::abs(appContext.springSimulation->g()) > appContext.functionPlotHistoricalMax)
            appContext.functionPlotHistoricalMax =std::abs(appContext.springSimulation->g());
        appContext.plotH.AddPoint(appContext.springSimulation->time, appContext.springSimulation->h());
        if(std::abs(appContext.springSimulation->h()) > appContext.functionPlotHistoricalMax)
            appContext.functionPlotHistoricalMax =std::abs(appContext.springSimulation->h());
        appContext.plotW.AddPoint(appContext.springSimulation->time, appContext.springSimulation->w());
        if(std::abs(appContext.springSimulation->w()) > appContext.functionPlotHistoricalMax)
            appContext.functionPlotHistoricalMax =std::abs(appContext.springSimulation->w());


        appContext.plotX.AddPoint(appContext.springSimulation->time, appContext.springSimulation->springState.x);
        if(std::abs(appContext.springSimulation->springState.x) > appContext.xPlotHistoricalMax)
            appContext.xPlotHistoricalMax =std::abs(appContext.springSimulation->springState.x);
        appContext.plotXt.AddPoint(appContext.springSimulation->time, appContext.springSimulation->springState.v);
        if(std::abs(appContext.springSimulation->springState.v) > appContext.xPlotHistoricalMax)
            appContext.xPlotHistoricalMax =std::abs(appContext.springSimulation->springState.v);
        appContext.plotXtt.AddPoint(appContext.springSimulation->time, appContext.springSimulation->xtt());
        if(std::abs(appContext.springSimulation->xtt()) > appContext.xPlotHistoricalMax)
            appContext.xPlotHistoricalMax =std::abs(appContext.springSimulation->xtt());

        appContext.plotTrajectory.AddPoint(appContext.springSimulation->springState.x, appContext.springSimulation->springState.v);
        if(std::abs(appContext.springSimulation->springState.x) > appContext.trajectoryHistoricalMax)
            appContext.trajectoryHistoricalMax =std::abs(appContext.springSimulation->springState.x);
        if(std::abs(appContext.springSimulation->springState.v) > appContext.trajectoryHistoricalMax)
            appContext.trajectoryHistoricalMax =std::abs(appContext.springSimulation->springState.v);
    }

}

void Scene::render() {
    appContext.frameBufferManager->bind();

    // TODO --- Here goes scene render.
    appContext.phongShader->use();
    appContext.phongShader->setUniform("viewPos", appContext.camera->getViewPosition());
    appContext.phongShader->setUniform("view", appContext.camera->getViewMatrix());
    appContext.phongShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    appContext.phongShader->setUniform("model", glm::identity<glm::mat4>());
    appContext.phongShader->setUniform("material.hasTexture", false);
    appContext.phongShader->setUniform("material.albedo", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    appContext.phongShader->setUniform("material.shininess", 256.f);
    appContext.light->setupPointLight(*appContext.phongShader);

    appContext.springModel->render(*appContext.phongShader);

    appContext.pointShader->use();
    appContext.pointShader->setUniform("view", appContext.camera->getViewMatrix());
    appContext.pointShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    appContext.lightBulb->render(*appContext.pointShader);

    appContext.frameBufferManager->unbind();
}
