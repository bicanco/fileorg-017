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
#include "bestFit.h"
#include "worstFit.h"

int main(int argc, char *argv[]){
	
	FILE *fp = fopen("test.csv", "r");
	FILE *saida = inicializaArquivo("test.out");
	Indice *ind = criaIndice("test.idx");

	int i;
	char *reg;
	char **data;
	//while (!feof(fp)){ // enquanto ainda houver dados
	for (i = 0; i < 7; i++){
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

	data = leCSV(fp); // leia um registro do arquivo de entrada
	int tam, tam2;
	reg = criaRegistro(data, &tam);
	int ticket = retornaTicket(reg);
	printf("tam = %d, ticket = %d\n", tam, ticket);

	insereRegistro_FirstFit(saida, ind, reg, tam, ticket);
	imprimeIndice(ind);
	estatisticasLista(saida);
	
	for (i = 0; i < ind->tamanho; i++){
		fseek(saida, ind->dados[i].offset, SEEK_SET);
		tam = tamanhoRegistro_Delimitador(saida);

		fseek(saida, ind->dados[i].offset, SEEK_SET);
		reg = buscaRegistro_Delimitador(saida, &tam2);
		imprimeRegistro(reg);

		printf(">>>> tam = %d, tam2 = %d\n", tam, tam2);
	}

	fclose(fp);
	fclose(saida);
	liberaIndice(ind);

	return 0;
}