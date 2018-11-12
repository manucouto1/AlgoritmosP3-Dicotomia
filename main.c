#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define MAX_N 38197

// Constantes Algoritmo Dicotomia
#define NUM_SITUATIONS 3
#define NUM_FUNCT 5
#define NUM_ALGORITHEMS 2

// Algoritmo Ordenacion
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
	time_dico tiempos;
	int tamV;
	void (*func)(int v[], int nargs);
	char sit_name[256];
} sit_dico;

typedef struct {
	sit_dico situation[NUM_SITUATIONS];
	int ini;
	int fin;
	int mult;
	int nTemp;
	void (*func)(int v[], int nargs, int punto);
	char alg_name[256];
} alg_dico;

double execute(funcion op , int n, double exp, int derivada);
/* FIN REGISTROS DICOTOMIA  */

int dispersionA(char *clave, int tamTabla) {
	int i, valor = clave[0], n = MIN(8, strlen(clave));
	for (i = 1; i < n; i++)
		valor += clave[i];

	return valor % tamTabla;
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
		if ((c = fgetc(archivo)) != '\t') {
			printf("Error al leer el tabulador\n");
			return(EXIT_FAILURE);
		}
		for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
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

	while ((diccionario[posActual].ocupada)&&(diccionario[posActual].clave != clave)){
		*colisiones = *colisiones + 1;
		posActual = (x + resol_colision(x,*colisiones)) % MAX_N;
	}

	return posActual;
}

int insertar_cerrada(char *clave, char *sinonimos,
                     tabla_cerrada *diccionario, int tam,
                     int (*dispersion)(char *, int),
                     unsigned int (*resol_colision)(int pos_ini, int num_intento));

void mostrar_cerrada(tabla_cerrada diccionario, int tam){
	int i;

	printf("{");
	for(i = 0; i<tam; i++){
		printf("%d- %s\n",i,diccionario->clave);
	}
	printf("}");

}

/* obtiene la hora actual en microsegundos */
double microsegundos(){
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0)
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}/* obtiene la hora actual en microsegundos */


/*
 * DONE Leer los tiempos de un algoritmo en una situacion concreta
 */
void leerTiempos(alg_dico algoritmo, sit_dico situacion, time_dico *tiempos, int *tamV){
	double ta, tb, t, ti;
	int k, n, i;
	int *v;

	void (*ini)(int [], int) = situacion.func;
	void (*ord)(int [], int) = algoritmo.func;

	//Crear objeto algoritmo
	int inicio = algoritmo.ini;
	int fin = algoritmo.fin;
	int mult = algoritmo.mult;

	i = 0;

	for (n = inicio; n <= fin; n = n * mult) {
		v = malloc(sizeof(int) * n);
		ini(v, n);
		ta = microsegundos();
		ord(v, n);
		tb = microsegundos();
		t = tb - ta;
		tamV[i]=n;

		if (t < 500) {
			ta = microsegundos();
			for (k = 0; k < 1000; k++) {
				ini(v, n);
				ord(v, n);
			}
			tb = microsegundos();
			t = tb - ta;

			ta = microsegundos();
			for (k = 0; k < 1000; k++) {
				ini(v, n);
			}
			tb = microsegundos();

			ti = tb - ta;
			t = (t - ti) / k;

			// DONE > guardar tiempo con *
			tiempos[i] = (time_dico){1,t};
		} else {
			// DONE > guardar tiempo
			tiempos[i] = (time_dico){0,t};
		}
		i++;
		free(v);
	}
}

/* INICIALIZACIÖN DEL VECTOR */

void inicializar_semilla() {
	srand(time(NULL));
}

void aleatorio(int v [], int n) {/* se generan números pseudoaleatorio entre -n y +n */
	int i, m=2*n+1;
	for (i=0; i < n; i++)
		v[i] = (rand() % m) - n;
}

void ascendente(int v [], int n) {
	int i;
	for (i=0; i < n; i++)
		v[i] = i;
}


void descendente (int v[], int n){
	int i;

	for (i = 0; i < n; i++) {
		v[i] = n - i - 1;
	}
}

/*  */



/*Algoritmos de ordenación*/
void interCambioCotas(cota_t *cota1, cota_t *cota2){
	cota1->exp = cota2->exp;
	cota1->cota.isComplex = cota2->cota.isComplex;
	cota1->cota.index = cota2->cota.index;
	strcpy(cota1->cota.name, cota2->cota.name);
}

void ord_ins (cota_t v[], int n, int punto) {
	int i, j;
	cota_t cotaAux;
	double valorCotaAux;

	for (i=1; i<n; i++) {
		interCambioCotas(&cotaAux, &v[i]);
		valorCotaAux = execute(v[i].cota,punto,v[i].exp,1);
		j = i-1;
		while (j>=0 && execute(v[j].cota,punto,v[j].exp,1)>valorCotaAux) {
			interCambioCotas(&v[j+1],&v[j]);
			j--;
		}
		interCambioCotas(&v[j+1],&cotaAux);
	}
}

void intercambiar (cota_t* i, cota_t* j) {
	cota_t aux;
	aux = *i;
	*i = *j;
	*j = aux;
}

void Mediana3(cota_t *v, int i, int j, int n) {
	int k;
	k = (i + j)/2;

	double valK = execute(v[k].cota , n, v[k].exp, 1);
	double valJ = execute(v[j].cota , n, v[j].exp, 1);
	double valI = execute(v[i].cota , n, v[i].exp, 1);

	if (valK > valJ) {
		intercambiar(&v[k], &v[j]);
	}
	if (valK > valI) {
		intercambiar(&v[k],&v[i]);
	}
	if (valI > valJ) {
		intercambiar(&v[i], &v[j]);
	}
}

void OrdenarAux(cota_t *v, int izq, int der, int n) {
	double pivote;
	int i, j;

	if ((izq + UMBRAL) <= der) {
		Mediana3(v, izq, der, n);

		pivote = execute(v[izq].cota,n,v[izq].exp,1);
		i = izq;
		j = der;

		while (j>i) {
			i++;
			while (execute(v[i].cota,n,v[i].exp,1)< pivote) {
				i++;
			}
			j--;
			while (execute(v[j].cota,n,v[j].exp,1) > pivote) {
				j--;
			}
			intercambiar(&v[i], &v[j]);
		}

		intercambiar(&v[i], &v[j]);
		intercambiar(&v[izq], &v[j]);
		OrdenarAux(v, izq, j-1, n);
		OrdenarAux(v, j+1, der, n);
	}
}

void ord_rapida(cota_t *v, int n, int punto) {
	OrdenarAux(v, 0, n-1, punto);
	if (UMBRAL > 1){
		ord_ins(v, n, punto);
	}
}

/* */

/*
 * DONE - funciones que se usaran para el calculo de las cotas
 */
void initFuncs(funcion *funcs){

	funcion LOG = {"log(n)",0,0};
	funcion N = {"n",0,1};
	funcion NxLogN = {"n*log(n)",0,2};
	funcion Nexp_x = {"n^$",1,3};
	funcion Nexp_x_LOG = {"n^$*log(n)",1,4};

	funcs[0] = LOG;
	funcs[1] = N;
	funcs[2] = NxLogN;
	funcs[3] = Nexp_x;
	funcs[4] = Nexp_x_LOG;

}

double execute(funcion op , int n, double exp, int derivada){

	if (!derivada) {

		switch (op.index) {
			case 0:
				return log(n);
			case 1:
				return n;
			case 2:
				return n*log(n);
			case 3:
				return pow(n,exp);
			case 4:
				return pow(n,exp)*log(n);
			default:
				return -1;
		}

	} else {

		switch (op.index) {
			case 0:
				return 1.0/n;
			case 1:
				return 1;
			case 2:
				return log(n)+(1);
			case 3:
				return exp*pow(n,exp-1);
			case 4:
				return ((n*exp)*log(n))+(pow(n,exp)*1/n);
			default:
				return -1;
		}

	}

}

sit_dico initStudyCase(char name[], void (*ini)(int [], int)){
	sit_dico caso;
	strcmp(caso.sit_name, name);
	caso.func = ini;
	return caso;
}

alg_dico initAlgorithem(char name[], void (*func)(int [], int, int), sit_dico sitDico[], int ini, int mult,
		int fin, int nTemp){

	int i;
	alg_dico algoritmo;

	strcpy(algoritmo.alg_name, name);
	algoritmo.func = func;
	algoritmo.ini = ini;
	algoritmo.mult = mult;
	algoritmo.fin = fin;
	algoritmo.nTemp = nTemp;

	for(i = 0; i<NUM_SITUATIONS; i++)
		algoritmo.situation[i] = sitDico[i];

	return algoritmo;
}

/*
 * TODO - inicializa las cotas, para ello tiene que crearlas usando las funciones y habrá que ordenarlas
 */
void initCotas(funcion funcs[], cota_t *cotas, int *nCotas) {
	int i;
	double j;
	char *token1;
	char *token2;
	char aux[256];

	*nCotas = 0;

	for(i = 0; i<NUM_FUNCT; i++){

		if(funcs[i].isComplex){
			j = 1.1;
			while(j < 3.0){
				cotas[*nCotas].cota = funcs[i];
				cotas[*nCotas].exp = j;
				token1 = strtok(cotas[*nCotas].cota.name,"$");
				token2 = strtok(NULL,"$");
				if(token2 != NULL) {
					sprintf(aux, "%s%2.1f%s", token1, cotas[*nCotas].exp, token2);
					strcpy(cotas[*nCotas].cota.name, aux);
				} else {
					sprintf(aux, "%s%2.1f", token1, cotas[*nCotas].exp);
					strcpy(cotas[*nCotas].cota.name, aux);
				}
				j = j + 0.1;
				(*nCotas)++;
			}
		} else {
			cotas[*nCotas].cota = funcs[i];
			cotas[*nCotas].exp = 0;
			(*nCotas)++;
		}
	}

}

/*
 * TODO - las cotas tienen que estar ordenadas,
 * al ser crecientes y al no tener puntos de inflexión debería llegar con ver la derivada en el punto inicial
 * si una función tiene mas pendiente cerca del punto inicial tendría que estar siempre por encima
 */
void sortCotas(cota_t *cotas, int nCotas, int punto) {
	ord_ins(cotas,nCotas,punto);
}

void printCotas(cota_t *cotas, int nCotas){
	int i = 0;

	for(i = 0; i < nCotas; i++){
		printf("Cota %d función %s\n",i,cotas[i].cota.name);
	}
}

void printDerivInPoint(cota_t *cotas, int nCotas, int point){

	int i;

	for(i=0; i<nCotas; i++){
		printf(" %f,", execute(cotas[i].cota,point,cotas[i].exp,1));
	}
	printf("\n");
}

/*
 * DONE - inicialización del los algoritmos de los que realizaremos el estudio
 */
alg_dico initAlgorithems(alg_dico *algoritmos){
	// Esta parte la cambiariamos para cada problema a estudiar,
	// por ejemplo si no hace falta ordenar no habria estas funciones
	sit_dico situations[NUM_SITUATIONS] = {
			initStudyCase("aleatorio", aleatorio),
			initStudyCase("ascendente",ascendente),
			initStudyCase("descendente", descendente)
	};

	alg_dico aux [NUM_ALGORITHEMS] = {
			initAlgorithem("inserción",ord_ins , situations, 500, 2, 32000, 7),
			initAlgorithem("rápida", ord_rapida, situations, 1000, (int) pow(10,8), 10, 6)
	};

	memcpy(algoritmos, aux, NUM_ALGORITHEMS * sizeof(alg_dico));
}

int main() {

	int nCotas;

	funcion funcs[NUM_FUNCT];
	alg_dico algoritmos[NUM_ALGORITHEMS];
	cota_t *cotasEstudio = malloc(sizeof(cota_t)*100);

	// Inicializamos el muestrario de funciones y los algoritmos de estudio
	initFuncs(funcs);
	initAlgorithems(algoritmos);

	// Generamos las Posibles cotas
	initCotas(funcs, cotasEstudio, &nCotas);
	printCotas(cotasEstudio, nCotas);

	// Ordenamos las Posibles Cotas basándonos en la pendiente de las funciones en un punto
	// representativo del intervalo en el que se realiza el estudio
	// printDerivInPoint(cotasEstudio, nCotas, 20000);
	sortCotas(cotasEstudio, nCotas, 20000);

	printCotas(cotasEstudio, nCotas);

	//medirTiempos();
	//acorarComplejidad();

	//mostrarResultados();

	//tabla_cerrada d = malloc (MAX_N * sizeof(entrada));


	free(cotasEstudio);
	return 0;
}