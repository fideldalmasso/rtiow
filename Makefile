all: normal distribuido

clean:
	rm -f rtiow_normal.bin rtiow_distribuido.bin

normal: main.cpp
	g++ main.cpp -o rtiow_normal.bin -I. -std=c++14 -lm -lstdc++ --pedantic-errors -O3 -lpthread

distribuido: main4.cpp
	mpicc main4.cpp -o rtiow_distribuido.bin -I. -std=c++14 -lm -lstdc++ --pedantic-errors -O3
