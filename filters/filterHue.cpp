#include "../include/filterHue.h"

void filterHue(vector<float>& hChannel, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        hChannel[i] += 30.0; // Desplazamiento de 30 grados en el círculo de color
        if (hChannel[i] > 360.0) {
            hChannel[i] -= 360.0; // Mantener el valor de hue entre 0 y 360
        }
    }
}