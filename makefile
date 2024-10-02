# Variables
CXX = g++
CXXFLAGS = -O2 -std=c++11# Opciones de compilación estándar

#Compilar applyFilters
filters:
	$(CXX) $(CXXFLAGS) -o applyFilters applyFilters.cpp

clean:
	rm -f applyFilters


# Ejecución de los binarios en orden
run: filters
	./readImage images/input.jpg
	./applyFilters
	./saveImage
