#ifndef HEADER_DICOTOMIA
#define HEADER_DICOTOMIA
#include "../dicotomia.h"
#endif

#define TAM 128000

typedef void (*typeIntVectorFunction)(int v[], int nargs);

/* Registros CONCRETOS ORDENACION VECTORES */
typedef struct {
	int vector[TAM];
	int ultimo;
}monticulo;

typedef struct {
	sit_dico sit;
	typeIntVectorFunction func;
} sit_dico_sort;

typedef struct {
	alg_dico alg;
	sit_dico_sort situation[NUM_SITUATIONS];
	typeIntVectorFunction func;
} alg_dico_sort;

/* CONCRETE PROBLEM VECTOR SORTING API */
alg_dico_sort initAlgorithems_s(alg_dico_sort algoritmos[]); // <-- Este se implementa en el main.c
sit_dico_sort initStudyCase_s(char *name, typeIntVectorFunction ini);
alg_dico_sort initAlgorithem_s(char *name,typeIntVectorFunction func, sit_dico_sort sitDico[], int ini, int mult,
                               int fin, int nTemp);
void printAlgorithemAndSituation_s(alg_dico_sort algoritmos[]);
void mostrarCotas_s(alg_dico_sort algoritmo[]);
void buscarCotas_s(alg_dico_sort algoritmos[], cota_t cotas[], int numCotas);

/* Test Lectura Tiempos */
void leerTiempo_s(alg_dico_sort algoritmo, sit_dico_sort situacion, time_dico tiempos[], int *tamV);
void lecturaTiempos_s(alg_dico_sort algoritmos[]);

void testTiempos_s(alg_dico_sort algoritmo[]);
void cargarTiemposEstaticos_s(alg_dico_sort algoritmo[]);
void testBuscarCotas(alg_dico algoritmos[]);

void inicializar_semilla();
void aleatorio(int v [], int n);
void ascendente(int v [], int n);
void descendente (int v[], int n);

void ini_monticulo(monticulo *);
void crear_montuculo(int [], int, monticulo *);
int eliminar_mayor(monticulo *);
void testHeap();
