#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

// Constantes Algoritmo Dicotomia
#define UMBRAL_DICO 10

#define NUM_SITUATIONS 3
#define NUM_FUNCT 4
#define NUM_ALGORITHEMS 2


/* Registros GENERICO DICOTOMIA */
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
	char sit_name[256];
}sit_dico;

typedef struct {
	int ini;
	int fin;
	int mult;
	int nTemp;
	char alg_name[256];
}alg_dico;

/*  COTAS AND FUNCTIONS DICOTOMIA API */
double execute(funcion op , int n, double exp, int derivada);

void initFuncs(funcion *funcs);
void printFuncs(funcion *funcs);
void initCotas(funcion funcs[], cota_t *cotas, int *nCotas);
void sortCotas(cota_t *cotas, int *nCotas, int puntoInicial, int puntoFinal);
void printCotas(cota_t *cotas, int nCotas);

void acotarComplejidad(sit_dico *sit, cota_t cotas[], int numCotas, int numValoresT);

double microsegundos();






