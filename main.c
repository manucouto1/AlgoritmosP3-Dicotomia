#include <string.h>

#include "dicotomia/d_sort/persist_to_txt/readWriteData_s.h"
#include "dicotomia/d_hash_tables/persist_to_txt/readWriteData_h.h"


void mideTiemposCalculaCotas_v(){
	int nCotas;
	funcion funcs[NUM_FUNCT];
	alg_dico_sort algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	initFuncs(funcs);
	printFuncs(funcs);
	initAlgorithems_v(algoritmos);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 1000, 100000000);
	printCotas(cotasEstudio, nCotas);
	lecturaTiempos_v(algoritmos);
	buscarCotas_s(algoritmos, cotasEstudio, nCotas);

}

void mideTiemposPersisteTxt_v(){
	int nCotas;
	char *filePath;
	funcion funcs[NUM_FUNCT];
	alg_dico_sort algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	initFuncs(funcs);
	printFuncs(funcs);
	filePath = initAlgorithems_v(algoritmos);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 1000, 100000000);
	printCotas(cotasEstudio, nCotas);
	lecturaTiempos_v(algoritmos);
	cacheTimeData_s(algoritmos,filePath);
	free(cotasEstudio);
}

void leeTxtCalculaCotas_v(){
	int nCotas;
	funcion funcs[NUM_FUNCT];
	char *filePath;
	alg_dico_sort algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	initFuncs(funcs);
	printFuncs(funcs);
	filePath = initAlgorithems_v(algoritmos);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 1000, 100000000);
	printCotas(cotasEstudio, nCotas);
	loadCachedTime_s(algoritmos, filePath);
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

void mideTiemposCalculaCotas_m(){
	int nCotas;
	char * filePath;
	funcion funcs[NUM_FUNCT];
	alg_dico_heap algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	initFuncs(funcs);
	printFuncs(funcs);
	initCotas(funcs, cotasEstudio, &nCotas);
	sortCotas(cotasEstudio, &nCotas, 125, 16000);
	printCotas(cotasEstudio, nCotas);

	filePath = initAlgorithems_m(algoritmos);
	printAlgorithemAndSituation_s((alg_dico_sort*)algoritmos);
	//lecturaTiempos_m(algoritmos);
	//cacheTimeData_s((alg_dico_sort*)algoritmos,filePath);
	loadCachedTime_s((alg_dico_sort*)algoritmos,filePath);

	buscarCotas_s((alg_dico_sort*)algoritmos,cotasEstudio,nCotas);
}

int main() {

	//mideTiemposCalculaCotas_h();
	//mideTiemposCotasManual();
	//mideTiemposCalculaCotas_s();
	//mideTiemposPersisteTxt_s();
	//leeTxtCalculaCotas_s();
	//mideTiemposPersisteTxt_h();
	//leeTxtCalculaCotas_h();

	//testHeap();

	mideTiemposCalculaCotas_m();
	return 0;

}