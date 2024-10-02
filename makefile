# Variables
CXX = g++
CXXFLAGS = -O2 -std=c++11# Opciones de compilación estándar

filters: filter3x3 filter9x9 filter18x18 filterHue filterSaturation filterValue
	g++ -c applyFilters.cpp -o o/applyFilters.o
	g++ o/applyFilters.o o/filter3x3.o o/filter9x9.o o/filter18x18.o o/filterHue.o o/filterSaturation.o o/filterValue.o -o applyFilters

all: filter3x3 filter9x9 filter18x18 filterHue filterSaturation filterValue filters

filter3x3:
	g++ -c filters/filter3x3.cpp -o o/filter3x3.o

filter9x9:
	g++ -c filters/filter9x9.cpp -o o/filter9x9.o

filter18x18:
	g++ -c filters/filter18x18.cpp -o o/filter18x18.o

filterHue:
	g++ -c filters/filterHue.cpp -o o/filterHue.o

filterSaturation:
	g++ -c filters/filterSaturation.cpp -o o/filterSaturation.o

filterValue:	
	g++ -c filters/filterValue.cpp -o o/filterValue.o

clean:
	rm -f applyFilters o/*.o


# Ejecución de los binarios en orden
run:
	./readImage images/input.jpg
	./applyFilters
	./saveImage
