#include "../include/filterHue.h"
#include "../include/filterSaturation.h"

void filterSaturation(vector<float>& sChannel, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        sChannel[i] *= 1.2; // Incrementa la saturación en un 20%
        if (sChannel[i] > 1.0) {
            sChannel[i] = 1.0; // Mantener la saturación entre 0 y 1
        }
    }
}