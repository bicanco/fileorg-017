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
		printf("[%d] C: %d\t\t| OFF: %d\n", i, indice->dados[i].chave, indice->dados[i].offset);
}

int comparaIndiceItem(const void *a, const void *b){
	IndiceItem A = *(IndiceItem*)a;
	IndiceItem B = *(IndiceItem*)b;
	return (A.chave - B.chave);
}

void ordenaIndice(Indice *indice){
	qsort(indice->dados, indice->tamanho, sizeof(IndiceItem), comparaIndiceItem);
}

void insereIndice(Indice *indice, int chave, int offset){
	int i;

	// aumenta espaco no vetor de indices
	indice->dados = (IndiceItem *) realloc(indice->dados, sizeof(IndiceItem) * (indice->tamanho + 1));

	// percorre vetor de indices do final para o comeco
	for (i = indice->tamanho-1; i >= 0; i--){
		// caso a chave seja maior que o item, encontrou a posicao de inserir
		if (chave > indice->dados[i].chave){
			indice->dados[i+1].chave = chave;
			indice->dados[i+1].offset = offset;
			break; // ja foi inserido, nao precisa mais do laco
		} else { // nao encontrou, joga o item atual para baixo e continua procurando
			indice->dados[i+1].chave = indice->dados[i].chave;
			indice->dados[i+1].offset = indice->dados[i].offset;
		}
	}

	// caso tenha jogado todo o indice pra baixo, coloque na primeira posicao
	if (i == -1){
		indice->dados[0].chave = chave;
		indice->dados[0].offset = offset;
	}

	// atualiza tamanho do indice
	indice->tamanho++;
}

int buscaIndice(Indice *indice, int chave){
	int esq, meio, dir;

	esq = 0;
	dir = indice->tamanho - 1;

	while (esq <= dir){
		meio = (esq + dir) / 2;
		if (chave == indice->dados[meio].chave) return meio;
		if (chave > indice->dados[meio].chave) esq = meio + 1;
		else dir = meio - 1;
	}

	return -1;
}

int offsetIndice(Indice *indice, int chave){
	int posicao = buscaIndice(indice, chave);

	if (posicao != -1) return indice->dados[posicao].offset;
		else return -1;
}

void removeIndice(Indice *indice, int chave){
	int i;
	int posicao = buscaIndice(indice, chave);

	if (posicao == -1) return;

	for (i = posicao + 1; i < indice->tamanho; i++){
		indice->dados[i-1].chave = indice->dados[i].chave;
		indice->dados[i-1].offset = indice->dados[i].offset;
	}

	indice->tamanho--;
	indice->dados = (IndiceItem *) realloc(indice->dados, sizeof(IndiceItem) * indice->tamanho);
}