#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "registro.h"
#include "delimitador.h"

#define DELIMITADOR -1

/**
	Cria registro Com Delimitador
	Cria e escreve em no arquivo de saida um registro de tamanho variavel com delimitador de final de registro,
	de acordo com dados recebidos do arquivo CSV e a documentação do trabalho, 
	quanto ao tamanho, à ordem e a forma com que cada campo é armazenado.
	PARAMETRO -csv- | campos lidos do arquivo CSV
    	PARAMETRO -fds- | arquivo de saida
**/
void insereRegistro_Delimitador(char **csv, FILE *fds){
	int tamanho;
	char *registro;
	char delimitador = (char) DELIMITADOR;
 
	registro = criaRegistro(csv, &tamanho);//cria-se o registro para escrever no arquivo
	fwrite(registro, sizeof(char), tamanho, fds);//escreve-se o registro
	fwrite(&delimitador, sizeof(char), 1, fds);//escreve-se o delimitador de final de registro

	free(registro);
}

/**
        BuscaRegistro com delimitador de final de registro
        Le o registro na posicao que o ponteiro do arquivo está apontando.
        PARAMETRO -fp- | ponteiro do arquivo
        RETORNA | vetor de bytes, que contém o registro encontrado
**/
char *buscaRegistro_Delimitador(FILE *fp){
        char *registro = NULL;
        char c;
        int tamanho = 0, aux;

        c = fgetc(fp);
        if(feof(fp)) return registro;
        fseek(fp, -1, SEEK_CUR);

        registro = (char *) realloc(registro, sizeof(char)*FIXOS);
        fread(registro, sizeof(char), FIXOS, fp);//le os campos de tamanho fixo

        do {//le os campos de tamanho variavel ate achar o indicadro de final de registro
                fread(&aux, sizeof(int), 1, fp);
                registro = (char *) realloc(registro, sizeof(char)*(FIXOS+tamanho+aux+4));
                memcpy(&registro[FIXOS+tamanho], &aux, 4);
                fread(&registro[FIXOS+tamanho+4], sizeof(char), aux, fp);
                tamanho += aux+4;
                c = fgetc(fp);
                fseek(fp, -1,SEEK_CUR);
        } while(c != DELIMITADOR);

        fseek(fp, 1, SEEK_CUR);
        return registro;
}

/**
	Busca RRN com delimitador de final de registro
	Busca com o indicador de final de registro o rrn desejado.
	PARAMETRO -fp- | ponteiro do arquivo
	PARAMETRO -rrn- | rrn desejado
	RETORNA | vetor de bytes, que contém o registro encontrado


**/
char* buscaRRN_Delimitador(FILE *fp, int RRN){
	int i = -1;
	char* reg;

	fseek(fp, 0, SEEK_SET);
	while(!feof(fp) && i<RRN){
		reg = buscaRegistro_Delimitador(fp);
		if (reg == NULL) return reg;
		i++;
		if(i < RRN) free(reg);
	}
	if(feof(fp)){
		return NULL;
	}
	else return reg;
}

