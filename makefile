# Variables
CXX = g++
CXXFLAGS = -O3 -std=c++11# Opciones de compilación estándar


#Aquí ponemos todas las configuraciones adicionales del compilador
OPTa = -march=core-avx2 #Cambiar a i7
OPTb = -masm=intel
OPTc = -mtune=intel

OPTd = -fsimd-cost-model=dynamic
OPTe = -funroll-loops

filters: filter3x3 filter9x9 filter18x18 filterHue filterSaturation filterValue
	$(CXX) -c applyFilters.cpp -o o/applyFilters.o
	$(CXX) o/applyFilters.o o/filter3x3.o o/filter9x9.o o/filter18x18.o o/filterHue.o o/filterSaturation.o o/filterValue.o -o applyFilters

all: filter3x3 filter9x9 filter18x18 filterHue filterSaturation filterValue filters

filter3x3:
	$(CXX) $(CXXFLAGS) $(OPTa) $(OPTb) $(OPTc) -c filters/filter3x3.cpp -o o/filter3x3.o

filter9x9:
	$(CXX) $(CXXFLAGS) $(OPTa) $(OPTd) $(OPTe) -c filters/filter9x9.cpp -o o/filter9x9.o

filter18x18:
	$(CXX) $(CXXFLAGS) $(OPTa) $(OPTb) -c filters/filter18x18.cpp -o o/filter18x18.o

filterHue:
	$(CXX) $(CXXFLAGS) $(OPTa) $(OPTc) -c filters/filterHue.cpp -o o/filterHue.o

filterSaturation:
	$(CXX) $(CXXFLAGS) $(OPTe) -c filters/filterSaturation.cpp -o o/filterSaturation.o

filterValue:	
	$(CXX) $(CXXFLAGS) $(OPTd) $(OPTc) -c filters/filterValue.cpp -o o/filterValue.o

clean:
	rm -f applyFilters o/*.o


# Ejecución de los binarios en orden
run:
	./readImage images/input.jpg
	./applyFilters
	./saveImage