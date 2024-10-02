#include "../include/filter3x3.h"
#include "../include/filter18x18.h"

void applyFilter18x18(vector<Pixel>& channel, int width, int height) {
    // Filtro 18x18
    vector<Pixel> temp = channel;
    int kernelSum = 324;  // Todos los valores son 1
    int kernel[18][18];

    // Inicializar el kernel con 1
    for (int i = 0; i < 18; ++i)
        for (int j = 0; j < 18; ++j)
            kernel[i][j] = 1;

    for (int y = 9; y < height - 9; ++y) {
        for (int x = 9; x < width - 9; ++x) {
            int sum = 0;
            for (int ky = -9; ky <= 9; ++ky) {
                for (int kx = -9; kx <= 9; ++kx) {
                    sum += temp[(y + ky) * width + (x + kx)].blue * kernel[ky + 9][kx + 9];
                }
            }
            channel[y * width + x].blue = sum / kernelSum;
        }
    }
}