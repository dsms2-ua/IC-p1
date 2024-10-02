#ifndef FILTER_3X3_H
#define FILTER_3X3_H

#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

void applyFilter3x3(vector<Pixel>& channel, int width, int height);

#endif