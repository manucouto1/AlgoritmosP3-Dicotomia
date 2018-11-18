#include "readWriteData.h"

int cacheTimeData(alg_dico alg[]) {
	int i;
	int j;
	int k;
	char aux[256];

	FILE *file;

	file = fopen(CACHE_FILE_NAME, "w");

	if (file != NULL){
		for (i = 0; i < NUM_ALGORITHEMS; i++) {
			sprintf(aux, "$%s$%d\n", alg[i].alg_name,alg[i].nTemp);
			fputs(aux, file);
			for (j = 0; j < NUM_SITUATIONS; j++) {
				sprintf(aux, "&%s\n", alg[i].situation[j].sit_name);
				fputs(aux, file);
				for (k = 0; k < alg[i].nTemp; k++) {
					sprintf(aux, "*%d,%f,%d\n",
					        alg[i].situation[j].valN[k],
					        alg[i].situation[j].tiempos[k].tiempo,
					        alg[i].situation[j].tiempos[k].is_under_500);
					fputs(aux, file);
				}
			}
		}
		fclose(file);
	} else {
		printf("ERROR al crear/abrir ficero ");
		return -1;
	}
	return 0;
}

int loadCachedTime(alg_dico alg[]) {
	int i = 0, j = 0, k = 0;
	FILE *file;
	char buff[1024];

	char *algAux[NUM_ALGORITHEMS];
	char *sitAux[NUM_SITUATIONS];
	char *data1[1024];
	char *data2[1024];
	char *data3[1024];
	char *aux;
	char *aux2;

	file = fopen(CACHE_FILE_NAME, "r");

	if (file != NULL) {
		while (fgets(buff,1024,file)) {
			if(strchr(buff,'$')){
				j = 0;
				aux = strtok(buff,"$");
				strcpy(alg[i].alg_name,aux);
				aux = strtok(NULL,"$");
				alg[i].nTemp = (int)strtol(aux,NULL,10);
				i++;
			}
			if(strchr(buff,'&')){
				k = 0;
				aux = strtok(buff,"&");
				strcpy(alg[i-1].situation[j].sit_name,aux);
				j++;
			}
			if(strchr(buff,'*')){
				aux = strtok(buff,"*");
				aux2 = malloc(strlen(aux)+1);
				strcpy(aux2,aux);

				alg[i-1].situation[j-1].valN[k] = (int)strtol(strtok(aux2,","),NULL,10);
				alg[i-1].situation[j-1].tiempos[k].tiempo = strtol(strtok(NULL,","),NULL,10);
				alg[i-1].situation[j-1].tiempos[k].is_under_500 = (int)strtol(strtok(NULL,","),NULL,10);
				k++;
			}
		}

	} else {
		fclose(file);
		perror("ERROR reading file ");
		return -1;
	}
	testTiempos(alg);
	fclose(file);
	return 0;
}
