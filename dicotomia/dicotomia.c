#include <math.h>
#ifndef HEADER_DICOTOMIA
#define HEADER_DICOTOMIA
#include "dicotomia.h"
#endif


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


cota_t generateName(cota_t cota){
	char *token1;
	char *token2;
	char aux[256];

	if(cota.cota.isComplex) {
		token1 = strtok(cota.cota.name, "$");
		token2 = strtok(NULL, "$");
		if (token2 != NULL) {
			sprintf(aux, "%s%2.1f%s", token1, cota.exp, token2);
			strcpy(cota.cota.name, aux);
		} else {
			sprintf(aux, "%s%2.1f", token1, cota.exp);
			strcpy(cota.cota.name, aux);
		}
	}
	return cota;
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
			j = 0.1;
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
int tendenciaCreciente(double umbral_tolerancia, cota_t cota, int nVector[], time_dico tiempos[], int nArgs, int anomalo_ini){

	int i = 1;
	int tend = 1;
	int anomalo = anomalo_ini;

	long double n;
	long double n_mas_uno;
	long double media = 0;
	long double media_anterior = 0;

	n = (tiempos[0].tiempo / execute(cota.cota, nVector[0],cota.exp,0));
	media = n;

	printf(" - Nombre F(n) = %-13s | f(n) = %-24f | t = %-20f | t/f(n) = %-20.15Lf\n", cota.cota.name,
	       execute(cota.cota, nVector[0],cota.exp,0), tiempos[0].tiempo, n);

	while (i < (nArgs - 1) && tend >= -1) {

		n_mas_uno = (tiempos[i + 1].tiempo / execute(cota.cota, nVector[i + 1],cota.exp,0));
		media = (media + n)/2;
		media = (media_anterior + media)/2;
		printf(" - Nombre F(n) = %-13s | f(n) = %-24f | t = %-20f | t/f(n) = %-20.15Lf | anomalo = %d \n", cota.cota.name,
		     execute(cota.cota, nVector[i],cota.exp,0), tiempos[i].tiempo, n, anomalo);

		if( n_mas_uno < n + media * umbral_tolerancia ) {
			if(anomalo >= 2) {
				tend = -2;
			} else
				anomalo++;
		}

		if(n > n_mas_uno)
			tiempos[i].is_anomalo = 1;
		n = n_mas_uno;
		media_anterior = media;
		i++;
	}

	return tend;;

}

int tendenciaDecreciente(double umbral_tolerancia, cota_t cota, int nVector[], time_dico tiempos[], int nArgs, int anomalo_ini){
	int i = 1;
	int tend = -1;
	int anomalo = anomalo_ini;

	long double n;
	long double n_mas_uno;
	long double media = 0;
	long double media_anterior = 0;

	n = (tiempos[0].tiempo / execute(cota.cota, nVector[0],cota.exp,0));
	media = n;

	printf(" - Nombre F(n) = %-13s | f(n) = %-24f | t = %-20f | t/f(n) = %-20.15Lf\n", cota.cota.name,
	    execute(cota.cota, nVector[0],cota.exp,0), tiempos[0].tiempo, n);

	while (i < (nArgs - 1) && tend >= -1) {

		n_mas_uno = (tiempos[i].tiempo / execute(cota.cota, nVector[i],cota.exp,0));

		media = (media + n)/2;
		media = (media_anterior + media)/2;

		printf(" - Nombre F(n) = %-13s | f(n) = %-24f | t = %-20f | t/f(n) = %-20.15Lf | media = %-20.15Lf | anomalo = %d \n", cota.cota.name,
		       execute(cota.cota, nVector[i],cota.exp,0), tiempos[i].tiempo, media, n_mas_uno, anomalo);

		if( n + media * umbral_tolerancia  < n_mas_uno ) {
			if (anomalo >= 2) {
				tend = -2;
			} else {
				anomalo++;
			}
		}

		if(n > n_mas_uno)
			tiempos[i].is_anomalo = 0;

		n = n_mas_uno;

		media_anterior = media;

		i++;
	}

	return tend;;
}


int tendencia(double umbral_tolerancia, cota_t cota ,int nVector[], time_dico tiempos[] , int nArgs){

	int i = 1;
	int tend = 7; // 7 indica que es la primera iteración
	int anomalo = 0;

	long double n;
	long double media = 0;

	n = (tiempos[0].tiempo / execute(cota.cota, nVector[0],cota.exp,0));
	media = n;

	printf(" - Nombre F(n) = %-13s | f(n) = %-24f | t = %-20f | t/f(n) = %-20.15Lf\n", cota.cota.name,
	       execute(cota.cota, nVector[0],cota.exp,0), tiempos[0].tiempo, n);

	while (i < (nArgs - 1) && tend >= -1) {

		n = (tiempos[i + 1].tiempo / execute(cota.cota, nVector[i + 1],cota.exp,0));

		printf(" - Nombre F(n) = %-13s | f(n) = %-24f | t = %-20f | t/f(n) = %-20.15Lf\n", cota.cota.name,
		       execute(cota.cota, nVector[i],cota.exp,0), tiempos[i].tiempo, n);

		if(n < media + (media * umbral_tolerancia)){
			if(tend == 7 || tend == -1)
				tend = -1;
			else if(anomalo == 1)
				tend = -2;
			else
				anomalo = 1;
		} else if( n + (media * umbral_tolerancia) > media) {
			if(tend == 7 || tend == 1)
				tend = 1;
			else if(anomalo == 1)
				tend = -2;
			else
				anomalo = 1;
		} else if(fabs((double)(n - media)) < media * umbral_tolerancia){
			if(tend == 7 || tend == 0) {
				tend = 0;
				anomalo = 0;
			} else if(anomalo == 1)
				tend = -2;

		} else if(anomalo == 1) {
			tend = -2;
		} else
			anomalo = 1;

		media = (media + n)/(i+1);
		i++;
	}

	return tend;;
}

double calcularMedia(const double valores[], int nTemp) {
	int i;
	double total = 0;

	for (i = 0; i < nTemp; i++) {
		total += valores[i];
	}

	return total/nTemp;
}

void auxAcotarSobre(double umbral,cota_t *cSobre, cota_t *cAjust, cota_t cotas[], int *pibSobre, int *pibSub, int valN[], time_dico tiempos[], int numT){

	int pibAjust;

	printf("_______________________________________________________________________________________________________________________________\n");
	printf("Acotando SOBRE \n");
	if (tendenciaCreciente(umbral, cotas[*pibSobre], valN, tiempos, numT, 0) == 1 || (*pibSobre < *pibSub)){
		*pibSobre = (*pibSobre)++;
		auxAcotarSobre(umbral, cSobre, cAjust, cotas, pibSobre, pibSub, valN, tiempos, numT);
	} else {
		pibAjust = *pibSobre;
		(*pibSobre)--;
		printf(" ^ >>>>>> AJUSTADA <<<<<   %d\n", *pibSobre);
		printf("SOBRE > %s\n",cotas[*pibSobre].cota.name);
		printf("AJUS > %s\n",cotas[pibAjust].cota.name);
		interCambioCotas(cAjust, &cotas[pibAjust]);
		interCambioCotas(cSobre, &cotas[*pibSobre]);
		}
}

void auxAcotarSub(double umbral,cota_t *cSub, cota_t *cAjust, cota_t cotas[], int *pibSub, int pibSobre, int valN[], time_dico tiempos[], int numT){

	int pibAjust;

	printf("_______________________________________________________________________________________________________________________________\n");
	printf("Acotando SUB \n");
	if (tendenciaDecreciente(umbral, cotas[*pibSub], valN, tiempos, numT,0) == -1 || (pibSobre < *pibSub)){
		*pibSub = (*pibSub)--;
		auxAcotarSub(umbral, cSub, cAjust, cotas, pibSub, pibSobre, valN, tiempos, numT);
	} else {
		pibAjust = *pibSub;
		pibSub++;
		printf(" ^ >>>>>> AJUSTADA <<<<<   %d\n", *pibSub);
		printf("SUB > %s\n",cotas[*pibSub].cota.name);
		printf("AJUS > %s\n",cotas[pibAjust].cota.name);
		interCambioCotas(cAjust, &cotas[pibAjust]);
		interCambioCotas(cSub, &cotas[*pibSub]);
	}
}


void acotarComplejidad(sit_dico *sit, cota_t cotas[], int numCotas, int numValoresT){
	int succesSobre = 0;
	int succesSub = 0;

	double err = 1.0;
	double umbral = 0.01;
	int pibSobre = 0;
	int pibSub = numCotas - 1;
	int pibAjusSobre = 0;
	int pibAjusSub = 0;

	int automata;
	while (pibSobre < pibSub && (!succesSobre || !succesSub)) {
		if(!succesSobre) {
			printf("_______________________________________________________________________________________________________________________________\n");
			printf("Acotando SOBRE \n");

			if (tendenciaCreciente(umbral, cotas[pibSobre], sit->valN, sit->tiempos, numValoresT,0) == 1){
				pibSobre++;
			} else {
				succesSobre = 1;
				pibAjusSobre = pibSobre;
				pibSobre--;
				printf(" ^ >>>>>> AJUSTADA <<<<<   %d\n", pibSobre);
				printf("SOBRE > %s\n",cotas[pibSobre].cota.name);
				printf("AJUS > %s\n",cotas[pibAjusSobre].cota.name);
				interCambioCotas(&sit->ajus, &cotas[pibAjusSobre]);
				interCambioCotas(&sit->sobre, &cotas[pibSobre]);
			}
		}
		printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n");
		if(!succesSub) {
			printf("Acotando SUB \n");
			automata = tendenciaDecreciente(umbral, cotas[pibSub], sit->valN, sit->tiempos, numValoresT,0);
			printf("AUTOMATA = %d \n", automata);
			if (automata == -1){
				pibSub--;
			} else {
				succesSub = 1;
				pibAjusSub = pibSub;
                pibSub++;
				printf(" ^ >>>>>> AJUSTADA <<<<<   %d\n", pibAjusSub);
				printf("SUB > %s\n",cotas[pibSub].cota.name);
				printf("AJUS > %s\n",cotas[pibAjusSub].cota.name);
				interCambioCotas(&sit->ajus, &cotas[pibAjusSub]);
				interCambioCotas(&sit->sub, &cotas[pibSub]);
			}
		}
	}

	if(!succesSobre && !succesSub){
		if(pibSobre > 0){
			pibAjusSobre = pibSobre - 1;
			pibSub = pibSobre;
			pibSobre = pibAjusSobre - 1;
			interCambioCotas(&sit->sobre, &cotas[pibSobre]);
			interCambioCotas(&sit->ajus, &cotas[pibAjusSobre]);
			interCambioCotas(&sit->sub, &cotas[pibSub]);
		} else {
			pibAjusSub = pibSub + 1;
			pibSobre = pibSub;
			pibSobre = pibAjusSub - 1;
			interCambioCotas(&sit->sobre, &cotas[pibSobre]);
			interCambioCotas(&sit->ajus, &cotas[pibAjusSub]);
			interCambioCotas(&sit->sub, &cotas[pibSub]);
		}
	} else {

		if(succesSobre && succesSub){
			interCambioCotas(&sit->ajus, &cotas[(pibAjusSub+pibAjusSobre)/2]);
		} else {
			if (!succesSobre) {
				printf("_______________________________________________________________________________________________________________________________\n");
				printf("Acotando SOBRE \n");
				pibSobre = pibSobre - 2;
				while (pibSobre < pibSub && !succesSobre){
					if(tendenciaCreciente(umbral, cotas[pibSobre], sit->valN, sit->tiempos, numValoresT,2) == 1){
						pibSobre++;
					} else {
						succesSobre = 1;
						pibAjusSobre = pibSobre;
						pibSobre--;
						printf(" ^ >>>>>> AJUSTADA <<<<<   %d\n", pibSobre);
						printf("SOBRE > %s\n",cotas[pibSobre].cota.name);
						printf("AJUS > %s\n",cotas[pibAjusSobre].cota.name);
						interCambioCotas(&sit->ajus, &cotas[pibAjusSobre]);
						interCambioCotas(&sit->sobre, &cotas[pibSobre]);
					}
					printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n");
				}
			}

			if (!succesSub) {
				printf("_______________________________________________________________________________________________________________________________\n");
				printf("Acotando SUB \n");
				pibSub = pibSub + 2;
				while (pibSobre < pibSub && !succesSub){
					if(tendenciaDecreciente(umbral, cotas[pibSub], sit->valN, sit->tiempos, numValoresT,2) == -1){
						pibSub--;
					} else {
						succesSub = 1;
						pibAjusSub = pibSub;
						pibSub++;
						printf(" ^ >>>>>> AJUSTADA <<<<<   %d\n", pibAjusSub);
						printf("SUB > %s\n",cotas[pibSub].cota.name);
						printf("AJUS > %s\n",cotas[pibAjusSub].cota.name);
						interCambioCotas(&sit->ajus, &cotas[pibAjusSub]);
						interCambioCotas(&sit->sub, &cotas[pibSub]);
					}
					printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n");
				}
			}
		}

	}


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

void putCotasManually(sit_dico *sit, cota_t sobre, cota_t ajus, cota_t sub){
	sit->sobre = sobre;
	sit->ajus = ajus;
	sit->sub = sub;
}