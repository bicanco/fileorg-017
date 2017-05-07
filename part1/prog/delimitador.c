#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"registro.h"
#include"delimitador.h"

/**
	Cria registro Com Delimitador
	Cria e escreve em no arquivo de saida um registro de tamanho variavel com delimitador de final de registro,
	de acordo com dados recebidos do arquivo CSV e a documentação do trabalho, 
	quanto ao tamanho, à ordem e a forma com que cada campo é armazenado.
	PARAMETRO -csv- | campos lidos do arquivo CSV
    	PARAMETRO -fds- | arquivo de saida
**/
void criaRegistro_com_delimitador(char **csv, FILE *fds){
	int tamanho;
	char *registro;

	registro = criaRegistro(csv, &tamanho);//cria-se o registro para escrever no arquivo
	fwrite(registro, sizeof(char), tamanho, fds);//escreve-se o registro
	fwrite("#", sizeof(char), 1, fds);//escreve-se o delimitador de final de registro

}
/**
	Busca RRN com delimitador de final de registro
	Busca com o indicador de final de registro o rrn desejado.
	PARAMETRO -fp- | ponteiro do arquivo
	PARAMETRO -rrn- | rrn desejado
	RETORNA | vetor de bytes, que contém o registro encontrado


**/
char *buscaRRN_Delimitador(FILE *fp, int rrn){
	int i = 0, tamanho, aux;
	char c;
	char *registro = NULL;

	fseek(fp, 0, SEEK_SET);
	if(rrn != 0)fseek(fp, FIXOS, SEEK_CUR);
	while(i != rrn && !feof(fp)){//procura pelo RRN desejado
		fread(&tamanho, sizeof(int), 1, fp);
		fseek(fp, tamanho, SEEK_CUR);
		c = fgetc(fp);
		if(!feof(fp))fseek(fp, -1, SEEK_CUR);
		if(c == '#'){//quando acha o delimitador de final de registro passa para o proximo registro
			rrn++;
			fseek(fp, FIXOS, SEEK_CUR);
		}
	}
	if(feof(fp))return registro;//se nao acho o RRN retorna NULL
	
	registro = (char *) realloc(registro, sizeof(char)*FIXOS);
	fread(registro, sizeof(char), FIXOS, fp);
	tamanho = 0;
	do{//escrevendo o registro em uma string (char *)
		fread(&aux, sizeof(int), 1, fp);
		registro = (char *) realloc(registro, sizeof(char)*(FIXOS+tamanho+aux+4));
		memcpy(&aux,&registro[FIXOS+tamanho], 4);
		fread(&registro[FIXOS+tamanho+4], sizeof(char), aux, fp);
		tamanho += aux+4;
		c = fgetc(fp);
		fseek(fp, -1,SEEK_CUR);
	}while(c != '#' && !feof(fp));

	return registro;//retorna o registro encontrado
}
