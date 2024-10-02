#include "../include/filterHue.h"
#include "../include/filterValue.h"

void filterValue(vector<float>& vChannel, vector<HSVPixel>& hsvPixels, int width, int height) {
    vector<float> temp = vChannel; // Copiar el canal V para procesar

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            float sum = 0.0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    sum += temp[(y + ky) * width + (x + kx)]; // Sumar los valores de V en el vecindario
                }
            }
            vChannel[y * width + x] = sum / 9.0; // Promedio 3x3
        }
    }

    // Actualizar el canal V en hsvPixels después del filtrado
    for (int i = 0; i < width * height; ++i) {
        hsvPixels[i].v = vChannel[i];
    }
}