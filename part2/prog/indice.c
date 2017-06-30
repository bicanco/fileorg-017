#include <stdlib.h>
#include <stdio.h>

#include "indice.h"

Indice *criaIndice(char *nomeArquivo){
	Indice *indice = (Indice *) calloc(1, sizeof(Indice));
	indice->arquivo = fopen(nomeArquivo, "r");
	return indice;
}

void liberaIndice(Indice *indice){
	if (indice != NULL){
		if (indice->dados != NULL) free(indice->dados);
		free(indice);
		if (indice->arquivo != NULL) fclose(indice->arquivo);
	}
}

void inicializaIndice(Indice *indice, int chave, int offset){
	indice->dados = (IndiceItem *) realloc(indice->dados, sizeof(IndiceItem) * (indice->tamanho + 1));
	indice->dados[indice->tamanho].chave = chave;
	indice->dados[indice->tamanho].offset = offset;
	indice->tamanho++;
}

void imprimeIndice(Indice *indice){
	int i;
	for (i = 0; i < indice->tamanho; i++)
		printf("C: %d\t\t| OFF: %d\n", indice->dados[i].chave, indice->dados[i].offset);
}