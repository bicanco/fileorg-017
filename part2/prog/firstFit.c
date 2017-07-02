#include <stdlib.h>
#include <stdio.h>

#include "delimitador.h"
#include "indice.h"
#include "firstFit.h"

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

void insereRegistro_FirstFit(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave){
	char delimitador = DELIMITADOR;
	char indicador = REMOVIDO;

	int aux = retornaTopoArquivo(arquivo);
	int anterior = FIM_DE_LISTA;
	int tamanhoAux, proximo, offset, antigoTopo;

	tamanho++; // tamanho considera o delimitador

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
/*
void insereRegistro_FirstFit1(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave){
	int topo = retornaTopoArquivo(arquivo);
	int topo_anterior = -1;
	int aux, espaco, offset;
	char delimitador = DELIMITADOR;


	while(topo != -1){
		fseek(arquivo, topo, SEEK_SET);
		fgetc(arquivo);
		fread(&espaco, sizeof(int), 1, arquivo);
		if(espaco >= tamanho){
			break;
		}
		topo_anterior = topo;
		fread(&topo, sizeof(int), 1, arquivo);		
	}



	if(topo == -1){
		fseek(arquivo, 0, SEEK_END);
	}else{
		if((espaco-tamanho) >= 10){
			fseek(arquivo, topo, SEEK_SET);
			aux = espaco-tamanho-1;
			fwrite(&aux, sizeof(int), 1, arquivo);
			fseek(arquivo, aux-5,SEEK_CUR);
			fwrite(&delimitador,sizeof(char),1,arquivo);
		}else if(topo_anterior != -1){
			fseek(arquivo, topo+5, SEEK_SET);
			fread(&aux, sizeof(int), 1, arquivo);
			fseek(arquivo, topo_anterior+5, SEEK_SET);
			fwrite(&aux, sizeof(int), 1, arquivo);
		}else{
			fread(&aux, sizeof(int), 1, arquivo);
			atualizaTopoArquivo(arquivo, aux);
		}
		fseek(arquivo, topo+espaco-tamanho,SEEK_SET);
	}
	offset = ftell(arquivo);
	fwrite(reg, sizeof(char), tamanho, arquivo);
	fwrite(&delimitador, sizeof(char), 1, arquivo);

	insereIndice(indice, chave, offset);
}*/
/*
void firstFit(FILE* fp, char **csv, int tamanho, int byteoffset, int byteoffset_anterior){
	int espaco, proximo;
	char delimitador = DELIMITADOR;

	if(byteoffset == -1){
		fseek(fp, 0,SEEK_END);
		insereRegistro_Delimitador(csv, fp);
		return;
	}

	fseek(fp, byteoffset, SEEK_SET);

	fgetc(fp);
	fread(&espaco, sizeof(int), 1, fp):
	fread(&proximo, sizeof(int), 1, fp);

	if( espaco == tamanho){
		fseek(fp, byteoffset, SEEK_SET);
		insereRegistro_Delimitador(csv,fp);
		fseek(fp, byteoffset_anterior+5,SEEK_SET);
		fwrite(&proximo, sizeof(int),1,fp);
		return;
	}else if(espaco >= tamanho){
		aux = espaco-(tamanho+1);

		fseek(fp, byteoffset+1, SEEK_SET);
		fwrite(&aux,sizeof(int),1,fp);
		fseek(fp, byteoffset+aux, SEEK_SET);
		fwrite(delimitador, sizeof(char),1,fp);
		insereRegistro_Delimitador(csv,fp);
		return;
	}else{
		firstFit(fp, csv, tamanho, proximo, byteoffset);
	}
}
*/
