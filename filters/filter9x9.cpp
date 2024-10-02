#include "../include/filter3x3.h"
#include "../include/filter9x9.h"

void applyFilter9x9(vector<Pixel>& channel, int width, int height) {
    // Filtro 9x9
    vector<Pixel> temp = channel;
    int kernelSum = 81;  // Todos los valores son 1
    int kernel[9][9];

    // Inicializar el kernel con 1
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            kernel[i][j] = 1;

    for (int y = 4; y < height - 4; ++y) {
        for (int x = 4; x < width - 4; ++x) {
            int sum = 0;
            for (int ky = -4; ky <= 4; ++ky) {
                for (int kx = -4; kx <= 4; ++kx) {
                    sum += temp[(y + ky) * width + (x + kx)].green * kernel[ky + 4][kx + 4];
                }
            }
            channel[y * width + x].green = sum / kernelSum;
        }
    }
}