#include <stdlib.h>
#include <stdio.h>

#include "delimitador.h"
#include "indice.h"

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
/*
void insereRegistro_WorstFit(FILE *arquivo, Indice *indice, char *reg, int chave, int tamanho){
	int topo = retornaTopoArquivo(arquivo);
	int topo_anterior = -1;
	int aux, espaco, offset;
	char delimitador = DELIMITADOR;

	fseek(arquivo, topo + 1, SEEK_SET);
	fread(&espaco, sizeof(int), 1, arquivo);

	if(espaco < tamanho){
		fseek(arquivo, 0, SEEK_END);
	} else {
	    if((espaco-tamanho-1) >= 10){
			fseek(arquivo, topo, SEEK_SET);
			aux = espaco-tamanho-1;
			fwrite(&aux, sizeof(int), 1, arquivo);
			fseek(arquivo, aux-5,SEEK_CUR);
			fwrite(&delimitador,sizeof(char),1,arquivo);
		} else { 
			fread(&aux, sizeof(int), 1, arquivo);
			atualizaTopoArquivo(arquivo, aux);
		}
		fseek(arquivo, topo+espaco-tamanho,SEEK_SET);
	}

	offset = ftell(arquivo);
	fwrite(reg, sizeof(char), tamanho, arquivo);
	fwrite(&delimitador, sizeof(char), 1, arquivo);

	insereIndice(indice, chave, offset);
}
*/