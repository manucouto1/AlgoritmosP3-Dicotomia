#include <string.h>

#include "dicotomia/d_vector/persist_to_txt/readWriteData_v.h"
#include "dicotomia/d_hash_tables/persist_to_txt/readWriteData_h.h"


void calculoCompAlgOrdenacion(){

	//COTAS
	int nCotas;
	funcion funcs[NUM_FUNCT];
	// ALGORITMOS que trabaja con VECTORES
	alg_dico_vector algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);
	// Inicializamos el muestrario de funciones
	initFuncs(funcs);
	//printFuncs(funcs);
	// Inicializamos los algoritmos y sus valores
	initAlgorithems_v(algoritmos);
	//printAlgorithemAndSituation_v(algoritmos);

	// Generamos las Posibles cotas
	initCotas(funcs, cotasEstudio, &nCotas);
	//printCotas(cotasEstudio, nCotas);

	// Ordenamos las Posibles Cotas basándonos en la pendiente de las funciones en un punto
	// representativo del intervalo en el que se realiza el estudio
	sortCotas(cotasEstudio, &nCotas, 1000, 100000000);
	printCotas(cotasEstudio, nCotas);

	// OBTENER TIEMPOS Y PERSISTIR A TXT
	//cargarTiemposEstaticos_v(algoritmos);
	//lecturaTiempos_v(algoritmos);
	//cacheTimeData_v(algoritmos);

	// CARGAR TIEMPOS DESDE TXT
	loadCachedTime_v(algoritmos);

	buscarCotas_v(algoritmos, cotasEstudio, nCotas);
	free(cotasEstudio);
}

void calculoCompHashingTables(){
	//COTAS
	int nCotas;
	funcion funcs[NUM_FUNCT];
	alg_dico_hash algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	tabla_cerrada d = malloc (MAX_N * sizeof(entrada));

	// Inicializamos el muestrario de funciones
	initFuncs(funcs);
	//printFuncs(funcs);

	// Generamos las Posibles cotas
	initCotas(funcs, cotasEstudio, &nCotas);

	// Ordenamos las Posibles Cotas basándonos en la pendiente de las funciones en un punto
	// representativo del intervalo en el que se realiza el estudio
	sortCotas(cotasEstudio, &nCotas, 125, 16000);
	printCotas(cotasEstudio, nCotas);

	// Inicializamos Algoritmos
	initAlgorithems_h(algoritmos);
	//printAlgorithemAndSituation_h(algoritmos);

	lecturaTiempos_h(algoritmos);
	//cacheTimeData_h(algoritmos);

	//loadCachedTime_h(algoritmos);

	buscarCotas_h(algoritmos, cotasEstudio, nCotas);
	free(cotasEstudio);
}


int main() {


	//printf("***TABLA CERRADA LINEAL\n");
	//test(11, resol_colision_lineal);
	//printf("***TABLA CERRADA CUADRATICA\n");
	//test(11, resol_colision_cuadratica);
	//printf("***TABLA CERRADA DOBLE\n");
	//test(11, resol_colision_exploracion_doble);
	calculoCompHashingTables();
	//calculoCompAlgOrdenacion();
	return 0;

}