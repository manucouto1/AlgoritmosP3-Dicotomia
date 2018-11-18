#include <string.h>


#include "dicotomia/d_vector/persist_to_txt/readWriteData_v.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define MAX_N 38197

#define UMBRAL 10


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

void inicializar (tabla_cerrada diccionario, int tam){
	int i;

	for(i = 0; i < tam; i++){
		diccionario[i].ocupada = 0;
		strcpy(diccionario[i].clave ,"");
		strcpy(diccionario[i].sinonimos ,"");
	}

}

void freeMem( tabla_cerrada *diccionario, int tam){
	int i;
	for (i=0; i<tam; i++)
			free(diccionario[i]);
}

int dispersionA(char *clave, int tamTabla) {
	int i, valor = clave[0], n = MIN(8, strlen(clave));
	for (i = 1; i < n; i++)
		valor += clave[i];

	return valor % tamTabla;
}

int dispersionB(char *clave, int tamTabla) {
	int i, n = MIN(8, strlen(clave));
	unsigned int valor = clave[0];
	for (i = 1; i < n; i++)
		valor = (valor<<5) + clave[i]; /* el desplazamiento de 5 bits equivale a */
	return valor % tamTabla; /* multipicar por 32 */
}

unsigned int resol_colision_lineal (int pos_ini, int num_intento){
	return (unsigned int) num_intento;
}

unsigned int resol_colision_cuadratica (int pos_ini, int num_intento){
	return (unsigned int) pow(num_intento,2);
}

unsigned int resol_colision_exploracion_doble (int pos_ini, int num_intento){
	int sol;

	int divid = (5 - pos_ini);

	if(divid > 0)
		sol = divid % 5;
	else
		sol =  (5 + divid%5);

	return (unsigned int) sol*num_intento;
}

int ndispersion(char *clave, int tamTabla) {
	if (strcmp(clave, "ANA")==0) return 7;
	if (strcmp(clave, "JOSE")==0) return 7;
	if (strcmp(clave, "OLGA")==0) return 7;
	return 6;
}

int leer_sinonimos(item datos[]) {
	char c;
	int i, j;
	FILE *archivo;
	if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
		printf("Error al abrir ’sinonimos.txt’\n");
		return(EXIT_FAILURE);
	}
	for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
		if ((c = (char)fgetc(archivo)) != '\t') {
			printf("Error al leer el tabulador\n");
			return(EXIT_FAILURE);
		}
		for (j = 0; (c = (char)fgetc(archivo)) != '\n'; j++) {
			if (j < LONGITUD_SINONIMOS - 1)
				datos[i].sinonimos[j] = c;
		}
		datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
	}
	if (fclose(archivo) != 0) {
		printf("Error al cerrar el fichero\n");
		return(EXIT_FAILURE);
	}
	return(i);
}

pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam,
                   int *colisiones, int (*dispersion)(char *, int),
                   unsigned int (*resol_colision)(int pos_ini, int num_intento)){

	int x = dispersion(clave, tam);
	int posActual = x;

	 if((diccionario[posActual].ocupada)&&(strcmp(diccionario[posActual].clave,clave)!=0))
		do{
			*colisiones = *colisiones + 1;
			posActual = (x + resol_colision(x,*colisiones))%tam;

		}while((diccionario[posActual].ocupada) && (strcmp(diccionario[posActual].clave,clave)!=0) && posActual!=x);

	return posActual;
}

int insertar_cerrada(char *clave, char *sinonimos,
                     tabla_cerrada *diccionario, int tam,
                     int (*dispersion)(char *, int),
                     unsigned int (*resol_colision)(int pos_ini, int num_intento)){
	int colisiones = 0;
	pos pos = buscar_cerrada(clave, *diccionario, tam, &colisiones, dispersion, resol_colision);

	if(!(*diccionario)[pos].ocupada){
		(*diccionario)[pos].ocupada = 1;
		strcpy((*diccionario)[pos].clave,clave);
		strcpy((*diccionario)[pos].sinonimos,sinonimos);
		return colisiones;
	}
	return -1;
}

void mostrar_cerrada(tabla_cerrada diccionario, int tam){
	int i;

	printf("{\n");
	for(i = 0; i<tam; i++){
		printf("%d- %s\n",i,diccionario[i].clave);
	}
	printf("}\n");

}


int test( int tam, unsigned int (*resol_colision)(int pos_ini, int num_intento)) {
	tabla_cerrada dicc = malloc(sizeof(entrada)*tam);
	item data[tam];

	int pos = 0;
	char *claves[7] = {"ANA","LUIS","JOSE","OLGA","ROSA","IVAN","CARLOS"};
	int i;
	int colisiones = 0;

	inicializar(dicc, tam);

	for(i = 0; i<6; i++)
		colisiones += insertar_cerrada(claves[i], "",
				&dicc, tam, ndispersion, resol_colision);


	mostrar_cerrada(dicc, tam);
	printf("Numero total de colisiones al insertar los elementos: %d\n",colisiones);

	for(i = 0; i<7; i++){
		colisiones = 0;
		pos = buscar_cerrada(claves[i], dicc, tam, &colisiones, ndispersion, resol_colision);
		if(dicc[pos].ocupada>0)
			printf("AL buscar %s, encuentro: %s, colisiones: %d\n", claves[i], dicc[pos].clave ,colisiones);
		else
			printf("No encuentro: %s, colisiones: %d\n", claves[i],colisiones);
	}

	free(dicc);
}



int main() {

	//COTAS
	int nCotas;
	funcion funcs[NUM_FUNCT];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	/*
	// ALGORITMO que trabaja con VECTORES
	alg_dico_vector algoritmos[NUM_ALGORITHEMS];

	// Inicializamos el muestrario de funciones y los algoritmos de estudio
	initFuncs(funcs);
	printFuncs(funcs);
	initAlgorithems_v(algoritmos);
	printAlgorithemAndSituation_v(algoritmos);

	// Generamos las Posibles cotas
	initCotas(funcs, cotasEstudio, &nCotas);
	printCotas(cotasEstudio, nCotas);

	// Ordenamos las Posibles Cotas basándonos en la pendiente de las funciones en un punto
	// representativo del intervalo en el que se realiza el estudio
	sortCotas(cotasEstudio, &nCotas, 1000, 100000000);
	printCotas(cotasEstudio, nCotas);

	// OBTENER TIEMPOS Y PERSISTIR A TXT
	//cargarTiemposEstaticos_v(algoritmos);
	//lecturaTiempos_v(algoritmos);
	//cacheTimeData_v(algoritmos);

	// CARGAR TIEMPOS DESDE TXT
	loadCachedTime_v(algoritmos);

	buscarCotas_v(algoritmos, cotasEstudio, nCotas);
	*/

	//



	//tabla_cerrada d = malloc (MAX_N * sizeof(entrada));




	//printf("***TABLA CERRADA LINEAL\n");
	//test(11, resol_colision_lineal);
	//printf("***TABLA CERRADA CUADRATICA\n");
	//test(11, resol_colision_cuadratica);
	//printf("***TABLA CERRADA DOBLE\n");
	//test(11, resol_colision_exploracion_doble);
	free(cotasEstudio);
}