#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

bool readImageAndSaveAsBinary(const string& imagePath, const string& outputBinaryFile) {
    // Leer la imagen utilizando OpenCV
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        cerr << "Error al leer la imagen." << endl;
        return false;
    }

    int width = image.cols;
    int height = image.rows;

    // Convertir la imagen a un array de píxeles RGB
    vector<Pixel> pixels(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cv::Vec3b color = image.at<cv::Vec3b>(y, x);
            pixels[y * width + x] = {color[2], color[1], color[0]};  // R, G, B (OpenCV usa BGR)
        }
    }

    // Guardar los píxeles en un archivo binario
    ofstream outputFile(outputBinaryFile, ios::binary);
    if (!outputFile.is_open()) {
        cerr << "Error al abrir el archivo binario para escribir." << endl;
        return false;
    }

    // Escribir el ancho y alto de la imagen
    outputFile.write(reinterpret_cast<const char*>(&width), sizeof(width));
    outputFile.write(reinterpret_cast<const char*>(&height), sizeof(height));

    // Escribir los píxeles
    outputFile.write(reinterpret_cast<const char*>(pixels.data()), pixels.size() * sizeof(Pixel));

    outputFile.close();
    return true;
}

int main(int argc, char* argv[]) {
    string imagePath = argv[1];          // Ruta de la imagen PNG
    string outputBinaryFile = "bin/input.bin";  // Archivo binario donde se guardarán los datos

    if (readImageAndSaveAsBinary(imagePath, outputBinaryFile)) {
        cout << "Imagen leída y guardada exitosamente en " << outputBinaryFile << endl;
    } else {
        cerr << "Error al procesar la imagen." << endl;
    }

    return 0;
}
