#include "d_sort.h"

sit_dico_sort initStudyCase_s(char *name, typeIntVectorFunction ini){
	sit_dico_sort caso;
	strcpy(caso.sit.sit_name, name);
	caso.func = ini;
	return caso;
}

alg_dico_sort initAlgorithem_s(char *name,typeIntVectorFunction func, sit_dico_sort *sitDico, int ini, int mult,
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
void testTiempos_s(alg_dico_sort * algoritmo){
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


void loadStaticData(alg_dico_sort *algoritmos){
	int i;
	int j;

	int valN1[7] = {
			500,
			1000,
			2000,
			4000,
			8000,
			16000,
			32000,
	};

	double tiemposAleatorio1[7] = {
			168.447,
			629.000,
			2541.000,
			10695.000,
			41743.000,
			164947.000,
			662970.000
	};
	double tiemposAscendente1[7] = {
			1.677,
			3.505,
			7.423,
			14.408,
			28.021,
			57.572,
			113.584
	};
	double tiemposDescendente1[7] = {
			325.930,
			1272.000,
			5045.000,
			21478.000,
			83554.000,
			333090.000,
			1329286.000
	};

	int valN2[6] = {
			1000,
			10000,
			100000,
			1000000,
			10000000,
			100000000,
	};

	double tiemposAleatorio2[6] = {
			88.181 ,
			1104.000,
			13402.000,
			157266.000,
			1785411.000,
			21578922.000
	};
	double tiemposAscendente2[6] = {
			29.084,
			357.952,
			4295.000,
			48106.000,
			545784.000,
			6603134.000
	};
	double tiemposDescendente2[6] = {
			32.999,
			374.958,
			4374.000,
			49870.000,
			556804.000,
			6727867.000,
	};

	double *tiemposSit[7] =
			{
					tiemposAleatorio1,
					tiemposAscendente1,
					tiemposDescendente1
			};

	double *tiemposSit2[6] =
			{
					tiemposAleatorio2,
					tiemposAscendente2,
					tiemposDescendente2
			};

	algoritmos[0].alg.nTemp = 7;
	for (i = 0; i< NUM_SITUATIONS; i++)
		for (j = 0; j < algoritmos[0].alg.nTemp; ++j) {
			algoritmos[0].situation[i].sit.tiempos[j].tiempo = tiemposSit[i][j];
			algoritmos[0].situation[i].sit.valN[j] = valN1[j];
			algoritmos[0].situation[i].sit.tiempos[j].is_under_500 = 0;
		}

	algoritmos[1].alg.nTemp = 6;
	for (i = 0; i< NUM_SITUATIONS; i++)
		for (j = 0; j < 6; ++j) {
			algoritmos[1].situation[i].sit.tiempos[j].tiempo = tiemposSit2[i][j];
			algoritmos[1].situation[i].sit.valN[j] = valN2[j];
			algoritmos[1].situation[i].sit.tiempos[j].is_under_500 = 0;
		}
}

void cargarTiemposEstaticos_s(alg_dico_sort algoritmos[]){
	loadStaticData(algoritmos);
	testTiempos_s(algoritmos);
}

/*
 * DONE Leer los tiempos de un algoritmo en una situacion concreta
 */
void leerTiempo_s(alg_dico_sort algoritmo, sit_dico_sort situacion, time_dico tiempos[], int *tamV){
	double ta, tb, t, ti;
	int k, n, i;
	int *v;

	typeIntVectorFunction ini = situacion.func;
	typeIntVectorFunction ord = algoritmo.func;

	int inicio = algoritmo.alg.ini;
	int fin = algoritmo.alg.fin;
	int mult = algoritmo.alg.mult;

	i = 0;
	printf("ALGORITMO > %s\n",algoritmo.alg.alg_name);
	printf("\tSITUATION > %s\n",situacion.sit.sit_name);
	for (n = inicio; n <= fin; n = n * mult) {
		tamV[i]=n;
		v = malloc(sizeof(int) * n);
		ini(v, n);
		ta = microsegundos();
		ord(v, n);
		tb = microsegundos();
		t = tb - ta;

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
			printf("\t\t > tiempo %f \n", tiempos[i].tiempo);
		} else {
			// DONE > guardar tiempo
			tiempos[i] = (time_dico){0,t};
			printf("\t\t > tiempo %f \n", tiempos[i].tiempo);
		}
		i++;
		free(v);
	}
}

void lecturaTiempos_s(alg_dico_sort algoritmos[]){
	int i;
	int j;

	time_dico times[256];
	printf(" - Leyendo tiempos \n");
	printf(" ************************************ \n");

	for(i = 0; i<NUM_ALGORITHEMS; i++) {
		for (j = 0; j < NUM_SITUATIONS; j++) {
			leerTiempo_s(algoritmos[i],
			             algoritmos[i].situation[j],
			             algoritmos[i].situation[j].sit.tiempos,
			             algoritmos[i].situation[j].sit.valN);
		}
	}
	//testTiempos_s(algoritmos);
}
