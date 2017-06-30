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
||   	numeroFixo.c											||
||   															||
|| Descrição:													||
||   	Funções que manipulam um arquivo unico de campos de		||	 
||   	tamanho variaveis com indicador de tamanho, campos de   ||
||   	tamanho fixos e registros de tamanhos variaveis com     ||
||   	número fixo de campos, incluindo a inserção e busca     ||
||   	de dados nesse padrão.                                  ||
\*==============================================================*/

#include <stdlib.h>
#include <stdio.h>

#include "numeroFixo.h"
#include "registro.h"

/**
	insereRegistro_NumeroFixo
	Cria e escreve no arquivo de saida um registro de tamanho variavel com numero fixo de campos,
	de acordo com dados recebidos do arquivo CSV e a documentação do trabalho, 
	quanto ao tamanho, à ordem e a forma com que cada campo é armazenado.
	
	PARAMETRO -csv- | campos lidos do arquivo CSV
    PARAMETRO -fds- | arquivo de saida
**/
void insereRegistro_NumeroFixo(char** csv, FILE *fds){
	int tam; // tamanho do registro
	char* registro; // ponteiro para o registro

	registro = criaRegistro(csv, &tam);
	//Len = numero de bytes contidos no registro
	fwrite(registro, sizeof(char), tam, fds);

	free(registro);
}

/**
    BuscaRegistro_NumeroFixo
    Le o registro na posicao que o ponteiro do arquivo está apontando.

    PARAMETRO -fp- | ponteiro do arquivo
    RETORNA | vetor de bytes, que contém o registro encontrado
**/
char* buscaRegistro_NumeroFixo(FILE *fp){
	char *reg = NULL; // registro a ser lido do arquivo
	int tamanho; // tamanho do registro inteiro
	int i; // variavel para iteração de laço
	int tamanhoVariado; // tamanho dos campos de tamanho variavel do registro

	char c = fgetc(fp); //confere se o arquivo está no fim
    if(feof(fp)) return reg; //retorna NULL se estiver no fim
    fseek(fp, -1, SEEK_CUR); //retorna para o arquivo para a posição inicial

    // calculo do tamanho dos campos fixos
	tamanhoVariado = 0; // zerando contador do tamanho de campos de tamanho variavel
	fseek(fp, TAM_CAMPOS_FIXOS, SEEK_CUR); // pulando os campos de tamanho fixo
	for(i=0; i < NUM_CAMPOS_VARIAVEIS; i++){ // para cada campo variavel
		//Le o tamanho do proximo campo e armazena o valor em tamanho;
		fread(&tamanho, sizeof(int), 1, fp);
		fseek(fp, tamanho, SEEK_CUR); // percorre esse tamanho no arquivo
		tamanhoVariado += sizeof(int)+tamanho; // guarda o tamanho do campo
	}

	reg = (char*)malloc(sizeof(char)*(TAM_CAMPOS_FIXOS+tamanhoVariado)); // aloca memoria para o registro todo
	fseek(fp, -(TAM_CAMPOS_FIXOS+tamanhoVariado), SEEK_CUR); // volta ao inicio do registro
	fread(reg, sizeof(char), TAM_CAMPOS_FIXOS+tamanhoVariado, fp); // le todo o registro

	return reg;
}

/**
	buscaRRN_NumeroFixo
	Busca com o indicador de final de registro o rrn desejado.
	
	PARAMETRO -fp- | ponteiro do arquivo
	PARAMETRO -RRN- | rrn desejado
	RETORNA | vetor de bytes, que contém o registro encontrado
**/
char* buscaRRN_NumeroFixo(FILE *fp, int RRN){
	int RRNatual = -1;//variável que guarda o RRN atual do arquivo
	char* reg;// variável que guarda o registro encontrado

	fseek(fp, 0, SEEK_SET);//a partir do inicio do arquivo
	while(!feof(fp) && RRNatual < RRN){//enquanto não for fim de arquivo e o RRN atual é menor do que o procurado
		reg = buscaRegistro_NumeroFixo(fp);//recupera o próxmo registro do arquivo
		if (reg == NULL) return reg;//se não achar um registro no arquivo retorna NULL
		RRNatual++;//incrementa o RRN atual
		if(RRNatual < RRN) free(reg);//se não for o registro desejado, desaloca o registro
	}
	if(feof(fp)){//se percorreu o arquivo inteiro e não achou o o registro desejado retorna NULL
		return NULL;
	}
	else return reg;//retorna o registro encontrado
}
