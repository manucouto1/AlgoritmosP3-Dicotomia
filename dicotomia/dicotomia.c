#include <math.h>
#include "dicotomia.h"


/*Algoritmos de ordenación*/
void interCambioCotas(cota_t *cota1, cota_t *cota2){
	cota1->exp = cota2->exp;
	cota1->cota.isComplex = cota2->cota.isComplex;
	cota1->cota.index = cota2->cota.index;
	strcpy(cota1->cota.name, cota2->cota.name);
}

void ord_ins_dico (cota_t v[], int n, int puntoIni, int puntoFin) {
	int i, j;
	cota_t cotaAux;
	double valorCotaAux;

	for (i=1; i<n; i++) {
		interCambioCotas(&cotaAux, &v[i]);
		valorCotaAux = execute(v[i].cota,puntoIni,v[i].exp,1)+execute(v[i].cota,puntoFin,v[i].exp,1);
		j = i-1;
		while (j>=0 && (execute(v[j].cota,puntoIni,v[j].exp,1)+execute(v[j].cota,puntoFin,v[j].exp,1))>valorCotaAux) {
			interCambioCotas(&v[j+1],&v[j]);
			j--;
		}
		interCambioCotas(&v[j+1],&cotaAux);
	}
}

// TODO IMPLEMENTAR ORDENACION RAPIDA DE COTAS

void printFuncs(funcion *funcs){
	int i;

	for(i = 0; i < NUM_FUNCT; i++){
		printf("\t - index %d - %s with exponent? %d \n", funcs[i].index, funcs[i].name, funcs[i].isComplex);
	}
	printf("\n");
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

	printf(" - Inicializando Cotas\n");
	printf(" ************************************ \n");

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
	printf("\n");
}

void fixProblematic(cota_t cotas1[], cota_t cotas2[], int *nCotas){
	int i,j;

	for(i=0; i<*nCotas; i++){
		if(strcmp(cotas1[i].cota.name, cotas2[i].cota.name)!=0){

			if(cotas1[i].cota.isComplex){
				interCambioCotas(&cotas1[i],&cotas2[i]);
				for(j=(i+1); j<*nCotas; j++){
					interCambioCotas(&cotas1[j],&cotas1[j+1]);
					interCambioCotas(&cotas2[j],&cotas2[j+1]);
				}
			} else {

				for(j=(i+1); j<*nCotas; j++){
					interCambioCotas(&cotas1[j],&cotas1[j+1]);
					interCambioCotas(&cotas2[j],&cotas2[j+1]);
				}
			}
			(*nCotas)--;
		}
	}
	printf("\n");
}

/*
 * TODO - las cotas tienen que estar ordenadas,
 * al ser crecientes y al no tener puntos de inflexión debería llegar con ver la derivada en el punto inicial
 * si una función tiene mas pendiente cerca del punto inicial tendría que estar siempre por encima
 */
void sortCotas(cota_t cotas[], int *nCotas, int puntoInicial, int puntoFinal) {
	cota_t *cotasAux = malloc(sizeof(cota_t)*100);
	int i;

	for(i=0; i<*nCotas; i++){
		interCambioCotas(&cotasAux[i], &cotas[i]);
	}
	printf(" - Ordenando Cotas \n");
	printf(" ************************************ \n");

	ord_ins_dico(cotas,*nCotas, puntoInicial, puntoFinal);
	//ord_ins_dico(cotasAux, *nCotas, puntoFinal);

	for(i=0; i<*nCotas; i++){
		printf(" %-15s - %10s\n", cotas[i].cota.name,cotasAux[i].cota.name);
	}
	//fixProblematic(cotas, cotasAux, nCotas);
	printf("\n");
	free(cotasAux);
}

void printCotas(cota_t *cotas, int nCotas){
	int i = 0;

	for(i = 0; i < nCotas; i++){
		printf("\t - Cota %d función %s\n",i,cotas[i].cota.name);
	}
	printf("\n");
}

int tendencia(double umbral_tolerancia, cota_t cota ,int nVector[], time_dico tiempos[] , int nArgs){

	int i = 1;
	int tend = 7; // 7 indica que es la primera iteración
	int anomalo = 0;

	long double n;
	long double n_mas_uno;

	// estado Anomalo = 0 -> el resultado anterior fue el esperado
	// estado Anomalo = 1 -> el resultado anterior cambia la tendencia

	// Si n < n_mas_uno -> ( n_mas_uno - n + umbral > 0 ) -> crece
	// Si n > n_mas_uno -> ( n - n_mas_uno + umbral > 0 ) -> decrece
	// Si n == n_mas_uno -> ( n_mas_uno - n < umbral ) -> ajustada

	// El umbral lo usamos para que tenga cierta tolerancia a mediciones extrañas
	// TODO - Hay que definir un comportamiento para los casos en los que no sea extrictamente creciente/decreciente/cte
	// TODO - He pensado en hacer un pequeño autómata como en EC para que sigua

	n = (tiempos[0].tiempo / execute(cota.cota, nVector[0],cota.exp,0));
	printf(" - Nombre F(n) = %-13s | f(n) = %-24f | t = %-20f | t/f(n) = %-20.15Lf\n", cota.cota.name,
	       execute(cota.cota, nVector[0],cota.exp,0), tiempos[0].tiempo, n);

	while (i < (nArgs - 1) && tend >= -1) {

		n_mas_uno = (tiempos[i + 1].tiempo / execute(cota.cota, nVector[i + 1],cota.exp,0));
		printf(" - Nombre F(n) = %-13s | f(n) = %-24f | t = %-20f | t/f(n) = %-20.15Lf\n", cota.cota.name,
		       execute(cota.cota, nVector[i],cota.exp,0), tiempos[i].tiempo, n_mas_uno);

		if (n_mas_uno - n + ((n_mas_uno - n)*umbral_tolerancia) > 0){
			if(!anomalo){
				if(tend == 7 || tend == 1) // tend == 7 es para la primera iteración
					tend = 1; // Crece
				else {
					anomalo = 1;
					tend = 1; // avisamos de que ha habido una dato anómalo
				}
			} else {
				if( tend == 1 ){
					tend = 1; // Crece
				} else {
					tend = -2; // optamos por dar el resultado por erroneo, demasiados datos erroneos
				}
			}

		} else if (n - n_mas_uno + fabs((double)(1000000*n - 1000000*n_mas_uno)/1000000)*umbral_tolerancia > 0){
			if(!anomalo){
				if(tend == 7 || tend == -1) { // tend == 7 es para la primera iteración
					tend = -1; // Decrece
					anomalo = 0;
				} else {
					anomalo = 1;
					tend = -1; // avisamos de que ha habido una dato anómalo
				}
			} else {
				if( tend == -1 ){
					tend = -1;
				} else {
					tend = -2; // optamos por dar el resultado por erroneo, demasiados datos erroneos
				}
			}
		} else if (fabs((double)(1000000*n - 1000000*n_mas_uno)/1000000)*umbral_tolerancia < umbral_tolerancia){
			if(!anomalo){
				if(tend == 7 || tend == 0) { // tend == 7 es para la primera iteración
					anomalo = 0;
					tend = 0;
				} else {
					anomalo = 1;
					tend = 0; // avisamos de que ha habido una dato anómalo
				}
			} else {
				if( tend == 0 ){
					tend = 0;
				} else {
					tend = -2; // optamos por dar el resultado por erroneo, demasiados datos erroneos
				}
			}
		}
		n = n_mas_uno;
		i++;
	}

	return tend;;
}

void acotarComplejidad(sit_dico *sit, cota_t cotas[], int numCotas, int numValoresT){
	int succesSobre = 0;
	int succesSub = 0;
	int succesAjus = 0;

	double err = 1.0;
	double umbral = 0.01;
	int pibSobre = 0;
	int pibSub = numCotas - 1;
	int pibAjus;

	int automata;

	while (pibSobre < pibSub && (!succesSobre || !succesSub) && err > 0.1) {
		if(!succesSobre) {
			printf("_______________________________________________________________________________________________________________________________\n");
			printf("Acotando SOBRE \n");
			automata = tendencia(umbral, cotas[pibSobre], sit->valN, sit->tiempos, numValoresT);
			if (automata == 1)
				pibSobre++;
			else if (automata == 0) {
				succesAjus = 1;
				pibAjus = pibSobre;
				interCambioCotas(&sit->ajus, &cotas[pibAjus]);
				printf(" ^ >>>>>> AJUSTADA <<<<<   %d\n", pibAjus);
				pibSobre--;
			} else {
				pibSobre--;
				printf(" ^ >>>>>> SOBRESTIMADA <<<<<   %d\n", pibSobre);
				interCambioCotas(&sit->sobre, &cotas[pibSobre]);
				succesSobre = 1;
			}
		}
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n");
		if(!succesSub) {
			printf("Acotando SUB \n");
			automata = tendencia(umbral, cotas[pibSub], sit->valN, sit->tiempos, numValoresT);
			if (automata == -1)
				pibSub--;
			else if (automata == 0) {
				succesAjus = 1;
				pibAjus = pibSub;
				interCambioCotas(&sit->ajus, &cotas[pibAjus]);
				printf(" ^ >>>>>> AJUSTADA <<<<<   %d\n", pibAjus);
				pibSub++;
			} else {
                pibSub++;
				printf(" ^ >>>>>> SUBESTIMADA <<<<<   %d\n", pibSub);
				interCambioCotas(&sit->sub, &cotas[pibSub]);
				succesSub = 1;
			}
		}
	}

	if(!succesSobre)
		interCambioCotas(&sit->sobre, &cotas[pibSobre]);
	if(!succesSub)
		interCambioCotas(&sit->sub, &cotas[pibSub]);
	if(!succesAjus)
		interCambioCotas(&sit->ajus, &cotas[(pibSub+pibSobre)/2]);
	printf("RESULTS ");
	printf("___________________________________________________________\n");
	printf("SuccesSub %5d | SuccesSobre %5d\n", succesSub, succesSobre);
	printf("Value Sub %5d | Value Sobre %5d\n", pibSub, pibSobre);
	printf("NOT SUCCESS | \tsub > %5s \tajust > %5s \tsobre > %5s \n",sit->sub.cota.name,sit->ajus.cota.name,sit->sobre.cota.name);

	//printf("%s\t %s\t %s\n", sit->sobre.cota.name, sit->ajus.cota.name, sit->sub.cota.name);
}

/* obtiene la hora actual en microsegundos */
double microsegundos(){
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0)
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}/* obtiene la hora actual en microsegundos */