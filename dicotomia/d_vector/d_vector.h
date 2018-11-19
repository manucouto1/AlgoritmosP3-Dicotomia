#ifndef HEADER_DICOTOMIA
#define HEADER_DICOTOMIA
#include "../dicotomia.h"
#endif
typedef void (*typeIntVector)(int v[], int nargs);

/* Registros CONCRETOS ORDENACION VECTORES */

typedef struct {
	sit_dico sit;
	typeIntVector func;
} sit_dico_vector;

typedef struct {
	alg_dico alg;
	sit_dico_vector situation[NUM_SITUATIONS];
	typeIntVector func;
} alg_dico_vector;

/* CONCRETE PROBLEM VECTOR SORTING API */
alg_dico_vector initAlgorithems_v(alg_dico_vector algoritmos[]); // <-- Este se implementa en el main.c
sit_dico_vector initStudyCase_v(char *name, typeIntVector ini);
alg_dico_vector initAlgorithem_v(char *name,typeIntVector func, sit_dico_vector sitDico[], int ini, int mult,
                               int fin, int nTemp);
void printAlgorithemAndSituation_v(alg_dico_vector algoritmos[]);
void mostrarCotas_v(alg_dico_vector algoritmo[]);
void buscarCotas_v(alg_dico_vector algoritmos[], cota_t cotas[], int numCotas);

/* Test Lectura Tiempos */
void leerTiempo_v(alg_dico_vector algoritmo, sit_dico_vector situacion, time_dico tiempos[], int *tamV);
void lecturaTiempos_v(alg_dico_vector algoritmos[]);

void testTiempos_v(alg_dico_vector algoritmo[]);
void cargarTiemposEstaticos_v(alg_dico_vector algoritmo[]);
void testBuscarCotas(alg_dico algoritmos[]);
