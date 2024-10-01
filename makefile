# Variables
CXX = g++
CXXFLAGS = -O2 -std=c++11      # Opciones de compilación estándar
LDFLAGS =                      # Opciones de enlace (por ahora vacías)
BINARY_DIR = ./bin             # Directorio para los archivos binarios
SOURCES = cargar_imagen.cpp procesar_filtros.cpp
OBJECTS = $(SOURCES:.cpp=.o)   # Convertir archivos .cpp a .o

# Objetivo por defecto
all: cargar procesar

# Compilar cargar_imagen.cpp
cargar: cargar_imagen.o
	$(CXX) $(CXXFLAGS) -o cargar_imagen cargar_imagen.o $(LDFLAGS)

# Compilar procesar_filtros.cpp
procesar: procesar_filtros.o
	$(CXX) $(CXXFLAGS) -o procesar_filtros procesar_filtros.o $(LDFLAGS)

# Regla para compilar archivos .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -f *.o cargar_imagen procesar_filtros

# Ejecución de los binarios en orden
run: cargar procesar
	./cargar_imagen input.png $(BINARY_DIR)/image_data.bin
	./procesar_filtros $(BINARY_DIR)/image_data.bin $(BINARY_DIR)/output_data.bin
