#ifndef HEADER_D_SORT
#define HEADER_D_SORT
#include "../d_sort.h"
#endif
#define CACHE_FILE_NAME_M "../tmp/tmpCachedData_m.txt"

void leerTiempo_m(alg_dico_heap algoritmo, sit_dico_sort situacion, time_dico tiempos[], int *tamV){
	double ta, tb, t, ti;
	int k, n, i;
	int *v;

	typeIntVectorFunction ini = situacion.func;
	typeHeapFunction op = algoritmo.func;

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
		op(v, n, &algoritmo.heap);
		tb = microsegundos();
		t = tb - ta;

		if (t < 500) {
			ta = microsegundos();
			for (k = 0; k < 1000; k++) {
				ini(v, n);
				op(v, n, &algoritmo.heap);
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
			tiempos[i] = (time_dico){1,0,t};
			printf("\t\t > tiempo %f \n", tiempos[i].tiempo);
		} else {
			// DONE > guardar tiempo
			tiempos[i] = (time_dico){0,0,t};
			printf("\t\t > tiempo %f \n", tiempos[i].tiempo);
		}
		i++;
		free(v);
	}
}

void lecturaTiempos_m(alg_dico_heap algoritmos[]){
	int i;
	int j;

	time_dico times[256];
	printf(" - Leyendo tiempos \n");
	printf(" ************************************ \n");

	for(i = 0; i<NUM_ALGORITHEMS; i++) {
		for (j = 0; j < NUM_SITUATIONS; j++) {
			leerTiempo_m(algoritmos[i],
			             algoritmos[i].situation[j],
			             algoritmos[i].situation[j].sit.tiempos,
			             algoritmos[i].situation[j].sit.valN);
		}
	}
	//testTiempos_s(algoritmos);
}

void hundir(monticulo *heap, int mi){
	int hijoIzq = 0;
	int hijoDer = 0;
	int j;
	int aux;

	do {
		hijoIzq = 2 * mi + 1;
		hijoDer = 2 * mi + 2;
		j = mi;

		if (hijoDer <= heap->ultimo && heap->vector[hijoDer] > heap->vector[mi])
			mi = hijoDer;
		if (hijoIzq <= heap->ultimo && heap->vector[hijoIzq] > heap->vector[mi])
			mi = hijoIzq;

		aux = heap->vector[j];
		heap->vector[j] = heap->vector[mi];
		heap->vector[mi] = aux;

	} while (j != mi);
}

void ini_monticulo(monticulo *heap){
	heap->ultimo = -1;
}

void crear_monticulo(int v [], int tam, monticulo *heap){
	int i;

	heap->ultimo = tam - 1;
	memcpy(heap->vector, v, sizeof(int)*tam);

	for(i=(tam/2); i>=0; i--){
		hundir(heap, i);
	}
}
int eliminar_mayor(monticulo *heap){
	int x = 0;

	if(heap->ultimo == -1){
		x = -1;
		perror("Monticulo vacío");
	} else {
		x = heap->vector[0];
		heap->vector[0] = heap->vector[heap->ultimo];
		heap->ultimo--;
		if(heap->ultimo != -1){
			hundir(heap,0);
		}
	}
	return x;
}




void testHeap(){
	monticulo heap;
	int v[7],r[7];
	int i,j;

	ini_monticulo(&heap);
	ascendente(v,7);
	crear_monticulo(v,7,&heap);
	printf("\n");
	printf("TEST HEAP \n");
	printf("----------------------------------------------------------------------------------------------------\n");
	for(i=0; i<=heap.ultimo; i++){
		printf("|    v[%d] = %d ",i,v[i]);
	}
	printf("\n");
	printf("\n");
	printf("HEAP_______________________________________________________________________________________________\n");
	for(i=0; i<=heap.ultimo; i++){
		printf("| heap[%d] = %d ",i,heap.vector[i]);
	}
	printf("\n");
	for(j=0; j<7; j++) {
		r[j] = eliminar_mayor(&heap);
		for (i = 0; i <= heap.ultimo; i++) {
			printf("| heap[%d] = %d ", i, heap.vector[i]);
		}
		printf("\n");
	}
	for(i = 0; i < 7; i++){
		printf("|    r[%d] = %d ", i, r[i]);
	}

}

char *initAlgorithems_m(alg_dico_heap algoritmos[]){
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

	alg_dico_heap aux [NUM_ALGORITHEMS] = {
			initAlgorithem_m("crear_monticulo", crear_monticulo, situations, 500, 2, 32000, 7),
			//initAlgorithem_s("Quicksort", ord_rapida, situations, 1000, 10, (int) pow(10,8), 6)
	};

	memcpy(algoritmos, aux, NUM_ALGORITHEMS * sizeof(alg_dico_sort));
	printf("\n");
	return CACHE_FILE_NAME_M;
}