#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "CSV.h"
#include "registro.h"
#include "delimitador.h"
#include "caractere.h"
#include "indice.h"
#include "firstFit.h"

int main(int argc, char *argv[]){
	
	FILE *fp = fopen("test.csv", "r");
	FILE *saida = inicializaArquivo("test.out");
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

	printf("topo = %d\n", retornaTopoArquivo(saida));

	ordenaIndice(ind);
	imprimeIndice(ind);

	removeRegistro_FirstFit(saida, ind, 83785);
	removeRegistro_FirstFit(saida, ind, 0);
	removeRegistro_FirstFit(saida, ind, 13971449);
	removeRegistro_FirstFit(saida, ind, 135007);
	removeRegistro_FirstFit(saida, ind, 453442);
	removeRegistro_FirstFit(saida, ind, 0);
	printf("topo = %d\n", retornaTopoArquivo(saida));
	imprimeIndice(ind);
	estatisticasLista(saida);

	fclose(fp);
	fclose(saida);
	liberaIndice(ind);

	return 0;
}