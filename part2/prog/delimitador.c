/*==============================================================*\
||  Segundo Trabalho Pr�tico - Organiza��o de Arquivos(SCC215) ||
||   				  			       ||
|| Alunos:  				N� USP: 	       ||
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
||   	delimitador.c					       ||
||   							       ||
|| Descri��o:    					       ||
||	Fun��es que manipulam um arquivo unico de campos de    ||
||	tamanho variaveis com indicador de tamanho, campos de  ||
||	tamanho fixos e registros de tamanhos variaveis com    ||
||	delimitador de final de registro, incluindo a inser��o ||
||	e busca de dados nesse padr�o.	           	       ||
\*==============================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "registro.h"
#include "delimitador.h"
#include "indice.h"

/**
	insereRegistro_Delimitador
	Cria e escreve no arquivo de saida um registro de tamanho variavel com delimitador de final de registro,
	de acordo com dados recebidos do arquivo CSV e a documentação do trabalho, 
	quanto ao tamanho, à ordem e a forma com que cada campo é armazenado.
	
	PARAMETRO -csv- | campos lidos do arquivo CSV
    PARAMETRO -fds- | arquivo de saida
**/
int insereRegistro_Delimitador(char **csv, FILE *fds){
	int tamanho;// variável que recebrá o tamanho do resgistro criado pela função criaRegistro
	char *registro;// variável que recberá o resgistro criado pela função criaRegistro
	char delimitador = (char) DELIMITADOR;//variável que guarda o delimitador
 
	registro = criaRegistro(csv, &tamanho);//cria-se o registro para escrever no arquivo
	fwrite(registro, sizeof(char), tamanho, fds);//escreve-se o registro
	fwrite(&delimitador, sizeof(char), 1, fds);//escreve-se o delimitador de final de registro

	free(registro);

	return tamanho;
}

void insereRegistro_Inicializa(char **csv, FILE *fds, Indice *indice){
	int tamanho;// variável que recebrá o tamanho do resgistro criado pela função criaRegistro
	char *registro;// variável que recberá o resgistro criado pela função criaRegistro
	char delimitador = (char) DELIMITADOR;//variável que guarda o delimitador

	registro = criaRegistro(csv, &tamanho);//cria-se o registro para escrever no arquivo

	int offset = ftell(fds);
	int ticket = retornaTicket(registro);
	
	fwrite(registro, sizeof(char), tamanho, fds);//escreve-se o registro
	fwrite(&delimitador, sizeof(char), 1, fds);//escreve-se o delimitador de final de registro

	inicializaIndice(indice, ticket, offset);

	free(registro);
}

FILE *inicializaArquivo(char *nomeArquivo){
	int cabecalhoInicial = FIM_DE_LISTA;
	//fun��o que incializa um arquivo com resgistro de cabe�alho
	FILE *arquivo = fopen(nomeArquivo, "w+");
	fwrite(&cabecalhoInicial, sizeof(int), 1, arquivo);

	return arquivo;
}

int retornaTopoArquivo(FILE *arquivo){
	int topo;
	//fun��o que retorna o conte�do do registro de cabe�alho
	fseek(arquivo, 0, SEEK_SET);
	fread(&topo, sizeof(int), 1, arquivo);

	return topo;
}

void atualizaTopoArquivo(FILE *arquivo, int novoTopo){
	fseek(arquivo, 0, SEEK_SET);
	fwrite(&novoTopo, sizeof(int), 1, arquivo);
	//fun��o que atualiza o registro de cabe�alho
}

int tamanhoListaArquivo(FILE *arquivo){
	int contador = 0;
	//fun��o que retorna o tamanho da lista de registros removidos
	int aux = retornaTopoArquivo(arquivo);
	while (aux != FIM_DE_LISTA){
		contador++;
		fseek(arquivo, aux + 1 + sizeof(int), SEEK_SET);
		fread(&aux, sizeof(int), 1, arquivo);
	}

	return contador;
}

int tamanhoRegistro_Delimitador(FILE *fp){
	int tamanho = 0;
	char c;
	//fun��o que retorna o tmanho do registro
	do {
		c = fgetc(fp);
		if (!feof(fp) && c != DELIMITADOR) tamanho++;
	} while (!feof(fp) && c != DELIMITADOR);

	return tamanho;
}

void estatisticasLista(FILE *arquivo){
	int quebraContador = 0;
	int proximo, tamanho;
	//fun��o que calcula as est�ticas da lista de registros removidos
	int aux = retornaTopoArquivo(arquivo);
	while (aux != FIM_DE_LISTA){
		fseek(arquivo, aux + 1, SEEK_SET);
		fread(&tamanho, sizeof(int), 1, arquivo);
		fread(&proximo, sizeof(int), 1, arquivo);

		printf("[%d | %d]--> ", aux, tamanho);
		
		if (++quebraContador % 4 == 0) printf("\n");
		aux = proximo;
	}

	printf("FIM\n");
}

/**
    buscaRegistro_Delimitador
    Le o registro na posicao que o ponteiro do arquivo está apontando.

    PARAMETRO -fp- | ponteiro do arquivo
    RETORNA | vetor de bytes, que contém o registro encontrado
**/
char *buscaRegistro_Delimitador(FILE *fp, int *tamanhoRegistro){
    char *registro = NULL;//váriavel que guarda o registro encontrado
    char c;//variável utilizada para verificar a condição de final de registro
    int tamanho = 0; //variável utilizada par guadar o tamanho do registro
	int aux;///variável utilizada para guadar o tamanho dos campos de tamanho variável

    c = fgetc(fp);//confere se o arquivo está no fim
    if(feof(fp)) return registro;//retorna NULL se estiver no fim
    fseek(fp, -1, SEEK_CUR);//retorna para o arquivo para a posição inicial

    registro = (char *) realloc(registro, sizeof(char)*TAM_CAMPOS_FIXOS);
    fread(registro, sizeof(char), TAM_CAMPOS_FIXOS, fp);//le os campos de tamanho fixo

    do {//leitura e criação do registro
            fread(&aux, sizeof(int), 1, fp);//leitura do tamanho do campo de tamanho variável
            registro = (char *) realloc(registro, sizeof(char)*(TAM_CAMPOS_FIXOS+tamanho+aux+4));
            memcpy(&registro[TAM_CAMPOS_FIXOS+tamanho], &aux, 4);//armazena o tamanho no registro em criação
            fread(&registro[TAM_CAMPOS_FIXOS+tamanho+4], sizeof(char), aux, fp);//le o campo de tamanho variável
            tamanho += aux+4;//atualiza o tamanho do registro
            c = fgetc(fp);//le o próximo caractere
            fseek(fp, -1,SEEK_CUR);//retorna para aposição anterior do arquivo
    } while(c != DELIMITADOR);//se for lido o delimitador, acabou o registro

    fseek(fp, 1, SEEK_CUR);//avança uma posição no arquivo(DELIMITADOR)

    *tamanhoRegistro = tamanho + TAM_CAMPOS_FIXOS;
    return registro;//retorna o registro lido em forma de char*
}
