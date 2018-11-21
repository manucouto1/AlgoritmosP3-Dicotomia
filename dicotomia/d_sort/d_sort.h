#ifndef HEADER_DICOTOMIA
#define HEADER_DICOTOMIA
#include "../dicotomia.h"
#endif

#define TAM 128000
/* Registros CONCRETOS ORDENACION VECTORES */
typedef void (*typeIntVectorFunction)(int v[], int nargs);
//typedef void (*typefun)(void *);

typedef struct {
	sit_dico sit;
	typeIntVectorFunction func;
} sit_dico_sort;
/*
typedef struct {
	int
	int []
	monticulo *heap;
};
*/
typedef struct {
	alg_dico alg;
	sit_dico_sort situation[NUM_SITUATIONS];
	//typefun f;
	//void* extra;
	typeIntVectorFunction func;
} alg_dico_sort;
/*
void monticulo (void *arg) {
	struct info_mont *m=arg;

	m->heap;

}
*/
/* Registros CONCRETOS ORDENACION HEAP */
typedef struct {
	int vector[TAM];
	int ultimo;
}monticulo;

typedef void (*typeHeapFunction)(int [], int, monticulo *);

typedef struct {
	alg_dico alg;
	sit_dico_sort situation[NUM_SITUATIONS];
	typeHeapFunction func;
	monticulo heap;
} alg_dico_heap;




/* CONCRETE PROBLEM VECTOR SORTING API */
sit_dico_sort initStudyCase_s(char *name, typeIntVectorFunction ini);

char* initAlgorithems_v(alg_dico_sort algoritmos[]); // <-- Este se implementa en el main.c
alg_dico_sort initAlgorithem_v(char *name,typeIntVectorFunction func, sit_dico_sort sitDico[], int ini, int mult,
                               int fin, int nTemp);

void printAlgorithemAndSituation_s(alg_dico_sort algoritmos[]);
void mostrarCotas_s(alg_dico_sort algoritmo[]);
void buscarCotas_s(alg_dico_sort algoritmos[], cota_t cotas[], int numCotas);

/* Test Lectura Tiempos V */
void leerTiempo_v(alg_dico_sort algoritmo, sit_dico_sort situacion, time_dico tiempos[], int *tamV);
void lecturaTiempos_v(alg_dico_sort algoritmos[]);

void testTiempos_s(alg_dico_sort algoritmo[]);
void testBuscarCotas(alg_dico algoritmos[]);

void inicializar_semilla();
void aleatorio(int v [], int n);
void ascendente(int v [], int n);
void descendente (int v[], int n);


char * initAlgorithems_m(alg_dico_heap algoritmos[]);
alg_dico_heap initAlgorithem_m(char *name,typeHeapFunction func, sit_dico_sort sitDico[], int ini, int mult,
                               int fin, int nTemp);
/* Test Lectura Tiempos m */
void leerTiempo_m(alg_dico_heap algoritmo, sit_dico_sort situacion, time_dico tiempos[], int *tamV);
void lecturaTiempos_m(alg_dico_heap algoritmos[]);

void ini_monticulo(monticulo *);
void crear_monticulo(int [], int, monticulo *);
int eliminar_mayor(monticulo *);
void testHeap();
