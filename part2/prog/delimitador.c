/*==============================================================*\
||  Segundo Trabalho Pr·tico - OrganizaÁ„o de Arquivos(SCC215) ||
||   				  			       ||
|| Alunos:  				N∫ USP: 	       ||
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
|| DescriÁ„o:    					       ||
||	FunÁıes que manipulam um arquivo unico de campos de    ||
||	tamanho variaveis com indicador de tamanho, campos de  ||
||	tamanho fixos e registros de tamanhos variaveis com    ||
||	delimitador de final de registro, incluindo a inserÁ„o ||
||	e busca de dados nesse padr„o.	           	       ||
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
	de acordo com dados recebidos do arquivo CSV e a documenta√ß√£o do trabalho, 
	quanto ao tamanho, √† ordem e a forma com que cada campo √© armazenado.
	
	PARAMETRO -csv- | campos lidos do arquivo CSV
    PARAMETRO -fds- | arquivo de saida
**/
int insereRegistro_Delimitador(char **csv, FILE *fds){
	int tamanho;// vari√°vel que recebr√° o tamanho do resgistro criado pela fun√ß√£o criaRegistro
	char *registro;// vari√°vel que recber√° o resgistro criado pela fun√ß√£o criaRegistro
	char delimitador = (char) DELIMITADOR;//vari√°vel que guarda o delimitador
 
	registro = criaRegistro(csv, &tamanho);//cria-se o registro para escrever no arquivo
	fwrite(registro, sizeof(char), tamanho, fds);//escreve-se o registro
	fwrite(&delimitador, sizeof(char), 1, fds);//escreve-se o delimitador de final de registro

	free(registro);

	return tamanho;
}

void insereRegistro_Inicializa(char **csv, FILE *fds, Indice *indice){
	int tamanho;// vari√°vel que recebr√° o tamanho do resgistro criado pela fun√ß√£o criaRegistro
	char *registro;// vari√°vel que recber√° o resgistro criado pela fun√ß√£o criaRegistro
	char delimitador = (char) DELIMITADOR;//vari√°vel que guarda o delimitador

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
	//funÁ„o que incializa um arquivo com resgistro de cabeÁalho
	FILE *arquivo = fopen(nomeArquivo, "w+");
	fwrite(&cabecalhoInicial, sizeof(int), 1, arquivo);

	return arquivo;
}

int retornaTopoArquivo(FILE *arquivo){
	int topo;
	//funÁ„o que retorna o conte˙do do registro de cabeÁalho
	fseek(arquivo, 0, SEEK_SET);
	fread(&topo, sizeof(int), 1, arquivo);

	return topo;
}

void atualizaTopoArquivo(FILE *arquivo, int novoTopo){
	fseek(arquivo, 0, SEEK_SET);
	fwrite(&novoTopo, sizeof(int), 1, arquivo);
	//funÁ„o que atualiza o registro de cabeÁalho
}

int tamanhoListaArquivo(FILE *arquivo){
	int contador = 0;
	//funÁ„o que retorna o tamanho da lista de registros removidos
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
	//funÁ„o que retorna o tmanho do registro
	do {
		c = fgetc(fp);
		if (!feof(fp) && c != DELIMITADOR) tamanho++;
	} while (!feof(fp) && c != DELIMITADOR);

	return tamanho;
}

void estatisticasLista(FILE *arquivo){
	int quebraContador = 0;
	int proximo, tamanho;
	//funÁ„o que calcula as estÌticas da lista de registros removidos
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
    Le o registro na posicao que o ponteiro do arquivo est√° apontando.

    PARAMETRO -fp- | ponteiro do arquivo
    RETORNA | vetor de bytes, que cont√©m o registro encontrado
**/
char *buscaRegistro_Delimitador(FILE *fp, int *tamanhoRegistro){
    char *registro = NULL;//v√°riavel que guarda o registro encontrado
    char c;//vari√°vel utilizada para verificar a condi√ß√£o de final de registro
    int tamanho = 0; //vari√°vel utilizada par guadar o tamanho do registro
	int aux;///vari√°vel utilizada para guadar o tamanho dos campos de tamanho vari√°vel

    c = fgetc(fp);//confere se o arquivo est√° no fim
    if(feof(fp)) return registro;//retorna NULL se estiver no fim
    fseek(fp, -1, SEEK_CUR);//retorna para o arquivo para a posi√ß√£o inicial

    registro = (char *) realloc(registro, sizeof(char)*TAM_CAMPOS_FIXOS);
    fread(registro, sizeof(char), TAM_CAMPOS_FIXOS, fp);//le os campos de tamanho fixo

    do {//leitura e cria√ß√£o do registro
            fread(&aux, sizeof(int), 1, fp);//leitura do tamanho do campo de tamanho vari√°vel
            registro = (char *) realloc(registro, sizeof(char)*(TAM_CAMPOS_FIXOS+tamanho+aux+4));
            memcpy(&registro[TAM_CAMPOS_FIXOS+tamanho], &aux, 4);//armazena o tamanho no registro em cria√ß√£o
            fread(&registro[TAM_CAMPOS_FIXOS+tamanho+4], sizeof(char), aux, fp);//le o campo de tamanho vari√°vel
            tamanho += aux+4;//atualiza o tamanho do registro
            c = fgetc(fp);//le o pr√≥ximo caractere
            fseek(fp, -1,SEEK_CUR);//retorna para aposi√ß√£o anterior do arquivo
    } while(c != DELIMITADOR);//se for lido o delimitador, acabou o registro

    fseek(fp, 1, SEEK_CUR);//avan√ßa uma posi√ß√£o no arquivo(DELIMITADOR)

    *tamanhoRegistro = tamanho + TAM_CAMPOS_FIXOS;
    return registro;//retorna o registro lido em forma de char*
}
