#ifndef HEADER_D_SORT
#define HEADER_D_SORT
#include "../d_sort.h"
#endif

#define UMBRAL 10
/*Algoritmos de ordenación*/

void ord_ins (int v [], int n) {
	int i, j, x;
	for (i=1; i<n; i++) {
		x = v[i];
		j = i-1;
		while (j>=0 && v[j]>x) {
			v[j+1] = v[j];;
			j--;
		}
		v[j+1] = x;
	}
}
void intercambiar (int* i, int* j) {
	int aux;
	aux = *i;
	*i = *j;
	*j = aux;
}
void Mediana3(int v[], int i, int j) {
	int k;
	k = (i + j)/2;

	if (v[k] > v[j]) {
		intercambiar(&v[k], &v[j]);
	}
	if (v[k] > v[i]) {
		intercambiar(&v[k],&v[i]);
	}
	if (v[i] > v[j]) {
		intercambiar(&v[i], &v[j]);
	}
}
void OrdenarAux(int v[], int izq, int der) {
	int pivote, i, j;

	if ((izq + UMBRAL) <= der) {
		Mediana3(v, izq, der);

		pivote = v[izq];
		i = izq;
		j = der;

		while (j>i) {
			i++;
			while (v[i] < pivote) {
				i++;
			}
			j--;
			while (v[j] > pivote) {
				j--;
			}
			intercambiar(&v[i], &v[j]);
		}

		intercambiar(&v[i], &v[j]);
		intercambiar(&v[izq], &v[j]);
		OrdenarAux(v, izq, j-1);
		OrdenarAux(v, j+1, der);
	}
}
void ord_rapida(int v[], int n) {
	OrdenarAux(v, 0, n-1);
	if (UMBRAL > 1){
		ord_ins(v, n);
	}
}
/*Fin algoritmos de ordenación*/

/*
 * DONE - inicialización del los algoritmos de los que realizaremos el estudio
 */
alg_dico_sort initAlgorithems_s(alg_dico_sort algoritmos[]){
	// Esta parte la cambiariamos para cada problema a estudiar,
	// por ejemplo si no hace falta ordenar no habria estas funciones
	int i = 0;
	inicializar_semilla();
	printf(" - Inicializando Algoritmos \n");
	printf(" ************************************ \n");
	sit_dico_sort situations[NUM_SITUATIONS] = {
			initStudyCase_s("vector aleatorio", aleatorio),
			initStudyCase_s("vector ascendente",ascendente),
			initStudyCase_s("vector descendente", descendente)
	};

	alg_dico_sort aux [NUM_ALGORITHEMS] = {
			initAlgorithem_s("Insercion",ord_ins , situations, 500, 2, 32000, 7),
			initAlgorithem_s("Quicksort", ord_rapida, situations, 1000, 10, (int) pow(10,8), 6)
	};

	memcpy(algoritmos, aux, NUM_ALGORITHEMS * sizeof(alg_dico_sort));
	printf("\n");
}