#ifndef FILTER_HUE_H
#define FILTER_HUE_H

#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

struct HSVPixel {
    float h; // Hue (0-360)
    float s; // Saturation (0-1)
    float v; // Value (0-1)
};

void filterHue(vector<float>& hChannel, int width, int height);

#endif