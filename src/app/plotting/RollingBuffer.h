//
// Created by faliszewskii on 08.10.24.
//

#ifndef SPRING_SIMULATION_ROLLINGBUFFER_H
#define SPRING_SIMULATION_ROLLINGBUFFER_H

#include <cmath>
#include "imgui.h"

// utility structure for realtime plot
struct RollingBuffer {
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer() {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
        }
    }
};

#endif //SPRING_SIMULATION_ROLLINGBUFFER_H
