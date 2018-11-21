#ifndef HEADER_D_HASH
#define HEADER_D_HASH
#include "d_hash.h"
#endif

void printAlgorithemAndSituation_h(alg_dico_hash *algoritmos){
	int i,j;

	for(i=0; i<NUM_ALGORITHEMS; i++){
		printf("\t - %s \n", algoritmos[i].alg.alg_name);
		for(j=0; j<NUM_SITUATIONS; j++){
			printf("\t\t * situacion %s \n",algoritmos[i].situation[j].sit.sit_name);
		}
	}
	printf("\n");
}

sit_dico_hash initStudyCase_h(char *name, typeResColision res_colision, tabla_cerrada diccionario){
	sit_dico_hash caso;
	strcpy(caso.sit.sit_name, name);
	caso.func = res_colision;
	caso.diccionario = diccionario;
	return caso;
}

alg_dico_hash initAlgorithem_h(char *name, item datos[], typeDispersion func, type_buscar_cerrada buscar_cerrada, type_insertar_datos insertar_datos,
                               sit_dico_hash sitDico[], int ini, int fin, int mult, int nTemp){
	int i;
	alg_dico_hash algoritmo;
	algoritmo.datos = malloc(sizeof(item)*NUM_ENTRADAS);
	memcpy(algoritmo.datos, datos, NUM_ENTRADAS * sizeof(item)); // Esto a vere si no casca
	strcpy(algoritmo.alg.alg_name, name);
	algoritmo.func = func;
	algoritmo.buscar_cerrada = buscar_cerrada;
	algoritmo.insertar_datos = insertar_datos;
	algoritmo.alg.ini = ini;
	algoritmo.alg.mult = mult;
	algoritmo.alg.fin = fin;
	algoritmo.alg.nTemp = nTemp;

	memcpy(algoritmo.situation, sitDico, NUM_SITUATIONS * sizeof(sit_dico_hash));

	return algoritmo;
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

			printf("   %-10s%-15st(n)/%-10st(n)/%-10st(n)/%-10s%7s\n", "n", "t(n)",algoritmo[i].situation[j].sit.sobre.cota.name,
					algoritmo[i].situation[j].sit.ajus.cota.name,algoritmo[i].situation[j].sit.sub.cota.name,"anomalo");

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

void buscarCotas_h(alg_dico_hash algoritmos[], cota_t cotas[], int numCotas){
	int i,j;
	for(i = 0; i < NUM_ALGORITHEMS; i++){
		printf("ALGORITMO -> %s \n",algoritmos[i].alg.alg_name);
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

void leerTiempo_h(alg_dico_hash algoritmo, sit_dico_hash situacion, time_dico tiempos[], int valoresN[]){
	double ta, tb, t, ti;
	int k, n, i, aleatorio, j = 0, colision = 0;

	typeResColision resColision = situacion.func;
	typeDispersion dispersion = algoritmo.func;
	type_buscar_cerrada buscar_cerrada = algoritmo.buscar_cerrada;

	int ini = algoritmo.alg.ini;
	int fin = algoritmo.alg.fin;
	int mult = algoritmo.alg.mult;

	printf("ALGORITMO > %s\n",algoritmo.alg.alg_name);
	printf("\tSITUATION > %s\n",situacion.sit.sit_name);

	for(n = ini; n<=fin; n*=mult){
		valoresN[j]=n;
		ta = microsegundos();
		for(i=0; i<n; i++){
			aleatorio = rand() % MAX_N;
			buscar_cerrada(situacion.diccionario[aleatorio].clave, situacion.diccionario, MAX_N, &colision, dispersion, resColision);
		}
		tb = microsegundos();
		t = tb - ta;

		ta = microsegundos();
		for (i = 0; i < n; i++) {
			aleatorio = rand() % MAX_N;
		}
		tb = microsegundos();
		ti = tb - ta;
		t = t - ti;
		if (t < 500) {
			ta = microsegundos();
			for (k = 0; k < 1000; k++) {
				for (i = 0; i < n; i++) {
					aleatorio = rand() % MAX_N;
					buscar_cerrada(situacion.diccionario[aleatorio].clave, situacion.diccionario, MAX_N, &colision, dispersion, resColision);
				}
			}
			tb = microsegundos();
			t = tb - ta;

			ta = microsegundos();
			for (k = 0; k < 1000; k++) {
				for (i = 0; i < n; i++) {
					aleatorio = rand() % MAX_N;
				}
			}
			tb = microsegundos();

			ti = tb - ta;
			t = (t - ti) / k;
			tiempos[j] = (time_dico){1,0,t};
			printf("\t\t > tiempo %f *\n", tiempos[j].tiempo);

		} else {
			tiempos[j] = (time_dico){0,0,t};
			printf("\t\t > tiempo %f \n", tiempos[j].tiempo);
		}
		j++;
	}


}

/*
 * TODO - implementar el leerTiempos de Hash
 */
void lecturaTiempos_h(alg_dico_hash algoritmo[]){
	int i;
	int j;

	printf(" - Leyendo tiempos \n");
	printf(" ************************************ \n");
	for(i = 0; i<NUM_ALGORITHEMS; i++){
		for(j = 0; j<NUM_SITUATIONS; j++) {
			if(algoritmo[i].insertar_datos(algoritmo[i].datos,
					&algoritmo[i].situation[j].diccionario,
					algoritmo[i].func,
					algoritmo[i].situation[j].func)) {

				leerTiempo_h(algoritmo[i], algoritmo[i].situation[j], algoritmo[i].situation[j].sit.tiempos,
				             algoritmo[i].situation[j].sit.valN);
			} else {
				printf(" - PROBLEMAS al insertando datos para %s con %s\n", algoritmo[i].alg.alg_name, algoritmo[i].situation[j].sit.sit_name);
			}
		}
	}
	//testTiempos_h(algoritmo);
}
