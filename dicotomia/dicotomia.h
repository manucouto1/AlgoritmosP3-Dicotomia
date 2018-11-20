#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>


#define LOG (funcion) {"log(n)",0,0}
#define N (funcion) {"n",0,1}
#define NxLogN (funcion) {"n*log(n)",0,2}
#define Nexp_x (funcion) {"n^$",1,3}
#define Nexp_x_LOG (funcion) {"n^$*log(n)",1,4}

// Constantes Algoritmo Dicotomia
#define UMBRAL_DICO 10

#define NUM_SITUATIONS 3
#define NUM_FUNCT 3
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
	int is_anomalo;
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
void putCotasManually(sit_dico *sit, cota_t sobre, cota_t ajus, cota_t sub);

void acotarComplejidad(sit_dico *sit, cota_t cotas[], int numCotas, int numValoresT);

cota_t generateName(cota_t cota);
double microsegundos();






