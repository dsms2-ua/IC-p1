#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdint>
#include <chrono>

using namespace std;

// Estructura para almacenar píxeles RGB
struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

// Estructura para almacenar píxeles HSV
struct HSVPixel {
    float h; // Hue (0-360)
    float s; // Saturation (0-1)
    float v; // Value (0-1)
};

// Función para cargar los píxeles desde un archivo binario
bool loadImageFromBinary(const string& inputBinaryFile, int& width, int& height, vector<Pixel>& pixels) {
    ifstream inputFile(inputBinaryFile, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Error al abrir el archivo binario." << endl;
        return false;
    }

    // Leer el ancho y alto de la imagen
    inputFile.read(reinterpret_cast<char*>(&width), sizeof(width));
    inputFile.read(reinterpret_cast<char*>(&height), sizeof(height));

    // Leer los píxeles RGB
    pixels.resize(width * height);
    inputFile.read(reinterpret_cast<char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    inputFile.close();
    return true;
}

// Función para guardar los píxeles procesados en un archivo binario
bool saveImageToBinary(const string& outputBinaryFile, int width, int height, const vector<Pixel>& pixels) {
    ofstream outputFile(outputBinaryFile, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "Error al abrir el archivo binario para escribir." << endl;
        return false;
    }

    // Escribir el ancho y alto de la imagen
    outputFile.write(reinterpret_cast<const char*>(&width), sizeof(width));
    outputFile.write(reinterpret_cast<const char*>(&height), sizeof(height));

    // Escribir los píxeles RGB
    outputFile.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    outputFile.close();
    return true;
}

// Función para aplicar un filtro básico (convolución) con un kernel 3x3
void applyFilter3x3(vector<Pixel>& channel, int width, int height) {
    // Ejemplo de filtro 3x3: Media
    vector<Pixel> temp = channel;
    int kernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
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

// Función para aplicar un filtro más complejo (9x9)
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

// Función para aplicar un filtro muy complejo (18x18)
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

// Función para convertir un solo pixel de RGB a HSV 
HSVPixel rgbToHsvPixel(Pixel rgb) {
    float r = rgb.red / 255.0f;
    float g = rgb.green / 255.0f;
    float b = rgb.blue / 255.0f;

    float maxVal = max(r, max(g, b));
    float minVal = min(r, min(g, b));
    float delta = maxVal - minVal;

    HSVPixel hsv;
    hsv.v = maxVal;

    if (delta == 0) {
        hsv.h = 0;
        hsv.s = 0;
    } else {
        hsv.s = delta / maxVal;

        if (maxVal == r) {
            hsv.h = 60 * (fmod(((g - b) / delta), 6));
        } else if (maxVal == g) {
            hsv.h = 60 * (((b - r) / delta) + 2);
        } else {
            hsv.h = 60 * (((r - g) / delta) + 4);
        }
    }

    if (hsv.h < 0) hsv.h += 360;

    return hsv;
}

// Función para convertir la imagen completa de RGB a HSV
vector<HSVPixel> rgbToHsv(const vector<Pixel>& rgbPixels, int width, int height) {
    vector<HSVPixel> hsvPixels(width * height);
    for (int i = 0; i < width * height; ++i) {
        hsvPixels[i] = rgbToHsvPixel(rgbPixels[i]);
    }
    return hsvPixels;
}

// Función para convertir un solo pixel de HSV a RGB
Pixel hsvToRgbPixel(HSVPixel hsv) {
    float c = hsv.v * hsv.s;
    float x = c * (1 - fabs(fmod(hsv.h / 60.0, 2) - 1));
    float m = hsv.v - c;

    float r, g, b;

    if (0 <= hsv.h && hsv.h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (60 <= hsv.h && hsv.h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (120 <= hsv.h && hsv.h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (180 <= hsv.h && hsv.h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (240 <= hsv.h && hsv.h < 300) {
        r = x;
        g = 0;
        b = c;
    } else {
        r = c;
        g = 0;
        b = x;
    }

    Pixel rgb;
    rgb.red = (r + m) * 255;
    rgb.green = (g + m) * 255;
    rgb.blue = (b + m) * 255;

    return rgb;
}

vector<Pixel> hsvToRgb(const vector<HSVPixel>& hsvPixels, int width, int height) {
    vector<Pixel> rgbPixels(width * height);
    for (int i = 0; i < width * height; ++i) {
        rgbPixels[i] = hsvToRgbPixel(hsvPixels[i]);
    }
    return rgbPixels;
}

// Filtro para el canal Hue (desplazamiento en el espectro de colores)
void filterHue(vector<float>& hChannel, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        hChannel[i] += 30.0; // Desplazamiento de 30 grados en el círculo de color
        if (hChannel[i] > 360.0) {
            hChannel[i] -= 360.0; // Mantener el valor de hue entre 0 y 360
        }
    }
}

// Filtro para el canal Saturation (multiplicación para aumentar la saturación)
void filterSaturation(vector<float>& sChannel, int width, int height) {
    for (int i = 0; i < width * height; ++i) {
        sChannel[i] *= 1.2; // Incrementa la saturación en un 20%
        if (sChannel[i] > 1.0) {
            sChannel[i] = 1.0; // Mantener la saturación entre 0 y 1
        }
    }
}

// Filtro para el canal Value (promedio con un desenfoque 3x3)
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


// Función para separar canales RGB
void separateRgbChannels(const vector<Pixel>& rgbPixels, vector<Pixel>& rChannel, vector<Pixel>& gChannel, vector<Pixel>& bChannel, int width, int height) {
    rChannel.resize(width * height);
    gChannel.resize(width * height);
    bChannel.resize(width * height);

    for (int i = 0; i < width * height; ++i) {
        rChannel[i].red = rgbPixels[i].red;
        rChannel[i].green = 0;
        rChannel[i].blue = 0;

        gChannel[i].red = 0;
        gChannel[i].green = rgbPixels[i].green;
        gChannel[i].blue = 0;

        bChannel[i].red = 0;
        bChannel[i].green = 0;
        bChannel[i].blue = rgbPixels[i].blue;
    }
}

// Función para combinar los canales RGB
vector<Pixel> combineRgbChannels(const vector<Pixel>& rChannel, const vector<Pixel>& gChannel, const vector<Pixel>& bChannel, int width, int height) {
    vector<Pixel> rgbPixels(width * height);
    for (int i = 0; i < width * height; ++i) {
        rgbPixels[i].red = rChannel[i].red;
        rgbPixels[i].green = gChannel[i].green;
        rgbPixels[i].blue = bChannel[i].blue;
    }
    return rgbPixels;
}

// Función para separar canales HSV
void separateHsvChannels(const vector<HSVPixel>& hsvPixels, vector<float>& hChannel, vector<float>& sChannel, vector<float>& vChannel, int width, int height) {
    hChannel.resize(width * height);
    sChannel.resize(width * height);
    vChannel.resize(width * height);

    for (int i = 0; i < width * height; ++i) {
        hChannel[i] = hsvPixels[i].h;
        sChannel[i] = hsvPixels[i].s;
        vChannel[i] = hsvPixels[i].v;
    }
}

// Función para combinar los canales HSV
vector<HSVPixel> combineHsvChannels(const vector<float>& hChannel, const vector<float>& sChannel, const vector<float>& vChannel, int width, int height) {
    vector<HSVPixel> hsvPixels(width * height);
    for (int i = 0; i < width * height; ++i) {
        hsvPixels[i].h = hChannel[i];
        hsvPixels[i].s = sChannel[i];
        hsvPixels[i].v = vChannel[i];
    }
    return hsvPixels;
}


// Cargar y guardar imagen en binario (puedes reutilizar las funciones del código anterior)

int main() {
    string inputBinaryFile = "bin/input.bin";  // Archivo binario de entrada con la imagen en RGB
    string outputBinaryFile = "bin/output.bin";  // Archivo binario de salida con la imagen procesada en HSV
    string intermediateBinaryFile = "bin/intermediate.bin"; //Archivo binario con la imagen procesada en RGB
    int width, height;
    vector<Pixel> rgbPixels;

    auto start = chrono::high_resolution_clock::now();

    // 1. Leer la imagen RGB desde el archivo binario
    if (!loadImageFromBinary(inputBinaryFile, width, height, rgbPixels)) {
        cerr << "Error al cargar la imagen RGB desde el archivo binario." << endl;
        return -1;
    }
    cout << "Imagen leída correctamente" << endl;

    //2. Separar en canales RGB
    vector<Pixel> rChannel, gChannel, bChannel;
    separateRgbChannels(rgbPixels, rChannel, gChannel, bChannel, width, height);

    //3. Aplicar los filtros a cada uno de los canales RGB
    applyFilter3x3(rChannel, width, height);
    applyFilter9x9(gChannel, width, height);
    applyFilter18x18(bChannel, width, height);

    //4. Volver a juntar la imagen desde los canales
    rgbPixels = combineRgbChannels(rChannel, gChannel, bChannel, width, height);

    //5. Guardar la imagen intermedia
    if (!saveImageToBinary(intermediateBinaryFile, width, height, rgbPixels)) {
        cerr << "Error al guardar la imagen procesada en el archivo binario." << endl;
        return -1;
    }

    cout << "Imagen intermedia procesada y guardada exitosamente en " << intermediateBinaryFile << endl;

    // 6. Convertir la imagen de RGB a HSV
    vector<HSVPixel> hsvPixels = rgbToHsv(rgbPixels, width, height);

    //7. Separar en cada uno de los canales HSV
    vector<float> hChannel, sChannel, vChannel;
    separateHsvChannels(hsvPixels, hChannel, sChannel, vChannel, width, height);

    // 8. Aplicar diferentes filtros a los canales HSV
    filterHue(hChannel, width, height);
    filterSaturation(sChannel, width, height);
    filterValue(vChannel, hsvPixels, width, height);

    //9. Combinar los canales en la imagen HSV final
    hsvPixels = combineHsvChannels(hChannel, sChannel, vChannel, width, height);

    //10. Transformar la imagen de HSV a RGB de nuevo
    rgbPixels = hsvToRgb(hsvPixels, width, height);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end -start;

    //11. Guardar el resultado final
    if (!saveImageToBinary(outputBinaryFile, width, height, rgbPixels)) {
        cerr << "Error al guardar la imagen procesada en el archivo binario." << endl;
        return -1;
    }

    cout << "Imagen procesada y guardada exitosamente en " << outputBinaryFile << endl;



    cout << endl << endl << "Tiempo de ejecución: " << duration.count() << " segundos" << endl << endl << endl;

    return 0;
}
