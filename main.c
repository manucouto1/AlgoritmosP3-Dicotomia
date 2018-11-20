#include <string.h>

#include "dicotomia/d_sort/persist_to_txt/readWriteData_s.h"
#include "dicotomia/d_hash_tables/persist_to_txt/readWriteData_h.h"


void mideTiemposCalculaCotas_s(){
	int nCotas;
	funcion funcs[NUM_FUNCT];
	alg_dico_sort algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	initFuncs(funcs);
	printFuncs(funcs);
	initAlgorithems_s(algoritmos);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 1000, 100000000);
	printCotas(cotasEstudio, nCotas);
	lecturaTiempos_s(algoritmos);
	buscarCotas_s(algoritmos, cotasEstudio, nCotas);

}

void mideTiemposPersisteTxt_s(){
	int nCotas;
	funcion funcs[NUM_FUNCT];
	alg_dico_sort algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	initFuncs(funcs);
	printFuncs(funcs);
	initAlgorithems_s(algoritmos);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 1000, 100000000);
	printCotas(cotasEstudio, nCotas);
	lecturaTiempos_s(algoritmos);
	cacheTimeData_s(algoritmos);
	free(cotasEstudio);
}

void leeTxtCalculaCotas_s(){
	int nCotas;
	funcion funcs[NUM_FUNCT];

	alg_dico_sort algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	initFuncs(funcs);
	printFuncs(funcs);
	initAlgorithems_s(algoritmos);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 1000, 100000000);
	printCotas(cotasEstudio, nCotas);
	loadCachedTime_s(algoritmos);
	buscarCotas_s(algoritmos, cotasEstudio, nCotas);
	free(cotasEstudio);
}

void mideTiemposCotasManual(){
	alg_dico_hash algoritmos[NUM_ALGORITHEMS];
	initAlgorithems_h_manual(algoritmos);
	lecturaTiempos_h(algoritmos);
	mostrarCotas_h(algoritmos);
}

void mideTiemposPersisteTxtCotasManual(){
	alg_dico_hash algoritmos[NUM_ALGORITHEMS];
	initAlgorithems_h_manual(algoritmos);
	lecturaTiempos_h(algoritmos);
	cacheTimeData_h(algoritmos);
}

void leeTxtCotasManual(){
	alg_dico_hash algoritmos[NUM_ALGORITHEMS];
	initAlgorithems_h_manual(algoritmos);
	loadCachedTime_h(algoritmos);
	mostrarCotas_h(algoritmos);
}

void mideTiemposCalculaCotas_h(){
	//COTAS
	int nCotas;
	funcion funcs[NUM_FUNCT];
	alg_dico_hash algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	tabla_cerrada d = malloc (MAX_N * sizeof(entrada));

	initFuncs(funcs);
	printFuncs(funcs);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 125, 16000);

	initAlgorithems_h(algoritmos);
	printAlgorithemAndSituation_h(algoritmos);
	lecturaTiempos_h(algoritmos);
	buscarCotas_h(algoritmos, cotasEstudio, nCotas);
	free(cotasEstudio);
}

void mideTiemposPersisteTxt_h(){
	//COTAS
	int nCotas;
	funcion funcs[NUM_FUNCT];
	alg_dico_hash algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	tabla_cerrada d = malloc (MAX_N * sizeof(entrada));

	initFuncs(funcs);
	printFuncs(funcs);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 125, 16000);
	printCotas(cotasEstudio, nCotas);

	initAlgorithems_h(algoritmos);
	printAlgorithemAndSituation_h(algoritmos);
	lecturaTiempos_h(algoritmos);
	cacheTimeData_h(algoritmos);

	free(cotasEstudio);
}

void leeTxtCalculaCotas_h(){
	//COTAS
	int nCotas;
	funcion funcs[NUM_FUNCT];
	alg_dico_hash algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	tabla_cerrada d = malloc (MAX_N * sizeof(entrada));

	initFuncs(funcs);
	printFuncs(funcs);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 125, 16000);
	printCotas(cotasEstudio, nCotas);

	initAlgorithems_h(algoritmos);
	printAlgorithemAndSituation_h(algoritmos);

	loadCachedTime_h(algoritmos);
	buscarCotas_h(algoritmos, cotasEstudio, nCotas);
	free(cotasEstudio);
}

int main() {

	//mideTiemposCalculaCotas_h();
	//mideTiemposCotasManual();
	//mideTiemposCalculaCotas_s();
	//mideTiemposPersisteTxt_s();
	//leeTxtCalculaCotas_s();
	//mideTiemposPersisteTxt_h();
	//leeTxtCalculaCotas_h();

	testHeap();
	return 0;

}