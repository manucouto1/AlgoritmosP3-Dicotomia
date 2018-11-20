#ifndef HEADER_D_SORT
#define HEADER_D_SORT
#include "../d_sort.h"
#endif

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

void crear_montuculo(int v [], int tam, monticulo *heap){
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
	crear_montuculo(v,7,&heap);
	printf("\n");
	printf("Vector inicial \n");
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