#include "rtweekend.h"
#include "lista_chocable.h"
#include "chocable.h"
#include "esfera.h"
#include "color.h"
#include "camara.h"
#include "material.h"
#include "esfera_en_movimiento.h"
#include "bvh.h"
#include "textura.h"
#include "aarect.h"
#include "caja.h"
#include "medio_constante.h"

#include <ctime>
#include <cstdio>
#include<iostream>
#include <mpi.h>

using namespace std;

lista_chocable escena_aleatoria2(){
	lista_chocable mundo;
	auto material2 = make_shared<lambertiano>(color(0.4,0.3,0.1));
	mundo.agregar(make_shared<esfera>(punto3(-4,1,0),1.0,material2));
	mundo.agregar(make_shared<esfera>(punto3(4,1,0),1.0,material2));
	// mundo.agregar(make_shared<esfera>(punto3(0,1,0),1.0,material2));
	// return mundo;
	lista_chocable objetos;
	objetos.agregar(make_shared<nodo_bvh>(mundo,0,0));
	return objetos;

}

lista_chocable dos_esferas(){
	lista_chocable objetos;

	auto damas_tex = make_shared<textura_damas>(color(0.2,0.3,0.1), color(0.9, 0.9, 0.9));
	auto material = make_shared<lambertiano>(damas_tex);

	objetos.agregar(make_shared<esfera>(punto3(0,-10,0),10,material));
	objetos.agregar(make_shared<esfera>(punto3(0,10,0),10,material));

	return objetos;
}


lista_chocable dos_esferas_perlin(){
	lista_chocable objetos;

	auto perlin_tex = make_shared<textura_ruido>(4);
	auto material = make_shared<lambertiano>(perlin_tex);

	objetos.agregar(make_shared<esfera>(punto3(0,-1000,0),1000,material));
	objetos.agregar(make_shared<esfera>(punto3(0,2,0),2,material));

	return objetos;
}


lista_chocable tierra(){
	auto textura_tierra = make_shared<textura_imagen>("../mapa.jpg");
	auto superficie_tierra = make_shared<lambertiano>(textura_tierra);
	auto globo = make_shared<esfera>(punto3(0,0,0),2,superficie_tierra);

	return lista_chocable(globo);
}


lista_chocable luz_simple(){
	lista_chocable objetos;

	auto perlin_tex = make_shared<textura_ruido>(4);
	auto material = make_shared<lambertiano>(perlin_tex);

	objetos.agregar(make_shared<esfera>(punto3(0,-1000,0),1000,material));
	objetos.agregar(make_shared<esfera>(punto3(0,2,0),2,material));

	auto luz = make_shared<luz_difusa>(color(4,4,4));
	objetos.agregar(make_shared<rectangulo_xy>(3,5,1,3,-2,luz));

	
	return objetos;
}


lista_chocable caja_cornell(){
	lista_chocable objetos;

	auto rojo = make_shared<lambertiano>(color(.65,.05,.05));
	auto verde = make_shared<lambertiano>(color(.12,.45,.15));
	auto blanco = make_shared<lambertiano>(color(.73,.73,.73));
	
	auto luz = make_shared<luz_difusa>(color(15,15,15));

	// objetos.agregar(make_shared<rectangulo_yz>(0,555,0,555,555,verde));
	// objetos.agregar(make_shared<rectangulo_yz>(0,555,0,555,0,rojo));

	// objetos.agregar(make_shared<rectangulo_xz>(0,555,0,555,0,blanco));
	// objetos.agregar(make_shared<rectangulo_xz>(0,555,0,555,555,blanco));
	objetos.agregar(make_shared<rectangulo_xy>(0,555,0,555,555,blanco));

	objetos.agregar(make_shared<rectangulo_xz>(213,343,227,332,554,luz));

	shared_ptr<chocable> caja1 = make_shared<caja>(punto3(0,0,0),punto3(165,330,165),blanco);
	caja1 = make_shared<rotar_y>(caja1,15);
	caja1 = make_shared<trasladar>(caja1,vec3(265,0,295));
	// objetos.agregar(caja1);

	shared_ptr<chocable> caja2 = make_shared<caja>(punto3(0,0,0),punto3(165,165,165),blanco);
	caja2 = make_shared<rotar_y>(caja2,-18);
	caja2 = make_shared<trasladar>(caja2,vec3(130,0,65));

	// objetos.agregar(caja2);



	return objetos;
}


lista_chocable caja_cornell_humo(){
	lista_chocable objetos;

	auto rojo = make_shared<lambertiano>(color(.65,.05,.05));
	auto verde = make_shared<lambertiano>(color(.12,.45,.15));
	auto blanco = make_shared<lambertiano>(color(.73,.73,.73));
	
	auto luz = make_shared<luz_difusa>(color(7,7,7));

	objetos.agregar(make_shared<rectangulo_yz>(0,555,0,555,555,verde));
	objetos.agregar(make_shared<rectangulo_yz>(0,555,0,555,0,rojo));

	objetos.agregar(make_shared<rectangulo_xz>(0,555,0,555,0,blanco));
	objetos.agregar(make_shared<rectangulo_xz>(0,555,0,555,555,blanco));
	objetos.agregar(make_shared<rectangulo_xy>(0,555,0,555,555,blanco));

	objetos.agregar(make_shared<rectangulo_xz>(113,443,127,432,554,luz));

	shared_ptr<chocable> caja1 = make_shared<caja>(punto3(0,0,0),punto3(165,330,165),blanco);
	caja1 = make_shared<rotar_y>(caja1,15);
	caja1 = make_shared<trasladar>(caja1,vec3(265,0,295));

	shared_ptr<chocable> caja2 = make_shared<caja>(punto3(0,0,0),punto3(165,165,165),blanco);
	caja2 = make_shared<rotar_y>(caja2,-18);
	caja2 = make_shared<trasladar>(caja2,vec3(130,0,65));

	objetos.agregar(make_shared<medio_constante>(caja1,0.01,color(0,0,0)));
	objetos.agregar(make_shared<medio_constante>(caja2,0.01,color(1,1,1)));

	return objetos;
}

lista_chocable escena_final(){
	lista_chocable cajas1;
	auto suelo = make_shared<lambertiano>(color(0.48,0.83,0.53));
	
	const int cajas_por_lado = 20;
	for(int i = 0; i<cajas_por_lado; i++){
		for(int j = 0; j<cajas_por_lado; j++){
			auto w = 100.0;
			auto x0 = -1000.0 + i*w;
			auto z0 = -1000.0 + j*w;
			auto y0 = 0.0;
			auto x1= x0+w;
			auto y1 = double_aleatorio(1,101);
			auto z1 = z0+w;

			cajas1.agregar(make_shared<caja>(punto3(x0,y0,z0),punto3(x1,y1,z1),suelo));
		}
	}

	lista_chocable objetos;
	objetos.agregar(make_shared<nodo_bvh>(cajas1,0,1));

	auto luz = make_shared<luz_difusa>(color(7,7,7));
	objetos.agregar(make_shared<rectangulo_xz>(123,423,147,412,554,luz));

	auto centro1 = punto3(400,400,200);
	auto centro2 = centro1 + vec3(30,0,0);
	auto esfera_en_movimiento_material = make_shared<lambertiano>(color(0.7,0.3,0.1));

	objetos.agregar(make_shared<esfera_en_movimiento>(centro1,centro2,0,1,50,esfera_en_movimiento_material));
	objetos.agregar(make_shared<esfera>(punto3(260,150,45),50,make_shared<dialectrico>(1.5)));
	objetos.agregar(make_shared<esfera>(punto3(0,150,145),50,make_shared<metalico>(color(0.8,0.8,0.90),1.0)));
	// objetos.agregar(make_shared<esfera>(punto3(0,0,0),300,make_shared<metalico>(color(0.8,0.8,0.90),0.0)));
	
	auto borde = make_shared<esfera>(punto3(360,150,145),70,make_shared<dialectrico>(1.5));
	objetos.agregar(borde);
	objetos.agregar(make_shared<medio_constante>(borde,0.2,color(0.2,0.4,0.9)));
	borde = make_shared<esfera>(punto3(0,0,0),5000,make_shared<dialectrico>(1.5));
	objetos.agregar(make_shared<medio_constante>(borde,.0001,color(1,1,1)));

	auto material_tierra = make_shared<lambertiano>(make_shared<textura_imagen>("../mapa.jpg"));
	objetos.agregar(make_shared<esfera>(punto3(400,200,400),100,material_tierra));
	auto textura_perlin = make_shared<textura_ruido>(0.1);
	objetos.agregar(make_shared<esfera>(punto3(220,280,300),80,make_shared<lambertiano>(textura_perlin)));

	lista_chocable cajas2;
	auto blanco = make_shared<lambertiano>(color(.73,.73,.73));
	int cant = 1000;
	for(int j = 0; j< cant; j++){
		cajas2.agregar(make_shared<esfera>(punto3::aleatorio(0,165),10,blanco));
	}

	objetos.agregar(make_shared<trasladar>(make_shared<rotar_y>(make_shared<nodo_bvh>(cajas2,0.0,1.0),15),vec3(-100,270,395)));


	return objetos;
}

lista_chocable sistema_solar(){
	lista_chocable objetos;
	
	vec3 postierra = vec3(0,0,0);
	vec3 posluna = vec3(30,0,0);
	vec3 possol = vec3(15,50,60);
	
	// objetos.agregar(make_shared<esfera>(punto3(0,0,0),20,make_shared<luz_difusa>(make_shared<textura_imagen>("../planetas/sol.png"))));

	// auto borde = make_shared<esfera>(punto3(0,0,0),3000,make_shared<dialectrico>(1.5));
	// objetos.agregar(make_shared<medio_constante>(borde,.001,make_shared<textura_imagen>("../planetas/espacio.jpg")));
	// objetos.agregar(make_shared<medio_constante>(borde,.0001,color(1,1,1)));
	// objetos.agregar(tierra);
	// objetos.agregar(make_shared<esfera>(punto3(0,0,0),11,make_shared<dialectrico>(1.5)));
	auto tierra = make_shared<esfera>(postierra,10,make_shared<lambertiano>(make_shared<textura_imagen>("../planetas/tierra3.jpg")));
	auto tierra2 = make_shared<esfera>(postierra,10.07,make_shared<lambertiano>(make_shared<textura_imagen>("../planetas/tierra3.jpg")));
	auto luna  = make_shared<esfera>(posluna,4,make_shared<lambertiano>(make_shared<textura_imagen>("../planetas/luna.png")));
	auto luna2  = make_shared<esfera>(posluna,4.05,make_shared<lambertiano>(make_shared<textura_imagen>("../planetas/luna.png")));
	
	
	objetos.agregar(make_shared<esfera>(possol,4,make_shared<luz_difusa>(color(100,100,100))));
	// objetos.agregar(make_shared<rotar_y>(tierra,-45));
	objetos.agregar(tierra);
	objetos.agregar(make_shared<medio_constante>(tierra2,0.6,color(0,0,0.9)));
	objetos.agregar(luna);
	objetos.agregar(make_shared<medio_constante>(luna2,0.3,color(0,0,0)));
	
	return objetos;
}

lista_chocable escena_aleatoria(){
	lista_chocable mundo;
	// auto material_suelo = make_shared<lambertiano>(color(0.5,0.5,0.5));
	auto damas_tex = make_shared<textura_damas>(color(0.2,0.3,0.1), color(0.9, 0.9, 0.9));
	auto material_suelo = make_shared<lambertiano>(damas_tex);
	
	for(int x = -11; x<11; x++){
		for(int z = -11; z<11; z++){
			auto elegir_material = double_aleatorio();
			auto radio  = double_aleatorio(0.1,0.4);
			punto3 centro(x + 0.9 * double_aleatorio(),radio, z + 0.9*double_aleatorio());

			shared_ptr<material> material_esfera;

			if(elegir_material < 0.8){
				//difuso
				auto albedo = color::aleatorio() * color::aleatorio();
				material_esfera = make_shared<lambertiano>(albedo);
				auto centro2 = centro + vec3(0,double_aleatorio(0,0.5),0);
				mundo.agregar(make_shared<esfera_en_movimiento>(centro,centro2,0.0,1.0,0.2,material_esfera));
			}
			else if(elegir_material < 0.95){
				//metal
				auto albedo = color::aleatorio(0.5,1);
				auto aspereza = double_aleatorio(0,0.5);
				material_esfera = make_shared<metalico>(albedo,aspereza);
				mundo.agregar(make_shared<esfera>(centro,radio,material_esfera));
			}
			else{
				//vidrio
				material_esfera = make_shared<dialectrico>(1.5);
				mundo.agregar(make_shared<esfera>(centro,radio,material_esfera));
			
			}


		}
	}
	auto material1 = make_shared<dialectrico>(1.5);
	auto material2 = make_shared<lambertiano>(color(0.4,0.2,0.1));
	auto material3 = make_shared<metalico>(color(0.7,0.6,0.5),0.0);

	mundo.agregar(make_shared<esfera>(punto3(0,1,0),1.0,material1));
	mundo.agregar(make_shared<esfera>(punto3(-4,1,0),1.0,material2));
	mundo.agregar(make_shared<esfera>(punto3(4,1,0),1.0,material3));
	
	mundo.agregar(make_shared<esfera>(punto3(0,-1000,0),1000,material_suelo));
	
	// return mundo;

	lista_chocable objetos;
	objetos.agregar(make_shared<nodo_bvh>(mundo,0,1));
	return objetos;

	

}

color color_de_rayo(const rayo&r, const color& fondo, const chocable& mundo, int profundidad){
	registro_choque registro;

	if(profundidad<=0)
		return color(0,0,0);
	
	if(!mundo.choca(r,0.001,infinito,registro))
		return fondo;
	
	rayo reflejado;
	color atenuacion;
	color emitido = registro.material_ptr->emitido(registro.u,registro.v,registro.p);

	if(!registro.material_ptr->refleja(r,registro,atenuacion,reflejado))
		return emitido;
	
	return emitido + atenuacion * color_de_rayo(reflejado,fondo,mundo,profundidad-1);
}


color color_de_rayo(const rayo& r, const chocable& mundo, int profundidad){ 
	registro_choque registro;
	
	if(profundidad <= 0)
		return color(0,0,0);
	
	if(mundo.choca(r,0.001,infinito, registro)){
		//objetivo es un punto aleatorio dentro de la superficie de una esfera unitaria que 
		//es tangente a la otra esfera en el punto donde el rayo la toca
		rayo rayo_reflejado;
		color atenuacion;
		if(registro.material_ptr->refleja(r,registro,atenuacion,rayo_reflejado))
			//para mezclar los colores, los multiplico componente a componente
			//esto funciona porque las componentes siempre varian entre 0 y 1
			//entonces multiplicar dos colores siempre da como resultado otro color v�lido
			return atenuacion * color_de_rayo(rayo_reflejado,mundo, profundidad - 1);
		else return color(0,0,0);
	}
	
	//sino, dibujo el fondo
	//mezclar linealmente blanco y azul dependiendo del alto (coordenada y)
	vec3 direccion_unitaria = vector_unitario(r.direccion());
	//en direccion_unitaria, la componente en y se mueve entre -1 y 1
	//pero necesito que se mueva entre 0 y 1, asi que 
	//mapeo [-1,1] a [0,1] de la siguiente forma
	auto t = 0.5 * (direccion_unitaria.y() + 1.0);
	
	//truco de interpolacion lineal o lerp (linear interpolation)
	// blendedValue = (1 - t) * startValue + t.endValue,  con t entre 0 y 1
	return (1.0-t)*color(1.0,1.0,1.0) + t*color(0.5,0.7,1.0);
	// 1.0,1.0,1.0 es blanco
	// 0.5,0.7,1.0 es azul
}

void algoritmo(const int& ancho, const int& alto,  const color& fondo, const lista_chocable& mundo, const camara& cam, const int& profundidad_maxima, const int& muestras_por_pixel, shared_ptr<vector<color>> imagen){
	int contador =0;
	for (int j = alto -1; j >= 0; --j) {
		// cerr << "\rScanlines remaining: " << j << ' ' << flush;
		for (int i = 0; i < ancho; ++i) {
			
			color pixel_color(0,0,0);
				
			//el color de cada pixel es el promedio de los colores de ese pixel 
			//junto con los colores de los pixeles que lo rodean.
			//aqui solo se acumula la suma, pero en escribir_color, se divide
			//por la cantidad de muestras

			for(int s = 0; s < muestras_por_pixel; ++s){
				auto u = (i + double_aleatorio()) / (ancho-1);
				auto v = (j + double_aleatorio()) / (alto-1);
				rayo r = cam.get_rayo(u,v);
				pixel_color += color_de_rayo(r,fondo,mundo, profundidad_maxima);
			}
			imagen->at(contador)= pixel_color;
			contador++;
			// escribir_color(cout,pixel_color, muestras_por_pixel);
		}
	}

}






int main(int argc, char** argv) {
	
	// archivo y parametros

	freopen("out.ppm", "w", stdout);
	auto relacion_de_aspecto = 16.0 / 9.0;
	int ancho = 300;
	int muestras_por_pixel  = 10;
	int profundidad_maxima = 10;
	bool mostrar_ejes = false;

	//mundo

	lista_chocable mundo;
	punto3 mirar_desde;
	punto3 mirar_hacia;
	auto apertura = 0.0;
	auto fov_vertical = 40.0;
	color fondo(0,0,0);

	switch(6){
		case 1:
			mundo = escena_aleatoria();
			fondo = color(0.7,0.8,1.0);
			mirar_desde = punto3(13,2,3);
			mirar_hacia = punto3(0,0,0);
			fov_vertical = 20.0;
			apertura = 0.1;
			break;
		case 2:
			mundo = dos_esferas();
			fondo = color(0.7,0.8,1.0);
			mirar_desde = punto3(13,2,13);
			mirar_hacia = punto3(0,0,0);
			fov_vertical = 20.0;

			// relacion_de_aspecto = 1.0;
			// ancho = 100;
			// muestras_por_pixel = 201;
			break;
		case 3:
			mundo = dos_esferas_perlin();
			fondo = color(0.7,0.8,1.0);
			mirar_desde = punto3(13,2,3);
			mirar_hacia = punto3(0,0,0);
			fov_vertical = 20.0;
			break;
		case 4:
			mundo = tierra();
			fondo = color(0.7,0.8,1.0);
			mirar_desde = punto3(13,2,13);
			mirar_hacia = punto3(0,0,0);
			fov_vertical = 20.0;
			break;
		case 5:
			mundo = luz_simple();
			fondo = color(0.0,0.0,0.0);
			muestras_por_pixel = 400;
			mirar_desde = punto3(26,3,6);
			mirar_hacia = punto3(0,2,0);
			fov_vertical = 20.0;
			break;
		case 6:
			mundo = caja_cornell();
			relacion_de_aspecto = 1.0;
			ancho = 20;
			muestras_por_pixel = 10000;
			fondo = color(0,0,0);
			mirar_desde = punto3(278,278,-800);
			mirar_hacia = punto3(278,278,0);
			fov_vertical = 40.0;
			break;
		case 7:
			mundo = caja_cornell_humo();
			relacion_de_aspecto = 1.0;
			ancho = 300;
			muestras_por_pixel = 200;
			fondo = color(0,0,0);
			mirar_desde = punto3(278,278,-800);
			mirar_hacia = punto3(278,278,0);
			fov_vertical = 40.0;
			break;
		case 8:
			mundo = escena_final();
			relacion_de_aspecto = 1.0;
			ancho = 1000;
			muestras_por_pixel = 1000;
			fondo = color(0,0,0);
			mirar_desde = punto3(478,278,-600);
			mirar_hacia = punto3(278,278,0);
			fov_vertical = 40.0;
			break;
		default:
		case 9:
			mundo = sistema_solar();
			ancho = 600;
			relacion_de_aspecto = 1.0;
			profundidad_maxima = 200;
			muestras_por_pixel = 5000;
			fondo = color(0,0,0);
			mirar_desde = punto3(-11.5,-10,30);
			mirar_hacia = punto3(12,2,-7);
			fov_vertical = 70.0;
			break;
	}
	
	
	if(mostrar_ejes){
		mundo.agregar(make_shared<caja>(punto3(0,0,0),punto3(1000,10,10),make_shared<lambertiano>(color(1.0,1.0,1.0))));
		mundo.agregar(make_shared<caja>(punto3(0,0,0),punto3(10,1000,10),make_shared<lambertiano>(color(1.0,1.0,1.0))));
		mundo.agregar(make_shared<caja>(punto3(0,0,0),punto3(10,10,1000),make_shared<lambertiano>(color(1.0,1.0,1.0))));
	}
	
	const int alto= static_cast<int>(ancho / relacion_de_aspecto);
	cout << "P3\n" << ancho << ' ' << alto << "\n255\n";

	//camara

	vec3 vup(0,1,0);
	auto distancia_focal = 10;//(mirar_desde - mirar_hacia).longitud();
	camara cam(mirar_desde,mirar_hacia,vup,fov_vertical,relacion_de_aspecto,apertura,distancia_focal,0.0,1.0);


	struct struct_color{
		double e[3];
	};

	struct struct_imagen{
		double e[3];
		struct struct_color pixels[];
	};
	// programa

	time_t inicio_ejecucion,fin_ejecucion;
	time(&inicio_ejecucion);

	int process_rank, size_of_cluster=1;

	MPI_Datatype mpi_color, mpi_imagen;
	
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size_of_cluster);

	//crear tipo color
	MPI_Type_contiguous(3, MPI_DOUBLE, &mpi_color);
	MPI_Type_commit(&mpi_color);


	//crear tipo imagen
	MPI_Type_contiguous(1 + ancho*alto,mpi_color,&mpi_imagen);
	MPI_Type_commit(&mpi_imagen);

	//variables
	int tamanio = sizeof(struct_imagen) + (ancho*alto * sizeof(struct_color));
	// struct struct_imagen *simagen = (struct_imagen*)malloc(tamanio);
	struct struct_imagen *simagen = (struct struct_imagen*)malloc(tamanio);
	// simagen->vacio={0,0,0};
	if(simagen==NULL){
		cerr << "memory not allocated\n";
		exit(0);
	}
	// struct struct_imagen *simagenes = (struct_imagen*)malloc(size_of_cluster*tamanio);
	struct struct_imagen *simagenes = (struct struct_imagen*)calloc(size_of_cluster,tamanio);
	if(simagenes==NULL){
		cerr << "memory not allocated\n";
		exit(0);
	}
	shared_ptr<vector<color>> imagen = make_shared<vector<color>>(ancho*alto,color(0,0,0));

	cerr << "cluster size " << size_of_cluster << endl;
	cerr << "tamanio  = " << tamanio << endl;
	cerr << "struct color = " << sizeof(struct_color) << endl;
	cerr << "struct imagen = " << sizeof(struct_imagen) << endl;
	// cerr << "tam simagen2 = " << sizeof(&simagen) << endl;
	// cerr << "tam simagenes = " << sizeof(simagenes) << endl;

	//calcular cuantas muestras tendra cada worker
	int operaciones_por_hilo = muestras_por_pixel/size_of_cluster;
	vector<int> muestras(size_of_cluster,operaciones_por_hilo);
	int modulo =muestras_por_pixel%size_of_cluster; 
	if(modulo!=0){
		muestras.back()+=modulo;
	}

	//fork
	int cantidad_hilos = size_of_cluster;
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

	for(int i = 0; i < size_of_cluster; i++){
		if(i == process_rank){
	        fprintf(stderr,"Worker %d conectado. Me encargo de %d muestras\n", process_rank, muestras.at(process_rank));
	    }
	}

	//ejecutar algoritmo
	algoritmo(ancho,alto,fondo,mundo,cam,profundidad_maxima,muestras.at(process_rank),imagen);

	//copiar resultado de algorimo al struct
	for(int i=0; i<ancho*alto; i++){
		for(int j=0; j<3; j++){
			simagen->pixels[i].e[j]=imagen.get()->at(i)[j];
		}
	}

	for(int i = 0; i < size_of_cluster; i++){
		if(i == process_rank){
	        fprintf(stderr,"Worker %d finalizado.\n", process_rank);
	    }
	}

	//reunir todas las imagenes
	if(process_rank==0){
		MPI_Gather(simagen,1,mpi_imagen,simagenes,1,mpi_imagen,0,MPI_COMM_WORLD);
	}
	else{
		MPI_Gather(simagen,1,mpi_imagen,NULL,0,mpi_imagen,0,MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	//calcular promedio
	if(process_rank==0){
		color pixel_color;
		color aux;
		for(int p = 0; p<ancho*alto; p++){
			pixel_color= color(0,0,0);
			for(int i=0;i<cantidad_hilos;i++){
				aux= color(simagenes[i].pixels[p].e[0], 
						  		 simagenes[i].pixels[p].e[1],
						  		 simagenes[i].pixels[p].e[2]);
				// cerr << i << " " << aux.x() << " " <<  aux.y() <<" " << aux.z() << endl;
				pixel_color+=aux;
			}
		escribir_color(cout,pixel_color,muestras_por_pixel);
		}

		time(&fin_ejecucion);
		double tiempo_transcurrido = (double) fin_ejecucion - inicio_ejecucion;
		fprintf(stderr,"\nTiempo transcurrido: %.1f",tiempo_transcurrido);
		cerr << "\nDone.\n";


	}

	// delete[] simagenes;
	free(simagenes);
	free(simagen);
	MPI_Finalize();
	/*int cantidad_hilos = thread::hardware_concurrency();
	vector<thread> hilos(cantidad_hilos);
	vector<shared_ptr<vector<color>>> imagenes(cantidad_hilos);
	int operaciones_por_hilo = muestras_por_pixel/cantidad_hilos;
	vector<int> muestras(cantidad_hilos,operaciones_por_hilo);
	int modulo =muestras_por_pixel%cantidad_hilos; 
	if(modulo!=0){
		muestras.back()+=modulo;
	}

	int contador = 0;
	for(thread &h : hilos){
		imagenes.at(contador)=make_shared<vector<color>>(ancho*alto,color(0,0,0));
		h = thread(algoritmo,ancho,alto,fondo,mundo,cam,profundidad_maxima,muestras.at(contador),imagenes.at(contador));
		contador++;
	}
	for(thread &h : hilos)
		h.join();

	for(int p = 0; p<ancho*alto; p++){
		color pixel_color(0,0,0);
		for(int i=0;i<cantidad_hilos;i++){
			pixel_color+=imagenes.at(i)->at(p);
		}
		escribir_color(cout,pixel_color,muestras_por_pixel);
	}
	*/
	
}



