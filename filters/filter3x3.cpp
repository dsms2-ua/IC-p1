#include "../include/filter3x3.h"

void applyFilter3x3(vector<Pixel>& channel, int width, int height) {
    // Ejemplo de filtro 3x3: Media
    vector<Pixel> temp = channel;
    int kernel[3][3] = {{0, 0, 0}, {-1, 1, 0}, {0, 0, 0}};
    int kernelSum = 9;

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int sum = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    sum += temp[(y + ky) * width + (x + kx)].red * kernel[ky + 1][kx + 1];
                }
            }
            channel[y * width + x].red = sum / kernelSum;
        }
    }
}