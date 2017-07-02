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
||   	worsttFit.c					       ||
||   							       ||
|| Descrição:    					       ||
||     Biblioteca que aplica o método de Worst-Fit             ||
\*==============================================================*/
#include <stdlib.h>
#include <stdio.h>

#include "delimitador.h"
#include "indice.h"
/**
 	insereLista_Descendente

 	Função que insere o registro removido em uma lista descendente
	PARAMETRO -arquivo- | o arquivo do qual se quer remover o registro
	PARAMETRO -offset- | o byteoffset do registro que se quer remover
	PARAMETRO -tamanho- | o tamanho do registro a ser removido
**/
void insereLista_Descendente(FILE *arquivo, int offset, int tamanho){
	char indicador = REMOVIDO;
	
	int aux = retornaTopoArquivo(arquivo);
	int anterior = FIM_DE_LISTA;
	int tamanhoAux;

	while (aux != FIM_DE_LISTA){
		fseek(arquivo, aux + 1, SEEK_SET);
		fread(&tamanhoAux, sizeof(int), 1, arquivo);

		if (tamanhoAux < tamanho) break;

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
/**
 	removeRegistro_WorstFit

 	Função que remove um registro de um arquivo que utiliza o método de Worst-Fit
	PARAMETRO -arquivo- | o arquivo do qual se quer remover o registro
	PARAMETRO -indice- | o indice primario do arquivo de dados
	PARAMETRO -chave- | a chave de busca a do registro a ser removido
	RETORNA | 0 se não há o registro no arquivo de dados e 1 se houver
**/
int removeRegistro_WorstFit(FILE *arquivo, Indice *indice, int chave){
	int offsetRemover = offsetIndice(indice, chave);
	if (offsetRemover == -1) return 0;

	fseek(arquivo, offsetRemover, SEEK_SET);
	int tamanho = tamanhoRegistro_Delimitador(arquivo);

	// insere na lista de registros removidos e faz a remocao fisica
	insereLista_Descendente(arquivo, offsetRemover, tamanho);

	// remocao fisica do arquivo de indice
	removeIndice(indice, chave);

	return 1;
}
/**
 	insereRegistro_WorstFit

	Função que insere de maneira dinâmica um novo registro em um arquivo de dados segundo o mátodo Worst-Fit
	PARAMETRO -arquivo- | o arquivo em que se quer inserir
	PARAMETRO -indice- | o índice do arquivo em que se quer inserir
	PARAMETRO -reg- | o registro que se quer inserir
	PARAMETRO -tamanho- | o tamanho do registro que se quer inserir
	PARAMETRO -chave- | a chave de busca do novo registro

**/
void insereRegistro_WorstFit(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave){
	char delimitador = DELIMITADOR;
	int espaco, offset, proximo;

	int topo = retornaTopoArquivo(arquivo);
	if (topo != FIM_DE_LISTA){ // lista nao esta vazia
		fseek(arquivo, topo + 1, SEEK_SET);
		fread(&espaco, sizeof(int), 1, arquivo);
		fread(&proximo, sizeof(int), 1, arquivo);
		if (espaco >= tamanho){ // novo registro cabe, pode gerar fragmentacao interna/externa
			// atualiza topo da lista para remover o espaco preenchido
			atualizaTopoArquivo(arquivo, proximo);

			// escrever novo registro no arquivo
			fseek(arquivo, topo, SEEK_SET);
			fwrite(reg, sizeof(char), tamanho, arquivo);
			fwrite(&delimitador, sizeof(char), 1, arquivo);

			// adiciona no indice
			insereIndice(indice, chave, topo);

			if (espaco - tamanho - 1 >= 10){ // tratar fragmentacao interna
				espaco -= tamanho + 1; // recupera novo tamanho do espaco que sobrou
				offset = ftell(arquivo); // recupera offset do espaco que sobrou, apos a insercao dos dados

				// insere na lista de forma ordenada descendente
				insereLista_Descendente(arquivo, offset, espaco);

			} // caso nao seja capaz de colocar os dados da remocao logica, temos fragmentacao externa

			return;
		}
	}

	// caso a funcao nao acabou ainda, o registro deve ser colocado ao fim do arquivo
	// posiciona no fim do arquivo
	fseek(arquivo, 0, SEEK_END); 
	offset = ftell(arquivo);

	// escreve no final do arquivo
	fwrite(reg, sizeof(char), tamanho, arquivo);
	fwrite(&delimitador, sizeof(char), 1, arquivo);

	// adiciona no indice
	insereIndice(indice, chave, offset);
}
