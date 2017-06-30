#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "CSV.h"
#include "registro.h"
#include "delimitador.h"
#include "caractere.h"

int main(int argc, char *argv[]){
	
	printf("ue2\n");
	FILE *fp = fopen("r", "test.csv");
	printf("ue3\n");
	FILE *saida = fopen("w", "test.out");

	//char **data = leCSV(fp);
	printf("ue\n");
	//int tam = insereRegistro_Delimitador(data, saida);

	fclose(fp);
	//fclose(saida);

	return 0;
}