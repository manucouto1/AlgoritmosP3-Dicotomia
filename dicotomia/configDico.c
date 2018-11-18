#include "dicotomia.h"

/*
 * DONE - funciones que se usaran para el calculo de las cotas
 */
void initFuncs(funcion *funcs){

	printf(" ************************************ \n\n");
	printf(" - Definiendo Funciones \n");
	printf(" ************************************ \n");

	funcion LOG = {"log(n)",0,0};
	funcion N = {"n",0,1};
	funcion NxLogN = {"n*log(n)",0,2};
	funcion Nexp_x = {"n^$",1,3};
	funcion Nexp_x_LOG = {"n^$*log(n)",1,4};

	funcs[0] = LOG;
	funcs[1] = N;
	funcs[2] = NxLogN;
	funcs[3] = Nexp_x;
	funcs[4] = Nexp_x_LOG;
	printf("\n");

}

double execute(funcion op , int n, double exp, int derivada){

	if (!derivada) {

		switch (op.index) {
			case 0:
				return log(n);
			case 1:
				return n;
			case 2:
				return n*log(n);
			case 3:
				return pow(n,exp);
			case 4:
				return pow(n,exp)*log(n);
			default:
				return -1;
		}

	} else {

		switch (op.index) {
			case 0:
				return 1.0/n;
			case 1:
				return 1;
			case 2:
				return log(n)+(1);
			case 3:
				return exp*pow(n,exp-1);
			case 4:
				return (exp*pow(n,exp-1)*log(n))+(pow(n,exp)*1/n);
			default:
				return -1;
		}
	}
}


