#ifndef HEADER_DICOTOMIA
#define HEADER_DICOTOMIA
#include "../dicotomia.h"
#endif
/* Registros CONCRETOS TABLAS DE DISPERSION */
typedef int (*typeDispersion)(char *, int);
typedef unsigned int (*typeResColision)(int , int);

#define DATA_FILE_PATH "../static/sinonimos.txt"
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define MAX_N 38197
#define NUM_ENTRADAS 19062

typedef int pos;

typedef struct entrada_ {
	int ocupada;
	char clave [LONGITUD_CLAVE];
	char sinonimos [LONGITUD_SINONIMOS];
} entrada;

typedef entrada *tabla_cerrada;

typedef struct {
	char clave [LONGITUD_CLAVE];
	char sinonimos [LONGITUD_SINONIMOS];
} item;

typedef struct {
	sit_dico sit;
	tabla_cerrada diccionario;
	typeResColision func;
} sit_dico_hash;

/* Metodos del test No me gusta tenerlos aquí, buscar solucion*/
typedef pos (*type_buscar_cerrada)(char *clave, tabla_cerrada diccionario, int tam,
                              int *colisiones, int (*dispersion)(char *, int),
                              unsigned int (*resol_colision)(int pos_ini, int num_intento));

typedef int (*type_insertar_datos)(item datos[], tabla_cerrada *diccionario, typeDispersion dispersion, typeResColision resolucion);

/**/

typedef struct {
	alg_dico alg;
	sit_dico_hash situation[NUM_SITUATIONS];
	typeDispersion func;
	type_buscar_cerrada buscar_cerrada;
	type_insertar_datos insertar_datos;
	item *datos;
} alg_dico_hash;

/* CONCRETE PROBLEM HASH TABLES API */
alg_dico_hash initAlgorithems_h(alg_dico_hash *algoritmos);
sit_dico_hash initStudyCase_h(char *name, typeResColision res_colision, tabla_cerrada diccionario);
alg_dico_hash initAlgorithem_h(char *name, item datos[], typeDispersion func,
		type_buscar_cerrada buscar_cerrada, type_insertar_datos insertar_datos,
                               sit_dico_hash sitDico[], int ini, int fin, int mult, int nTemp);

void printAlgorithemAndSituation_h(alg_dico_hash *algoritmos);
void buscarCotas_h(alg_dico_hash algoritmos[], cota_t cotas[], int numCotas);
void mostrarCotas_h(alg_dico_hash algoritmo[]);

/* Test Lectura Tiempos */
void leerTiempo_h(alg_dico_hash algoritmo, sit_dico_hash situacion, time_dico tiempos[], int valoresN[]);
void lecturaTiempos_h(alg_dico_hash algoritmo[]);
