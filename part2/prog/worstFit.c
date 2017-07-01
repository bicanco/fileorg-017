#include<stdlib.h>
#include<stdio.h>

#include"delimitador.h"
#include"indice.h"

void insereResgistro_WorstFit(FILE *arquivo, Indice *indice, char *reg, int chave, int tamanho){
	int topo = retornaTopoArquivo(arquivo);
	int topo_anterior = -1;
	int aux, espaco, offset;
	char delimitador = DELIMITADOR;

	fseek(arquivo, topo, SEEK_SET);
	fgetc(arquivo);
	fread(&espaco, sizeof(int), 1, arquivo);

	if(espaco < tamanho){
		fseek(arquivo, 0, SEEK_END);
	}else{
	       	if((espaco-tamanho) >= 10){
			fseek(arquivo, topo, SEEK_SET);
			aux = espaco-tamanho-1;
			fwrite(&aux, sizeof(int), 1, arquivo);
			fseek(arquivo, aux-5,SEEK_CUR);
			fwrite(&delimitador,sizeof(char),1,arquivo);
		}else{
			fread(&aux, sizeof(int), 1, arquivo);
			atualizaTopoArquivo(arquivo, aux);
		}
		fseek(arquivo, topo+espaco-tamanho,SEEK_SET)
	}

	offset = ftell(arquivo);
	fwrite(reg, sizeof(char), tamanho, arquivo);
	fwrite(&delimitador, sizeof(char), 1, arquivo);

	insereIndice(indice, chave, offset);
}
