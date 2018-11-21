#include "d_sort.h"

sit_dico_sort initStudyCase_s(char *name, typeIntVectorFunction ini){
	sit_dico_sort caso;
	strcpy(caso.sit.sit_name, name);
	caso.func = ini;
	return caso;
}

alg_dico_sort initAlgorithem_v(char *name,typeIntVectorFunction func, sit_dico_sort *sitDico, int ini, int mult,
                        int fin, int nTemp){
	int i;
	alg_dico_sort algoritmo;

	strcpy(algoritmo.alg.alg_name, name);
	algoritmo.func = func;
	algoritmo.alg.ini = ini;
	algoritmo.alg.mult = mult;
	algoritmo.alg.fin = fin;
	algoritmo.alg.nTemp = nTemp;

	memcpy(algoritmo.situation, sitDico, NUM_SITUATIONS * sizeof(sit_dico_sort));

	return algoritmo;
}

alg_dico_heap initAlgorithem_m(char *name,typeHeapFunction func, sit_dico_sort sitDico[], int ini, int mult,
                               int fin, int nTemp){
	int i;
	alg_dico_heap algoritmo;

	strcpy(algoritmo.alg.alg_name, name);
	algoritmo.func = func;
	algoritmo.alg.ini = ini;
	algoritmo.alg.mult = mult;
	algoritmo.alg.fin = fin;
	algoritmo.alg.nTemp = nTemp;
	ini_monticulo(&algoritmo.heap);

	memcpy(algoritmo.situation, sitDico, NUM_SITUATIONS * sizeof(sit_dico_sort));

	return algoritmo;
}

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

/*
 * TODO - Ver que parametros vamos a necesitar mostrar
 */
void printAlgorithemAndSituation_s(alg_dico_sort algoritmos[]){
	int i,j;

	for(i=0; i<NUM_ALGORITHEMS; i++){
		printf("\t - %s - tamaño vector de %d a %d de %d en %d\n", algoritmos[i].alg.alg_name,
		       algoritmos[i].alg.ini ,algoritmos[i].alg.fin, algoritmos[i].alg.mult,algoritmos[i].alg.mult);
		for(j=0; j<NUM_SITUATIONS; j++){
			printf("\t\t * situacion %s \n",algoritmos[i].situation[j].sit.sit_name);
		}
	}
	printf("\n");
}

void mostrarCotas_s(alg_dico_sort algoritmo[]){
	int j, i, k;

	int valN;
	double tiempo;

	for(i = 0; i<NUM_ALGORITHEMS; i++) {
		for (j = 0;  j< NUM_SITUATIONS; j++) {

			printf("\n-------------------------------------------------------------\n");
			printf("\nOrdenación %s con inicialización %s\n\n", algoritmo[i].alg.alg_name
					, algoritmo[i].situation[j].sit.sit_name);

			printf("   %-10s%-15s%-15s%-15s%-15s%-7s\n", "n", "t(n)",algoritmo[i].situation[j].sit.sobre.cota.name,
			       algoritmo[i].situation[j].sit.ajus.cota.name,algoritmo[i].situation[j].sit.sub.cota.name,"anomala");

			for (k = 0; k<algoritmo[i].alg.nTemp; k++) {

				valN = algoritmo[i].situation[j].sit.valN[k];
				tiempo = algoritmo[i].situation[j].sit.tiempos[k].tiempo;

				if(algoritmo[i].situation[j].sit.tiempos[k].is_under_500) {

					printf("(*)%-10d%-15.5f%-15.8f%-15.8f%-15.8f%7d\n", valN, tiempo,
					       tiempo / execute(algoritmo[i].situation[j].sit.sobre.cota, valN, algoritmo[i].situation[j].sit.sobre.exp,0),
					       tiempo / execute(algoritmo[i].situation[j].sit.ajus.cota, valN, algoritmo[i].situation[j].sit.ajus.exp,0),
					       tiempo / execute(algoritmo[i].situation[j].sit.sub.cota, valN, algoritmo[i].situation[j].sit.sub.exp,0),
					       algoritmo[i].situation[j].sit.tiempos[k].is_anomalo
					);
				} else {
					printf("   %-10d%-15.5f%-15.8f%-15.8f%-15.8f%7d\n", valN, tiempo,
					       tiempo / execute(algoritmo[i].situation[j].sit.sobre.cota, valN, algoritmo[i].situation[j].sit.sobre.exp,0),
					       tiempo / execute(algoritmo[i].situation[j].sit.ajus.cota, valN, algoritmo[i].situation[j].sit.ajus.exp,0),
					       tiempo / execute(algoritmo[i].situation[j].sit.sub.cota, valN, algoritmo[i].situation[j].sit.sub.exp,0),
					       algoritmo[i].situation[j].sit.tiempos[k].is_anomalo
					);
				}
			}
		}
	}
}

void buscarCotas_s(alg_dico_sort algoritmos[], cota_t cotas[], int numCotas){
	int i,j;

	for(i = 0; i < NUM_ALGORITHEMS; i++){
		for(j = 0; j < NUM_SITUATIONS; j++){
			acotarComplejidad(&algoritmos[i].situation[j].sit, cotas, numCotas, algoritmos[i].alg.nTemp);
			printf("ALGORITMO -> %s \n",algoritmos[i].alg.alg_name);
			printf("SITUACION -> %s \n",algoritmos[i].situation[j].sit.sit_name);
		}
	}
	mostrarCotas_s(algoritmos);
}

/* Test Lectura Tiempos */
void testTiempos_s(alg_dico_sort algoritmo[]){
	int i,j,k;
	int n;

	for( i = 0; i<NUM_ALGORITHEMS; i++){
		printf("\n\t Algoritmo > %s\n", algoritmo[i].alg.alg_name);
		for (k= 0; k<NUM_SITUATIONS; k++) {
			n = algoritmo[i].alg.ini;
			printf("\n\t\t Metodo > %s\n", algoritmo[i].situation[k].sit.sit_name);
			for (j = 0; j < algoritmo[i].alg.nTemp; j++) {
				printf("\t\t\t%d > %f \n", n,algoritmo[i].situation[k].sit.tiempos[j].tiempo);
				n = n*algoritmo[i].alg.mult;
			}
		}
	}
}