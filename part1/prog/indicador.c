#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "registro.h"
#include "CSV.h"

/**
	CriaRegistro Com Indicador
	Cria e arruma um registro de tamanho variavel com indicador de tamanho,
	de acordo com dados recebidos do arquivo CSV e a documentação do trabalho, 
	quanto ao tamanho, à ordem e a forma com que cada campo é armazenado.

	PARAMETRO -csv- | campos lidos do arquivo CSV
    PARAMETRO -tamanhoRegistro- | endereco de um inteiro, para ser retornado o tamanho do registro
    RETORNA | vetor de bytes, que contém o registro criado


**/

void insereRegistro_Indicador(char** csv, FILE *fds){
	int tamanho;
	//lendo um registro com campos de tamanhos fixos e campos tamanhos variaveis respectivamente
	char *registro = criaRegistro(csv,&tamanho);
	fwrite(&tamanho,sizeof(int),1,fds);
	fwrite(registro,sizeof(char),tamanho,fds);

}

/*
	BuscaRegistro Com Indicador
	Le o registro na posicao que o ponteiro do arquivo está apontando.
	PARAMETRO -fp- | ponteiro do arquivo
	RETORNA | vetor de bytes, que contém o registro criado

*/
char *buscaRegistro_Indicador(FILE *fp){
	char *registro= NULL;
	int tam;
	char c;

	c = fgetc(fp);
    if(feof(fp)) return registro;
    fseek(fp, -1, SEEK_CUR);	

	//le o tamanho do registro
	fread(&tam,sizeof(int),1,fp);
	//aloca memoria e armazena todo registro
	registro = (char*)realloc(registro,sizeof(char)*tam);
	fread(registro,sizeof(char),tam,fp);
	//retorna registro pronto
	return registro;
}
/*
	BuscaRRN Com Indicador
	Busca com o indicador de tamanho o rrn desejado.
	PARAMETRO -fp- | ponteiro do arquivo
	PARAMETRO -rrn- | rrn desejado
	RETORNA | vetor de bytes, que contém o registro criado

*/
char *buscaRRN_Indicador(FILE *fp, int rrn){
	char *registro= NULL;
	int tam;
	int pos = ftell(fp); //vai armazenar a posicao original do fp
	int i = 0;
	char c;

	// mandando o fp para o inicio do arquivo
	fseek(fp,0,SEEK_SET);

	// procurando o rrn
	while(i != rrn && !feof(fp)){
		c = fgetc(fp);
		if(feof(fp)){
			return registro;
		}
        fseek(fp, -1, SEEK_CUR);

		fread(&tam,sizeof(int),1,fp);
		fseek(fp,tam,SEEK_CUR);
		i++;
	}

	c = fgetc(fp);
	c = fgetc(fp);
	if(feof(fp)) return registro;
    fseek(fp, -2, SEEK_CUR);

	// lendo o tamanho do registro
	fread(&tam,sizeof(int),1,fp);
	//alocando memoria e guardando o registro numa string
	registro = (char*)realloc(registro,sizeof(char)*tam);
	fread(registro,sizeof(char), tam, fp);
	//vontado fp para a posicao que estava no inicio da funcao
	fseek(fp,pos,SEEK_SET);

	return registro;
}



