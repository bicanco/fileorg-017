/*==============================================================*\
||  Segundo Trabalho Pr�tico - Organiza��o de Arquivos(SCC215) ||
||   				  			       ||
|| Alunos:  				N� USP: 	       ||
||   	David Souza Rodrigues			       4461180 ||
||   	Fernanda Tostes Marana  	               4471070 ||
||   	Gabriel Toschi de Oliveira  		       9763039 ||
||   	Marcelo de Moraes Carvalho da Silva 	       9791048 ||
||							       ||
|| Professora: 						       || 
||   	Dra. Cristina Dutra de Aguiar Ciferri  	               ||
||  							       ||
|| Entrega:						       || 
||     	02/07/2017					       ||
||   							       ||
|| Arquivo: 						       ||
||   	indice.c					       ||
||   							       ||
|| Descri��o:    					       ||
||	Biblioteca com fun��es de tratamento de �ndice         ||
\*==============================================================*/
#include <stdlib.h>
#include <stdio.h>

#include "indice.h"
/**
	criaIndice

	Fun��o que cria um �ndice
	RETORNA | o �ndice criado
**/
Indice *criaIndice(){
	Indice *indice = (Indice *) calloc(1, sizeof(Indice));
	return indice;
}

/**
	liberaIndice

	Fun��o que apaga um �ndice da RAM
	PARAMETRO -indice- | o �ndice que deseja-se apagar
**/
void liberaIndice(Indice *indice){
	if (indice != NULL){
		if (indice->dados != NULL) free(indice->dados);
		free(indice);
	}
}

/**
	inicializaIndice

	Fun��o que inicializa o �ndice
	PARAMETRO -indice- | o �ndice j� criado
	PARAMETRO -chave- | a chave prim�ria do primeiro item a ser colocado
	PARAMETRO -offset- | o byteoofset do registro indexado
**/
void inicializaIndice(Indice *indice, int chave, int offset){
	indice->dados = (IndiceItem *) realloc(indice->dados, sizeof(IndiceItem) * (indice->tamanho + 1));
	indice->dados[indice->tamanho].chave = chave;
	indice->dados[indice->tamanho].offset = offset;
	indice->tamanho++;
}

/**
	imprimeIndice

	Fun��o que imprime um �ndice
	PARAMETRO -indice- | o indice que deseja-se imprimir
**/
void imprimeIndice(Indice *indice){
	int i;
	for (i = 0; i < indice->tamanho; i++)
		printf("[%d] C: %d\t\t| OFF: %d\n", i, indice->dados[i].chave, indice->dados[i].offset);
}

/**
	comparaIndiceItem

	Fun��o que compara dos itens de um �ndice
	PARAMETRO -a- | um dos itens a comparar
	PARAMETRO -b- | o outro item a ser comaparado
	RETORNA | a compara��o
**/
int comparaIndiceItem(const void *a, const void *b){
	IndiceItem A = *(IndiceItem*)a;
	IndiceItem B = *(IndiceItem*)b;
	return (A.chave - B.chave);
}

/**
	ordenaIndice

	Fun��o que ordena um �ndice
	PARAMETRO -indice- | o indice que deseja-se ordenar
**/
void ordenaIndice(Indice *indice){
	qsort(indice->dados, indice->tamanho, sizeof(IndiceItem), comparaIndiceItem);
}

/**
	insereIndice

	Fun��o que insere um elemento em um �ndice
	PARAMETRO -indice- | o indice em que deseja-se inserir
	PARAMETRO -chave- | a chave primaria
	PARAMETRO -offset- | o byteoffset do registro indexado
**/
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
/**
	buscaIndice

	Fun��o que busca um elemento em um �ndice
	PARAMETRO -indice- | o indice em que deseja-se inserir
	PARAMETRO -chave- | a chave primaria de busca
	RETORNA | -1 se n�o for encontrado e o bytsoffset se for
**/
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

/**
	offsetIndice

	Fun��o que retorna o offset relacionada a chave de busaca passda
	PARAMETRO -indice- | o indice em que deseja-se inserir
	PARAMETRO -chave- | a chave primaria de busca
	RETORNA | o offset relacionado a chave passada e -1 se n�o for encontrada a chave
**/
int offsetIndice(Indice *indice, int chave){
	int posicao = buscaIndice(indice, chave);

	if (posicao != -1) return indice->dados[posicao].offset;
		else return -1;
}

/**
	dadosPosIndice

	Fun��o que retorna o item do indice na posi��o desejada
	PARAMETRO -indice- | o indice em que deseja-se inserir
	PARAMETRO -posicao- | a posi��o desejada
	RATORNA | o item da posi��o passada
**/
IndiceItem dadosPosIndice(Indice *indice, int posicao){
	if (posicao >= 0 && posicao < indice->tamanho)
		return indice->dados[posicao];

	IndiceItem retornoNulo;
	retornoNulo.offset = -1;
	retornoNulo.chave = -1;
	return retornoNulo;
}

/**
	removeIndice

	Fun��o que remove um elemento em um �ndice
	PARAMETRO -indice- | o indice em que deseja-se inserir
	PARAMETRO -chave- | a chave primaria
**/
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

/**
	tamanhoIndice

	Fun��o que retorna o tamanho do �ndice
	PARAMETRO -indice- | o indice em que deseja-se inserir
	RETORNA | o tamanho do indice
**/
int tamanhoIndice(Indice *indice){
	return indice->tamanho;
}

/**
	salvaIndice

	Fun��o que salva um �ndice em um arquivo
	PARAMETRO -indice- | o indice em que deseja-se inserir
	PARAMETRO -nomeArquivo- | o nome do arquivo em que se quer gravar o �ndice
	RETORNA | 0 se for passado um nome inv�lido e 1 se for bem sucedido
**/
int salvaIndice(Indice *indice, char *nomeArquivo){
	FILE *arquivo = fopen(nomeArquivo, "w+");
	if (arquivo == NULL) return 0;

	int i, aux;

	for (i = 0; i < indice->tamanho; i++){
		aux = indice->dados[i].chave;
		fwrite(&aux, sizeof(int), 1, arquivo);

		aux = indice->dados[i].offset;
		fwrite(&aux, sizeof(int), 1, arquivo);
	}

	fclose(arquivo);
	return 1;
}

/**
	recuperaIndice

	Fun��o que um �ndice salvo em um arquivo
	PARAMETRO -arquivo- | o arquivo de �ndice
	RETORNA | o �ndice
**/
Indice *recuperaIndice(FILE *arquivo){
	Indice *indice = criaIndice();
	int chave, offset;

	while (!feof(arquivo)){
		fread(&chave, sizeof(int), 1, arquivo);
		if (!feof(arquivo)){
			fread(&offset, sizeof(int), 1, arquivo);
			inicializaIndice(indice, chave, offset);
		}
	}

	ordenaIndice(indice);
	return indice;
}
