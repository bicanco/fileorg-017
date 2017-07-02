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
||   	firstFit.c					       ||
||   							       ||
|| Descrição:    					       ||
||     Biblioteca que aplica o método de First-Fit             ||
\*==============================================================*/
#include <stdlib.h>
#include <stdio.h>

#include "delimitador.h"
#include "indice.h"
#include "firstFit.h"
/**
 	removeRegistro_FirstFit

 	Função que remove um registro de um arquivo que utiliza o método de First-Fit
	PARAMETRO -arquivo- | o arquivo do qual se quer remover o registro
	PARAMETRO -indice- | o indice primario do arquivo de dados
	PARAMETRO -chave- | a chave de busca a do registro a ser removido
	RETORNA | 0 se não há o registro no arquivo de dados e 1 se houver
**/
int removeRegistro_FirstFit(FILE *arquivo, Indice *indice, int chave){
	int offsetRemover = offsetIndice(indice, chave);
	if (offsetRemover == -1) return 0;

	char indicador = REMOVIDO;

	fseek(arquivo, offsetRemover, SEEK_SET);
	int tamanho = tamanhoRegistro_Delimitador(arquivo);

	int antigoTopo = retornaTopoArquivo(arquivo);
	atualizaTopoArquivo(arquivo, offsetRemover);

	// remocao logica do arquivo de dados
	fseek(arquivo, offsetRemover, SEEK_SET);
	fwrite(&indicador, sizeof(char), 1, arquivo);
	fwrite(&tamanho, sizeof(int), 1, arquivo);
	fwrite(&antigoTopo, sizeof(int), 1, arquivo);

	// remocao fisica do arquivo de indice
	removeIndice(indice, chave);

	return 1;
}
/**
 	insereRegistro_FisrtFit

	Função que insere de maneira dinâmica um novo registro em um arquivo de dados segundo o mátodo First-Fit
	PARAMETRO -arquivo- | o arquivo em que se quer inserir
	PARAMETRO -indice- | o índice do arquivo em que se quer inserir
	PARAMETRO -reg- | o registro que se quer inserir
	PARAMETRO -tamanho- | o tamanho do registro que se quer inserir
	PARAMETRO -chave- | a chave de busca do novo registro

**/
void insereRegistro_FirstFit(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave){
	char delimitador = DELIMITADOR;
	char indicador = REMOVIDO;

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

		if (tamanhoAux - tamanho >= 10){ // tratar fragmentacao interna
			tamanhoAux -= tamanho; // recupera novo tamanho do espaco que sobrou
			offset = ftell(arquivo); // recupera offset do espaco que sobrou, apos a insercao dos dados

			// atualiza topo do arquivo
			antigoTopo = retornaTopoArquivo(arquivo);
			atualizaTopoArquivo(arquivo, offset);

			// coloca valores de remocao logica no espaco que sobrou
			fseek(arquivo, offset, SEEK_SET);
			fwrite(&indicador, sizeof(char), 1, arquivo);
			fwrite(&tamanhoAux, sizeof(int), 1, arquivo);
			fwrite(&antigoTopo, sizeof(int), 1, arquivo);

		} // caso nao seja capaz de colocar os dados da remocao logica, temos fragmentacao externa
	}
}
