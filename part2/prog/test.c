#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "CSV.h"
#include "registro.h"
#include "delimitador.h"
#include "caractere.h"
#include "indice.h"

int main(int argc, char *argv[]){
	
	FILE *fp = fopen("test.csv", "r");
	FILE *saida = fopen("test.out", "w");
	Indice *ind = criaIndice("test.idx");

	char **data;

	while (!feof(fp)){ // enquanto ainda houver dados
		data = leCSV(fp); // leia um registro do arquivo de entrada
		
		// insere no arquivo de saída e libera a memória alocada para ele
		insereRegistro_Inicializa(data, saida, ind);
		liberaCSV(data);

		// caso tenha chegado ao fim do arquivo, pare de ler
		if (feof(fp)) break;
	}

	imprimeIndice(ind);

	fclose(fp);
	fclose(saida);
	liberaIndice(ind);

	return 0;
}