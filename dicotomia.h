#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Constantes Algoritmo Dicotomia
#define UMBRAL_DICO 10

#define NUM_SITUATIONS 3
#define NUM_FUNCT 5
#define NUM_ALGORITHEMS 2

 /* Registros DICOTOMIA */

typedef struct {
	char name[256];
	int isComplex;
	int index;
} funcion;

typedef  struct {
	funcion cota;
	double exp;
}cota_t;

typedef struct {
	int is_under_500;
	double tiempo;
} time_dico;

typedef struct {
	cota_t sobre;
	cota_t ajus;
	cota_t sub;
	time_dico tiempos[256];
	int valN[256];
	void (*func)(int v[], int nargs);
	char sit_name[256];
} sit_dico;

typedef struct {
	sit_dico situation[NUM_SITUATIONS];
	int ini;
	int fin;
	int mult;
	int nTemp;
	void (*func)(int v[], int nargs);
	char alg_name[256];
} alg_dico;

/* FIN REGISTROS DICOTOMIA  */

double execute(funcion op , int n, double exp, int derivada);

void initFuncs(funcion *funcs);
void printFuncs(funcion *funcs);
void initCotas(funcion funcs[], cota_t *cotas, int *nCotas);
void sortCotas(cota_t *cotas, int *nCotas, int puntoInicial, int puntoFinal);
void printCotas(cota_t *cotas, int nCotas);

alg_dico initAlgorithems(alg_dico *algoritmos);
sit_dico initStudyCase(char *name, void (*ini)(int [], int));
alg_dico initAlgorithem(char *name, void (*func)(int [], int), sit_dico sitDico[], int ini, int mult,
                        int fin, int nTemp);
void printAlgorithemSituation(alg_dico *algoritmos);

//void acotarComplejidad(alg_dico *algoritmos, cota_t *cotas);
void buscarCotas(alg_dico *algoritmos, cota_t *cotas, int numCotas);


/* Test Lectura Tiempos */
void leerTiempos(alg_dico algoritmo, sit_dico situacion, time_dico *tiempos, int *tamV);
void lecturaTiempos(alg_dico *algoritmo);
void cargarTiemposEstaticos(alg_dico algoritmo[]);
void testBuscarCotas(alg_dico *algoritmos);
void testTiempos(alg_dico * algoritmo);


