GPP=g++
CXXFLAGS=  -fshow-column -fno-diagnostics-show-caret  -g2 -Wall -O0    -D_DEBUG -std=c++14
LDFLAGS=     
OBJS=Debug/main.o 

all: Debug Debug/rtiow.bin

clean:
	rm -rf ${OBJS} Debug/rtiow.bin

Debug/rtiow.bin: ${OBJS}
	${GPP} ${OBJS} ${LDFLAGS} -o $@

Debug:
	mkdir Debug

Debug/main.o: main.cpp rtweekend.h vec3.h rayo.h chocable.h aabb.h lista_chocable.h esfera.h color.h camara.h material.h esfera_en_movimiento.h bvh.h
	${GPP} ${CXXFLAGS} -c main.cpp -o $@

