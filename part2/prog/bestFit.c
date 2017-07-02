/*==============================================================*\
||  Segundo Trabalho Prático - Organização de Arquivos(SCC215) ||
||   				  			       ||
|| Alunos:  				Nº USP: 	       ||
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
||   	bestFit.c					       ||
||   							       ||
|| Descrição:    					       ||
||     Biblioteca que aplica o método de Best-Fit              ||
\*==============================================================*/

#include <stdlib.h>
#include <stdio.h>

#include "delimitador.h"

void insereLista_Ascendente(FILE *arquivo, int offset, int tamanho){
	char indicador = REMOVIDO;
	
	int aux = retornaTopoArquivo(arquivo);
	int anterior = FIM_DE_LISTA;
	int tamanhoAux;

	while (aux != FIM_DE_LISTA){
		fseek(arquivo, aux + 1, SEEK_SET);
		fread(&tamanhoAux, sizeof(int), 1, arquivo);

		if (tamanhoAux > tamanho) break;

		anterior = aux;
		fread(&aux, sizeof(int), 1, arquivo);
	}

	// atualizar no anterior, caso exista
	if (anterior == FIM_DE_LISTA){ // novo no fica no topo da lista
		atualizaTopoArquivo(arquivo, offset);
	} else { // tem um anterior para atualizar
		fseek(arquivo, anterior + 1 + sizeof(int), SEEK_SET);
		fwrite(&offset, sizeof(int), 1, arquivo);
	}

	// remocao logica do arquivo de dados (arruma no da lista)
	fseek(arquivo, offset, SEEK_SET);
	fwrite(&indicador, sizeof(char), 1, arquivo);
	fwrite(&tamanho, sizeof(int), 1, arquivo);
	fwrite(&aux, sizeof(int), 1, arquivo);
}

int removeRegistro_BestFit(FILE *arquivo, Indice *indice, int chave){
	int offsetRemover = offsetIndice(indice, chave);
	if (offsetRemover == -1) return 0;

	fseek(arquivo, offsetRemover, SEEK_SET);
	int tamanho = tamanhoRegistro_Delimitador(arquivo);

	// insere na lista de registros removidos e faz a remocao fisica
	insereLista_Ascendente(arquivo, offsetRemover, tamanho);

	// remocao fisica do arquivo de indice
	removeIndice(indice, chave);

	return 1;
}

void insereRegistro_BestFit(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave){
	char delimitador = DELIMITADOR;

	int aux = retornaTopoArquivo(arquivo);
	int anterior = FIM_DE_LISTA;
	int tamanhoAux, proximo, offset, antigoTopo;

	// percorre a lista
	while (aux != FIM_DE_LISTA){
		fseek(arquivo, aux + 1, SEEK_SET);
		fread(&tamanhoAux, sizeof(int), 1, arquivo);

		if (tamanhoAux >= tamanho) break; // caso ache um no com tamanho suficiente, pare

		anterior = aux; // continue iterando pela lista para o proximo no
		fread(&aux, sizeof(int), 1, arquivo);
	}

	if (aux == FIM_DE_LISTA){ // lista esta vazia/nenhum no tem espaco
		// posiciona no fim do arquivo
		fseek(arquivo, 0, SEEK_END); 
		offset = ftell(arquivo);

		// escreve no final do arquivo
		fwrite(reg, sizeof(char), tamanho, arquivo);
		fwrite(&delimitador, sizeof(char), 1, arquivo);

		// adiciona no indice
		insereIndice(indice, chave, offset);
	} else { // um espaco da lista foi escolhido
		fread(&proximo, sizeof(int), 1, arquivo); // recupera o proximo do no a ser removido da lista

		// remove o no preenchido da lista
		// atualizar no anterior, caso exista
		if (anterior == FIM_DE_LISTA){ // novo no fica no topo da lista
			atualizaTopoArquivo(arquivo, proximo);
		} else { // tem um anterior para atualizar
			fseek(arquivo, anterior + 1 + sizeof(int), SEEK_SET);
			fwrite(&proximo, sizeof(int), 1, arquivo);
		}

		// escrever novo registro no arquivo
		fseek(arquivo, aux, SEEK_SET);
		fwrite(reg, sizeof(char), tamanho, arquivo);
		fwrite(&delimitador, sizeof(char), 1, arquivo);

		// adiciona no indice
		insereIndice(indice, chave, aux);

		if (tamanhoAux - tamanho - 1 >= 10){ // tratar fragmentacao interna
			tamanhoAux -= tamanho + 1; // recupera novo tamanho do espaco que sobrou
			offset = ftell(arquivo); // recupera offset do espaco que sobrou, apos a insercao dos dados

			// insere na lista de forma ordenada ascendente
			insereLista_Ascendente(arquivo, offset, tamanhoAux);

		} // caso nao seja capaz de colocar os dados da remocao logica, temos fragmentacao externa
	}
}
