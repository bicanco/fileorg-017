/*==============================================================*\
||  Primeiro Trabalho Prático - Organização de Arquivos(SCC215) ||
||   															||
|| Alunos:  									Nº USP: 	    ||
||   	David Souza Rodrigues							4461180 ||
||   	Fernanda Tostes Marana  						4471070 ||
||   	Gabriel Toschi de Oliveira  					9763039 ||
||   	Marcelo de Moraes Carvalho da Silva 			9791048 ||
||   															||
|| Professora:  												|| 
||   	Dra. Cristina Dutra de Aguiar Ciferri  					||
||   															||
|| Entrega:														|| 
||     	09/05/2017				 								||
||   															||
|| Arquivo: 													||
||   	indicador.c												||
||   															||
|| Descrição:													||
||    	Funções que manipulam um arquivo único de campos de 	||
||		tamanho variáveis com indicador de tamanho, campos de 	||
||		tamanho fixos e registros de tamanhos variáveis com 	||
||		indicadores de tamanho, incluindo a inserção e busca 	||
||		de dados nesse padrão.									||
\*==============================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "registro.h"
#include "CSV.h"

/**
	insereRegistro_Indicador
	Cria e escreve no arquivo de saida um registro de tamanho variavel com indicador de tamanho,
	de acordo com dados recebidos do arquivo CSV e a documentação do trabalho, 
	quanto ao tamanho, à ordem e a forma com que cada campo é armazenado.
	
	PARAMETRO -csv- | campos lidos do arquivo CSV
    PARAMETRO -fds- | arquivo de saida
**/
void insereRegistro_Indicador(char** csv, FILE *fds){
	int tamanho; //variável que contem o tamanho total do registro
	//lendo um registro com campos de tamanhos fixos e campos tamanhos variaveis respectivamente
	char *registro; // vetor de bytes que irá armazena os campos do registro e os indicadores de tamanho dos campos de tamanho variável
	registro = criaRegistro(csv,&tamanho); 
	fwrite(&tamanho,sizeof(int),1,fds); // escrevendo no arquivo binário o tamanho do registro
	fwrite(registro,sizeof(char),tamanho,fds); // escrevendo no arquivo binário o registro

	free(registro);
}

/**
    buscaRegistro_Indicador
    Le o registro na posicao que o ponteiro do arquivo está apontando.

    PARAMETRO -fp- | ponteiro do arquivo
    RETORNA | vetor de bytes, que contém o registro encontrado
**/
char *buscaRegistro_Indicador(FILE *fp){
	char *registro = NULL;//vetor de bytes que irá armazena os campos do registro e os indicadores de tamanho dos campos de tamanho variável
	int tam; //variável que contem o tamanho total do registro
	char c; //variavel que recebe o byte para o qual o ponteiro do arquivo está apontando

	c = fgetc(fp); 
    if(feof(fp)) return registro; //se chegou no final do arquivo retorna a string NULL
   	//se no chegou no final do arquivo
	fseek(fp, -1, SEEK_CUR); //leva o ponteiro do arquivo para a posição anterior

	//le o tamanho do registro do arquivo binário
	fread(&tam,sizeof(int),1,fp);
	//aloca memoria usando com base o tamanho e armazena todo registro na string registro 
	registro = (char*)realloc(registro,sizeof(char)*tam);
	fread(registro,sizeof(char),tam,fp);
	//retorna registro pronto
	return registro;
}

/**
	buscaRRN_Indicador
	Busca com o indicador de final de registro o RRN desejado.
	
	PARAMETRO -fp- | ponteiro do arquivo
	PARAMETRO -rrn- | RRN desejado
	RETORNA | vetor de bytes, que contém o registro encontrado
**/
char *buscaRRN_Indicador(FILE *fp, int rrn){
	char *registro;//vetor de bytes que irá armazena os campos do registro e os indicadores de tamanho dos campos de tamanho variável
	registro= NULL;	
	int tam; //variável que contem o tamanho total do registro
	int i = 0; // variável que será o contador
	char c;//variavel que recebe o byte para o qual o ponteiro do arquivo está apontando

	// mandando o fp para o inicio do arquivo
	fseek(fp,0,SEEK_SET);

	// procurando o rrn
	while(i != rrn && !feof(fp)){
		
		c = fgetc(fp);
		if(feof(fp)){ //se chegou no final do arquivo retorna a string NULL
			return registro;
		}
        fseek(fp, -1, SEEK_CUR);//leva o ponteiro do arquivo para a posição anteriro
		//le o tamanho do registro do arquivo binário
		fread(&tam,sizeof(int),1,fp);
		//leva o ponteiro do arquivo para o endereço do primeiro byte do indicador de tamanho do próximo registro
		fseek(fp,tam,SEEK_CUR);
		i++;
	}
	// conferindo se o arquivo está no fim
	c = fgetc(fp);
	c = fgetc(fp);
	if(feof(fp)) return registro; //se chegou no final do arquivo retorna a string NULL
	//se no chegou no final do arquivo
    fseek(fp, -2, SEEK_CUR); //leva o ponteiro do arquivo para a posições anteriores

	// lendo o tamanho do registro do arquivo binário
	fread(&tam,sizeof(int),1,fp);
	//alocando memoria usando com base o tamanho 
	registro = (char*)realloc(registro,sizeof(char)*tam);
	//armazenando o registro em uma string
	fread(registro,sizeof(char), tam, fp);

	return registro; // retornando o registro pronto
}



