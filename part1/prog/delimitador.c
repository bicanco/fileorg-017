/**
    delimitador
    Funções que manipulam um arquivo unico de campos de tamanho
    variaveis com indicador de tamanho, campos de tamanho fixos e
    registros de tamanhos variaveis com delimitador de final de registro, 
    incluindo a inserção e busca de dados nesse padrão.
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "registro.h"
#include "delimitador.h"

/**
	CONSTANTE: DELIMITADOR
	Caractere usado como delimitador entre registros.
**/
#define DELIMITADOR -1

/**
	insereRegistro_Delimitador
	Cria e escreve no arquivo de saida um registro de tamanho variavel com delimitador de final de registro,
	de acordo com dados recebidos do arquivo CSV e a documentação do trabalho, 
	quanto ao tamanho, à ordem e a forma com que cada campo é armazenado.
	
	PARAMETRO -csv- | campos lidos do arquivo CSV
    PARAMETRO -fds- | arquivo de saida
**/
void insereRegistro_Delimitador(char **csv, FILE *fds){
	int tamanho;// variável que recebrá o tamanho do resgistro criado pela função criaRegistro
	char *registro;// variável que recberá o resgistro criado pela função criaRegistro
	char delimitador = (char) DELIMITADOR;//variável que guarda o delimitador
 
	registro = criaRegistro(csv, &tamanho);//cria-se o registro para escrever no arquivo
	fwrite(registro, sizeof(char), tamanho, fds);//escreve-se o registro
	fwrite(&delimitador, sizeof(char), 1, fds);//escreve-se o delimitador de final de registro

	free(registro);
}

/**
    buscaRegistro_Delimitador
    Le o registro na posicao que o ponteiro do arquivo está apontando.

    PARAMETRO -fp- | ponteiro do arquivo
    RETORNA | vetor de bytes, que contém o registro encontrado
**/
char *buscaRegistro_Delimitador(FILE *fp){
        char *registro = NULL;//váriavel que guarda o registro encontrado
        char c;//variável utilizada para verificar a condição de final de registro
        int tamanho = 0; //variável utilizada par guadar o tamanho do registro
	int aux;///variável utilizada para guadar o tamanho dos campos de tamanho variável

        c = fgetc(fp);//confere se o arquivo está no fim
        if(feof(fp)) return registro;//retorna NULL se estiver no fim
        fseek(fp, -1, SEEK_CUR);//retorna para o arquivo para a posição inicial

        registro = (char *) realloc(registro, sizeof(char)*FIXOS);
        fread(registro, sizeof(char), FIXOS, fp);//le os campos de tamanho fixo

        do {//leitura e criação do registro
                fread(&aux, sizeof(int), 1, fp);//leitura do tamanho do campo de tamanho variável
                registro = (char *) realloc(registro, sizeof(char)*(FIXOS+tamanho+aux+4));
                memcpy(&registro[FIXOS+tamanho], &aux, 4);//armazena o tamanho no registro em criação
                fread(&registro[FIXOS+tamanho+4], sizeof(char), aux, fp);//le o campo de tamanho variável
                tamanho += aux+4;//atualiza o tamanho do registro
                c = fgetc(fp);//le o próximo caractere
                fseek(fp, -1,SEEK_CUR);//retorna para aposição anterior do arquivo
        } while(c != DELIMITADOR);//se for lido o delimitador, acabou o registro

        fseek(fp, 1, SEEK_CUR);//avança uma posição no arquivo(DELIMITADOR)
        return registro;//retorna o registro lido em forma de char*
}

/**
	buscaRRN_Delimitador
	Busca com o indicador de final de registro o RRN desejado.
	
	PARAMETRO -fp- | ponteiro do arquivo
	PARAMETRO -RRN- | rrn desejado
	RETORNA | vetor de bytes, que contém o registro encontrado
**/
char* buscaRRN_Delimitador(FILE *fp, int RRN){
	int RRNatual = -1;//variável que guarda o RRN atual do arquivo
	char* reg;// variável que guarda o registro encontrado

	fseek(fp, 0, SEEK_SET);//a partir do inicio do arquivo
	while(!feof(fp) && RRNatual<RRN){//enquanto não for fim de arquivo e o RRN atual é menor do que o procurado
		reg = buscaRegistro_Delimitador(fp);//recupera o próxmo registro do arquivo
		if (reg == NULL) return reg;//se não achar um registro no arquivo retorna NULL
		RRNatual++;//incrementa o RRN atual
		if(RRNatual < RRN) free(reg);//se não for o registro desejado, desaloca o registro
	}
	if(feof(fp)){//se percorreu o arquivo inteiro e não achou o o registro desejado retorna NULL
		return NULL;
	}
	else return reg;//retorna o registro encontrado
}

