//
// Created by faliszewskii on 16.06.24.
//

#ifndef OPENGL_TEMPLATE_GUI_H
#define OPENGL_TEMPLATE_GUI_H


#include "../../app/AppContext.h"
#include "imgui.h"

class Gui {
    AppContext &appContext;

public:
    Gui(AppContext &appContext);

    void render();

    void renderLightUI(PointLight &light);

    void renderFpsCounter();

    void renderSimulationParameters();

    void renderFunctionPlots();

    void renderXPlots();

    void renderTrajectory();

    void renderFunctionParams(const std::string &text, SimpleFunc &func, int &i, int i1);
};


#endif //OPENGL_TEMPLATE_GUI_H
