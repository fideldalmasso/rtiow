all: normal distribuido

clean:
	rm -f rtiow_normal.bin rtiow_distribuido.bin

normal: main1.cpp
	g++ main1.cpp -o rtiow_normal.bin -I. -std=c++14 -lm -lstdc++ --pedantic-errors -O3 -lpthread

distribuido: main2.cpp
	mpicc main2.cpp -o rtiow_distribuido.bin -I. -std=c++14 -lm -lstdc++ --pedantic-errors -g 
