#include "dicotomia.h"

/*Algoritmos de ordenación*/
void interCambioCotas(cota_t *cota1, cota_t *cota2){
	cota1->exp = cota2->exp;
	cota1->cota.isComplex = cota2->cota.isComplex;
	cota1->cota.index = cota2->cota.index;
	strcpy(cota1->cota.name, cota2->cota.name);
}

void ord_ins_dico (cota_t v[], int n, int punto) {
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

// TODO IMPLEMENTAR ORDENACION RAPIDA DE COTAS
void intercambiar_dico (cota_t* i, cota_t* j) {
	cota_t aux;
	aux = *i;
	*i = *j;
	*j = aux;
}

void Mediana3_dico(cota_t *v, int i, int j, int n) {
	int k;
	k = (i + j)/2;

	double valK = execute(v[k].cota , n, v[k].exp, 1);
	double valJ = execute(v[j].cota , n, v[j].exp, 1);
	double valI = execute(v[i].cota , n, v[i].exp, 1);

	if (valK > valJ) {
		intercambiar_dico(&v[k], &v[j]);
	}
	if (valK > valI) {
		intercambiar_dico(&v[k],&v[i]);
	}
	if (valI > valJ) {
		intercambiar_dico(&v[i], &v[j]);
	}
}

void OrdenarAux_dico(cota_t *v, int izq, int der, int n) {
	double pivote;
	int i, j;

	if ((izq + UMBRAL_DICO) <= der) {
		Mediana3_dico(v, izq, der, n);

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
			intercambiar_dico(&v[i], &v[j]);
		}

		intercambiar_dico(&v[i], &v[j]);
		intercambiar_dico(&v[izq], &v[j]);
		OrdenarAux_dico(v, izq, j-1, n);
		OrdenarAux_dico(v, j+1, der, n);
	}
}

void ord_rapida_dico(cota_t *v, int n, int punto) {
	OrdenarAux_dico(v, 0, n-1, punto);
	if (UMBRAL_DICO > 1){
		ord_ins_dico(v, n, punto);
	}
}
//

sit_dico initStudyCase(char *name, void (*ini)(int [], int)){
	sit_dico caso;
	strcpy(caso.sit_name, name);
	caso.func = ini;
	return caso;
}

alg_dico initAlgorithem(char *name, void (*func)(int [], int), sit_dico *sitDico, int ini, int mult,
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

void printAlgorithemSituation(alg_dico *algoritmos){

	int i,j;

	for(i=0; i<NUM_ALGORITHEMS; i++){
		printf("\t - %s - tamaño vector de %d a %d de %d en %d\n", algoritmos[i].alg_name,algoritmos[i].ini,algoritmos[i].fin, algoritmos[i].mult,algoritmos[i].mult);
		for(j=0; j<NUM_SITUATIONS; j++){
			printf("\t\t * situacion %s \n",algoritmos[i].situation[j].sit_name);
		}
	}
	printf("\n");
}


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

/*
 * TODO - las cotas tienen que estar ordenadas,
 * al ser crecientes y al no tener puntos de inflexión debería llegar con ver la derivada en el punto inicial
 * si una función tiene mas pendiente cerca del punto inicial tendría que estar siempre por encima
 */
void sortCotas(cota_t *cotas, int nCotas, int punto) {
	printf(" - Ordenando Cotas \n");
	printf(" ************************************ \n");
	ord_ins_dico(cotas,nCotas,punto);
	printf("\n");
}

void printCotas(cota_t *cotas, int nCotas){
	int i = 0;

	for(i = 0; i < nCotas; i++){
		printf("\t - Cota %d función %s\n",i,cotas[i].cota.name);
	}

	printf("\n");
}