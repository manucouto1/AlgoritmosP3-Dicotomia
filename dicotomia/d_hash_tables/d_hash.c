#include "d_hash.h"

/*
 * TODO - Ver que parametros vamos a necesitar mostrar
 */
void printAlgorithemAndSituation_h(alg_dico_hash *algoritmos){
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

void mostrarCotas_h(alg_dico_hash algoritmo[]){
	int j, i, k;

	int valN;
	double tiempo;

	for(i = 0; i<NUM_ALGORITHEMS; i++) {
		for (j = 0;  j< NUM_SITUATIONS; j++) {

			printf("\n-------------------------------------------------------------\n");
			printf("\nOrdenación %s con inicialización %s\n\n", algoritmo[i].alg.alg_name
					, algoritmo[i].situation[j].sit.sit_name);

			printf("   %-10s%-15s%-15s%-15s%-15s\n", "n", "t(n)",algoritmo[i].situation[j].sit.sobre.cota.name,
					algoritmo[i].situation[j].sit.ajus.cota.name,algoritmo[i].situation[j].sit.sub.cota.name);

			for (k = 0; k<algoritmo[i].alg.nTemp; k++) {

				valN = algoritmo[i].situation[j].sit.valN[k];
				tiempo = algoritmo[i].situation[j].sit.tiempos[k].tiempo;

				if(algoritmo[i].situation[j].sit.tiempos->is_under_500) {

					printf("(*)%-10d%-15.5f%-15.8f%-15.8f%-15.8f\n", valN, tiempo,
					       tiempo / execute(algoritmo[i].situation[j].sit.sobre.cota, valN, algoritmo[i].situation[j].sit.sobre.exp,0),
					       tiempo / execute(algoritmo[i].situation[j].sit.ajus.cota, valN, algoritmo[i].situation[j].sit.ajus.exp,0),
					       tiempo / execute(algoritmo[i].situation[j].sit.sub.cota, valN, algoritmo[i].situation[j].sit.sub.exp,0)
					);
				} else {
					printf("   %-10d%-15.5f%-15.8f%-15.8f%-15.8f\n", valN, tiempo,
					       tiempo / execute(algoritmo[i].situation[j].sit.sobre.cota, valN, algoritmo[i].situation[j].sit.sobre.exp,0),
					       tiempo / execute(algoritmo[i].situation[j].sit.ajus.cota, valN, algoritmo[i].situation[j].sit.ajus.exp,0),
					       tiempo / execute(algoritmo[i].situation[j].sit.sub.cota, valN, algoritmo[i].situation[j].sit.sub.exp,0)
					);
				}
			}
		}
	}
}

void buscarCotas_h(alg_dico_hash algoritmos[], cota_t cotas[], int numCotas){
	int i,j;

	for(i = 0; i < NUM_ALGORITHEMS; i++){
		for(j = 0; j < NUM_SITUATIONS; j++){
			acotarComplejidad(&algoritmos[i].situation[j].sit, cotas, numCotas, algoritmos[i].alg.nTemp);
		}
	}
	mostrarCotas_h(algoritmos);
}

void testTiempos_h(alg_dico_hash * algoritmo){
	int i,j,k;
	int n;

	for( i = 0; i<2; i++){
		printf("\n\t Algoritmo > %s\n", algoritmo[i].alg.alg_name);
		for (k= 0; k<3; k++) {
			n = algoritmo[i].alg.ini;
			printf("\n\t\t Metodo > %s\n", algoritmo[i].situation[k].sit.sit_name);
			for (j = 0; j < algoritmo[i].alg.nTemp; j++) {
				printf("\t\t\t%d > %f \n", n,algoritmo[i].situation[k].sit.tiempos[j].tiempo);
				n = n*algoritmo[i].alg.mult;
			}
		}
	}
}
/*
 * TODO - implementar el leerTiempos de Hash
 */
void lecturaTiempos_h(alg_dico_hash *algoritmo){
	int i;
	int j;
	/*
	printf(" - Leyendo tiempos \n");
	printf(" ************************************ \n");
	for(i = 0; i<2; i++){
		for(j = 0; j<3; j++) {
			leerTiempos(algoritmo[i], algoritmo[i].situation[j] ,algoritmo[i].situation[j].tiempos,
			            algoritmo[i].situation[j].valN);
		}
	}
	 */
	testTiempos_h(algoritmo);
}
