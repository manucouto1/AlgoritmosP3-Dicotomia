#ifndef HEADER_D_HASH
#define HEADER_D_HASH
#include "../d_hash.h"
#endif


void inicializar_cerrada(tabla_cerrada diccionario, int tam){
	int i;

	for(i = 0; i < tam; i++){
		diccionario[i].ocupada = 0;
		strcpy(diccionario[i].clave ,"");
		strcpy(diccionario[i].sinonimos ,"");
	}

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
	if ((archivo = fopen(DATA_FILE_PATH, "r")) == NULL) {
		printf("Error al abrir ’%s’\n",DATA_FILE_PATH);
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

int insertarDatos(item datos[], tabla_cerrada *diccionario, typeDispersion dispersion, typeResColision resolucion) {
	int colisiones = 0, i, insertar;

	printf("Insertando %d elementos... ", NUM_ENTRADAS);

	for (i = 0; i < NUM_ENTRADAS; i++) {
		if ((insertar = insertar_cerrada(datos[i].clave, datos[i].sinonimos, diccionario, MAX_N, dispersion, resolucion)) >=0)
			colisiones += insertar;
		else {
			printf("No se han podido insertar los datos\n");
			return 0;
		}
	}

	printf("Numero total de colisiones: %d\n", colisiones);
	return 1;
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

	inicializar_cerrada(dicc, tam);

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

void testResDispersion(){
	printf("***TABLA CERRADA LINEAL\n");
	test(11, resol_colision_lineal);
	printf("***TABLA CERRADA CUADRATICA\n");
	test(11, resol_colision_cuadratica);
	printf("***TABLA CERRADA DOBLE\n");
	test(11, resol_colision_exploracion_doble);
}

alg_dico_hash initAlgorithems_h(alg_dico_hash algoritmos[]){

	int i = 0;

	tabla_cerrada diccionario1 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario2 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario3 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario4 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario5 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario6 = malloc(sizeof(entrada)*MAX_N);

	item *datos = malloc(sizeof(item)*NUM_ENTRADAS);

	inicializar_cerrada(diccionario1, MAX_N);
	inicializar_cerrada(diccionario2, MAX_N);
	inicializar_cerrada(diccionario3, MAX_N);
	inicializar_cerrada(diccionario4, MAX_N);
	inicializar_cerrada(diccionario5, MAX_N);
	inicializar_cerrada(diccionario6, MAX_N);

	leer_sinonimos(datos);

	printf(" - Inicializando Algoritmos \n");
	printf(" ************************************ \n");

	sit_dico_hash situations1[NUM_SITUATIONS] = {
			initStudyCase_h("RESOLUCION LINEAL", resol_colision_lineal, diccionario1),
			initStudyCase_h("RESOLUCION CUADRÁTICA",resol_colision_cuadratica, diccionario2),
			initStudyCase_h("RESOLUCION DOBLE", resol_colision_exploracion_doble, diccionario3)
	};

	sit_dico_hash situations2[NUM_SITUATIONS] = {
			initStudyCase_h("RESOLUCION LINEAL", resol_colision_lineal, diccionario4),
			initStudyCase_h("RESOLUCION CUADRÁTICA",resol_colision_cuadratica, diccionario5),
			initStudyCase_h("RESOLUCION DOBLE", resol_colision_exploracion_doble, diccionario6)
	};

	// Usar un Contenedor de aplicación para inyeccion de dependencias
	alg_dico_hash aux [NUM_ALGORITHEMS] = {
			initAlgorithem_h("DISPERSION A", datos,
					dispersionA,
					buscar_cerrada,
					insertarDatos,
					situations1, 125, 16000, 2, 8),
			initAlgorithem_h("DISPERSION B", datos,
					dispersionB,
					buscar_cerrada,
					insertarDatos,
					situations2, 125, 16000, 2, 8)
	};

	memcpy(algoritmos, aux, NUM_ALGORITHEMS * sizeof(alg_dico_hash));
	printf("\n");
}

alg_dico_hash initAlgorithems_h_manual(alg_dico_hash algoritmos[]){

	int i = 0;

	tabla_cerrada diccionario1 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario2 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario3 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario4 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario5 = malloc(sizeof(entrada)*MAX_N);
	tabla_cerrada diccionario6 = malloc(sizeof(entrada)*MAX_N);

	item *datos = malloc(sizeof(item)*NUM_ENTRADAS);

	inicializar_cerrada(diccionario1, MAX_N);
	inicializar_cerrada(diccionario2, MAX_N);
	inicializar_cerrada(diccionario3, MAX_N);
	inicializar_cerrada(diccionario4, MAX_N);
	inicializar_cerrada(diccionario5, MAX_N);
	inicializar_cerrada(diccionario6, MAX_N);

	leer_sinonimos(datos);

	printf(" - Inicializando Algoritmos \n");
	printf(" ************************************ \n");

	sit_dico_hash sitResLinealA = initStudyCase_h("RESOLUCION LINEAL", resol_colision_lineal, diccionario1);
	sit_dico_hash sitResCuadA = initStudyCase_h("RESOLUCION CUADRÁTICA",resol_colision_cuadratica, diccionario2);
	sit_dico_hash sitResDobleA = initStudyCase_h("RESOLUCION DOBLE", resol_colision_exploracion_doble, diccionario3);

	putCotasManually(&sitResLinealA.sit,(cota_t){N,0},(cota_t){Nexp_x,0.1},(cota_t){NxLogN,0});
	putCotasManually(&sitResCuadA.sit,(cota_t){N,0},(cota_t){Nexp_x,0.1},(cota_t){NxLogN,0});
	putCotasManually(&sitResDobleA.sit,(cota_t){N,0},(cota_t){Nexp_x,0.1},(cota_t){NxLogN,0});

	sit_dico_hash situations1[NUM_SITUATIONS] = { sitResLinealA, sitResCuadA, sitResDobleA	};

	sit_dico_hash sitResLinealB = initStudyCase_h("RESOLUCION LINEAL", resol_colision_lineal, diccionario4);
	sit_dico_hash sitResCuadB = initStudyCase_h("RESOLUCION CUADRÁTICA",resol_colision_cuadratica, diccionario5);
	sit_dico_hash sitResDobleB = initStudyCase_h("RESOLUCION DOBLE", resol_colision_exploracion_doble, diccionario6);

	putCotasManually(&sitResLinealB.sit,(cota_t){N,0},(cota_t){Nexp_x,0.1},(cota_t){NxLogN,0});
	putCotasManually(&sitResCuadB.sit,(cota_t){N,0},(cota_t){Nexp_x,0.1},(cota_t){NxLogN,0});
	putCotasManually(&sitResDobleB.sit,(cota_t){N,0},(cota_t){Nexp_x,0.1},(cota_t){NxLogN,0});

	sit_dico_hash situations2[NUM_SITUATIONS] = {sitResLinealB, sitResCuadB, sitResDobleB};

	// Usar un Contenedor de aplicación para inyeccion de dependencias
	alg_dico_hash aux [NUM_ALGORITHEMS] = {
			initAlgorithem_h("DISPERSION A", datos,
			                 dispersionA,
			                 buscar_cerrada,
			                 insertarDatos,
			                 situations1, 125, 16000, 2, 8),
			initAlgorithem_h("DISPERSION B", datos,
			                 dispersionB,
			                 buscar_cerrada,
			                 insertarDatos,
			                 situations2, 125, 16000, 2, 8)
	};

	memcpy(algoritmos, aux, NUM_ALGORITHEMS * sizeof(alg_dico_hash));
	printf("\n");
}