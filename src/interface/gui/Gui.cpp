//
// Created by faliszewskii on 16.06.24.
//

#include <glm/gtc/type_ptr.hpp>
#include "Gui.h"
#include "imgui.h"
#include "implot.h"

Gui::Gui(AppContext &appContext) : appContext(appContext) {}

void Gui::render() {
//    ImGui::ShowDemoWindow();
    ImPlot::ShowDemoWindow();
    renderSimulationParameters();

    if(ImGui::Begin("Plots")) {
        renderFunctionPlots();
        renderXPlots();
        ImGui::End();
    }
    if(ImGui::Begin("Trajectory")) {
        renderTrajectory();
        ImGui::End();
    }
}

void Gui::renderLightUI(PointLight &light) {
    ImGui::ColorPicker3("Light Color", glm::value_ptr(light.color));
    ImGui::DragFloat3("Light Position", glm::value_ptr(light.position), 0.001f);
}

void Gui::renderFpsCounter() {
    auto &io = ImGui::GetIO();
    const static auto getter = [](void* data, int i){return static_cast<float*>(data)[i];};
    static float delta = 0;
    delta += io.DeltaTime;
    if(delta > 0.2) {
        delta = 0;
        appContext.rollingMspf.first = (appContext.rollingMspf.first + 1) % appContext.rollingMspf.second.size();
        appContext.rollingMspf.second[appContext.rollingMspf.first] = io.Framerate ? 1000.0f / io.Framerate : 0.0f;
    }
    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
}

void Gui::renderSimulationParameters() {
    ImGui::Begin("Simulation Parameters");
    if(ImGui::CollapsingHeader("Light")) {
        renderLightUI(*appContext.light);
    }
    renderFpsCounter();
    if(ImGui::Button(appContext.running? "Stop": "Start"))
        appContext.running = !appContext.running;
    ImGui::SameLine();
    if(ImGui::Button("Reset")) {
        appContext.springSimulation->reset();
        appContext.springModel->updateX(appContext.springSimulation->springState.x - appContext.wFunc.f(0));
        appContext.springModel->updateHeight(0.5f - appContext.wFunc.f(0));
        appContext.plotF.Erase();
        appContext.plotG.Erase();
        appContext.plotH.Erase();
        appContext.plotW.Erase();
        appContext.functionPlotHistoricalMax = 0;
        appContext.plotX.Erase();
        appContext.plotXt.Erase();
        appContext.plotXtt.Erase();
        appContext.xPlotHistoricalMax = 0;
        appContext.plotTrajectory.Erase();
        appContext.trajectoryHistoricalMax = 0;
    }
    if(ImGui::CollapsingHeader("State", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Kinematics");
        ImGui::Text("x:    %s  %.3f", std::signbit(appContext.springSimulation->springState.x)?"":" ", appContext.springSimulation->springState.x);
        ImGui::Text("xt:   %s  %.3f", std::signbit(appContext.springSimulation->springState.v)?"":" ", appContext.springSimulation->springState.v);
        ImGui::Text("xtt:  %s  %.3f", std::signbit(appContext.springSimulation->xtt())?"":" ", appContext.springSimulation->xtt());

        ImGui::Text("Forces");
        ImGui::Text("f(t): %s  %.3f", std::signbit(appContext.springSimulation->f())?"":" ", appContext.springSimulation->f());
        ImGui::Text("g(t): %s  %.3f", std::signbit(appContext.springSimulation->g())?"":" ", appContext.springSimulation->g());
        ImGui::Text("h(t): %s  %.3f", std::signbit(appContext.springSimulation->h())?"":" ", appContext.springSimulation->h());

        ImGui::Text("Other");
        ImGui::Text("w(t): %s  %.3f", std::signbit(appContext.springSimulation->w())?"":" ", appContext.springSimulation->w());

    }
    if(ImGui::CollapsingHeader("Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat("x0", &appContext.springSimulation->startCondition.x, 0.01, -1, 1);
        ImGui::DragFloat("v0", &appContext.springSimulation->startCondition.v, 0.01, -2, 2);
        ImGui::DragFloat("dt(ms)", &appContext.springSimulation->timeStepMs, 0.01, 0.1, 20);
        ImGui::DragFloat("m", &appContext.springSimulation->weightMass, 0.01, 0.1, 10);
        ImGui::DragFloat("c", &appContext.springSimulation->elasticityCoeff, 0.01, 0.1, 10);
        ImGui::DragFloat("k", &appContext.springSimulation->dampingCoeff, 0.01, 0.1, 10);

        static int type1 = 0, type2 = 0;
        renderFunctionParams("h(t)", appContext.hFunc, type1, 0);
        renderFunctionParams("w(t)", appContext.wFunc, type2, 1);
    }
    ImGui::End();
}

void Gui::renderFunctionPlots() {
    static float history = 10.0f;
    appContext.plotF.Span = history;
    appContext.plotG.Span = history;
    appContext.plotH.Span = history;
    appContext.plotW.Span = history;
    static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
    if (ImPlot::BeginPlot("Functions##Functions", ImVec2(-1,300))) {
        ImPlot::SetupAxes("t (s)", "F (N)");
        ImPlot::SetupAxisLimits(ImAxis_X1,0,history, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-appContext.functionPlotHistoricalMax, appContext.functionPlotHistoricalMax, ImGuiCond_Always);
        if(!appContext.plotF.Data.empty()) {
            ImPlot::PlotLine("f(t)", &appContext.plotF.Data[0].x, &appContext.plotF.Data[0].y, appContext.plotF.Data.size(), 0, 0, 2 * sizeof(float));
            ImPlot::PlotLine("g(t)", &appContext.plotG.Data[0].x, &appContext.plotG.Data[0].y, appContext.plotG.Data.size(), 0, 0, 2 * sizeof(float));
            ImPlot::PlotLine("h(t)", &appContext.plotH.Data[0].x, &appContext.plotH.Data[0].y, appContext.plotH.Data.size(), 0, 0, 2 * sizeof(float));
            ImPlot::PlotLine("w(t)", &appContext.plotW.Data[0].x, &appContext.plotW.Data[0].y, appContext.plotW.Data.size(), 0, 0, 2 * sizeof(float));
        }
        ImPlot::EndPlot();
    }
}

void Gui::renderXPlots() {
    static float history = 10.0f;
    appContext.plotX.Span = history;
    appContext.plotXt.Span = history;
    appContext.plotXtt.Span = history;
    if (ImPlot::BeginPlot("Kinematics##Kinematics", ImVec2(-1,300))) {
        ImPlot::SetupAxes("t (s)", nullptr);
        ImPlot::SetupAxisLimits(ImAxis_X1,0,history, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-appContext.xPlotHistoricalMax, appContext.xPlotHistoricalMax, ImGuiCond_Always);
        if(!appContext.plotX.Data.empty()) {
            ImPlot::PlotLine("x(t)", &appContext.plotX.Data[0].x, &appContext.plotX.Data[0].y, appContext.plotX.Data.size(), 0, 0, 2 * sizeof(float));
            ImPlot::PlotLine("xt(t)", &appContext.plotXt.Data[0].x, &appContext.plotXt.Data[0].y, appContext.plotXt.Data.size(), 0, 0, 2 * sizeof(float));
            ImPlot::PlotLine("xtt(t)", &appContext.plotXtt.Data[0].x, &appContext.plotXtt.Data[0].y, appContext.plotXtt.Data.size(), 0, 0, 2 * sizeof(float));
        }
        ImPlot::EndPlot();
    }

}

void Gui::renderTrajectory() {

    static float history = 3.0f;
    appContext.plotTrajectory.MaxSize = 250;

    if (ImPlot::BeginPlot("Trajectory", ImVec2(450,450))) {
        ImPlot::SetupAxes("x (m)", "v (m/s)");
        ImPlot::SetupAxisLimits(ImAxis_X1,-appContext.trajectoryHistoricalMax, appContext.trajectoryHistoricalMax, ImGuiCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1,-appContext.trajectoryHistoricalMax, appContext.trajectoryHistoricalMax, ImGuiCond_Always);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.5f);
        if(!appContext.plotTrajectory.Data.empty())
            ImPlot::PlotLine("", &appContext.plotTrajectory.Data[0].x, &appContext.plotTrajectory.Data[0].y, appContext.plotTrajectory.Data.size(), 0, appContext.plotTrajectory.Offset, 2 * sizeof(float));
        ImPlot::EndPlot();
    }
}

void Gui::renderFunctionParams(const std::string &text, SimpleFunc &func, int &i, int id) {
    static const char* const types[] = {
            "None",
            "Constant",
            "Step",
            "Signum",
            "Sinus"
    };
    ImGui::Text("%s parameters", text.c_str());
    if(ImGui::Combo(("Type##"+std::to_string(id)).c_str(), &i, types, 5))
        func.type = static_cast<SimpleFunc::Type>(i);
    ImGui::DragFloat(("Constant##"+std::to_string(id)).c_str(), &func.constant, 0.01, -10, 10);
    ImGui::DragFloat(("Step time##"+std::to_string(id)).c_str(), &func.stepTime, 0.01, 0.0, 10);
    ImGui::DragFloat(("Angle speed##"+std::to_string(id)).c_str(), &func.angleSpeed, 0.01, -10, 10);
    ImGui::DragFloat(("Phase##"+std::to_string(id)).c_str(), &func.phase, 0.01, -10, 10);

}
