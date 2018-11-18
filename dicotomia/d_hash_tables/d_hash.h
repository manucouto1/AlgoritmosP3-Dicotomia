#include "../dicotomia.h"
/* Registros CONCRETOS TABLAS DE DISPERSION */
typedef int (*typeDispersion)(char *, int);
typedef unsigned int (*typeResColision)(int , int);

typedef struct {
	sit_dico sit;
	typeResColision func;
} sit_dico_hash;

typedef struct {
	alg_dico alg;
	sit_dico_hash situation[NUM_SITUATIONS];
	typeDispersion func;
} alg_dico_hash;

/* CONCRETE PROBLEM HASH TABLES API */
alg_dico_hash initAlgorithems(alg_dico_hash *algoritmos);
sit_dico_hash initStudyCase(char *name, typeResColision res_colision);
alg_dico_hash initAlgorithem(char *name, typeDispersion func, sit_dico_hash sitDico[], int nTemp);

void printAlgorithemAndSituation_h(alg_dico_hash *algoritmos);
void buscarCotas_h(alg_dico_hash algoritmos[], cota_t cotas[], int numCotas);
void mostrarCotas_h(alg_dico_hash algoritmo[]);
