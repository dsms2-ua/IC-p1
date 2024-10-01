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

// Función para convertir los datos de píxeles a un cv::Mat (imagen OpenCV)
cv::Mat convertPixelsToMat(int width, int height, const vector<Pixel>& pixels) {
    cv::Mat image(height, width, CV_8UC3);  // Crear una imagen de 8 bits con 3 canales (BGR)
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& pixel = pixels[y * width + x];
            image.at<cv::Vec3b>(y, x) = cv::Vec3b(pixel.blue, pixel.green, pixel.red);  // BGR en OpenCV
        }
    }

    return image;
}

int main() {
    string inputBinaryFile = "bin/intermediate.bin";  // Archivo binario con la imagen procesada
    int width, height;
    vector<Pixel> pixels;

    //Cargar la imagen intermedia
    if (loadImageFromBinary(inputBinaryFile, width, height, pixels)) {
        cout << "Imagen intermedia procesada cargada desde archivo binario. Ancho: " << width << ", Alto: " << height << endl;

        // Convertir los datos a un objeto cv::Mat
        cv::Mat image = convertPixelsToMat(width, height, pixels);

        // Guardar la imagen como PNG
        if (cv::imwrite("images/intermediate.png", image)) {
            cout << "Imagen guardada exitosamente como intermediate.png" << endl;
        } else {
            cerr << "Error al guardar la imagen." << endl;
        }
    } else {
        cerr << "Error al cargar la imagen desde el archivo binario." << endl;
    }

    inputBinaryFile = "bin/output.bin";

    // Cargar la imagen procesada desde el archivo binario
    if (loadImageFromBinary(inputBinaryFile, width, height, pixels)) {
        cout << "Imagen procesada cargada desde archivo binario. Ancho: " << width << ", Alto: " << height << endl;

        // Convertir los datos a un objeto cv::Mat
        cv::Mat image = convertPixelsToMat(width, height, pixels);

        // Guardar la imagen como PNG
        if (cv::imwrite("images/output.png", image)) {
            cout << "Imagen guardada exitosamente como output.png" << endl;
        } else {
            cerr << "Error al guardar la imagen." << endl;
        }
    } else {
        cerr << "Error al cargar la imagen desde el archivo binario." << endl;
    }

    return 0;
}
