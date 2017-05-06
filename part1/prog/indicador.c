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

char* criaRegistro_com_indicador(char** csv, int* tamanhoRegistro){
	char *registro = NULL;
	int tamanho;
	//lendo um registro com campos de tamanhos fixos e campos tamanhos variaveis respectivamente
	char *campos = criaRegistro(csv,&tamanho);
	//alocando memoria e armazenando o indicador de tamanho do registro
	registro = (char*)realloc(registro,sizeof(char)*(sizeof(int)+tamanho));
	memcpy(registro, &tamanho, sizeof(int));
	//unindo a string que contem no momento apenas o indicador de tamanho do registro com a que contem o registro
	memcpy(registro, campos, tamanho);
	(*tamanhoRegistro) = tamanho;
	return registro;
}
/**
	CriaArquivo Com Indicador
	Cria e arruma um arquivo que conterá registros de tamanho variaveis com 
	indicador de tamanho, de acordo com dados recebidos do arquivo CSV e a 
	documentação do trabalho, quanto ao tamanho, à ordem e a forma com que 
	cada campo é armazenado.

	PARAMETRO -origin- | arquivo input 
    RETORNA | vetor de bytes, que contém a string arquivo criada


**/
char* criarArquivo_Indicador(FILE *origin){
	char* registro;
	char** csv;
	int tam_registro;
	char* arquivo = NULL;
	int tam_arquivo = 0;
	//lendo um arquivo ate o fim
	while(!feof(origin)){

		csv = leCSV(origin);
	
		
//TESTANDO ---------------------------------
		for (int i = 0; i < 8; i++)
		printf("%s\n", csv[i]);
	printf("---------------------------\n");
//------------------------------------------

		//obtendo um registro de tamanho variavel com seu indicador de tamanho
		registro = criaRegistro_com_indicador(csv,&tam_registro);
		//alocando memoria e armazenando o registro na string arquivo que sera retornada
		arquivo = (char*)realloc(arquivo,sizeof(char)*(tam_arquivo + tam_registro));
		memcpy(&arquivo[tam_arquivo], registro, tam_registro);
		//incrementando o tamanho do arquivo final
		tam_arquivo += tam_registro;
	}
	//TESTANDO ---------------------------------
	printf("REGISTROOOOO\n");
		for (int i = 0; i < tam_arquivo; i++) 
		printf("%d\t%c\n", i, arquivo[i]);
	printf("terminou\n");
	//------------------------------------------
	return arquivo;
}