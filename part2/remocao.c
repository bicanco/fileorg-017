#include<stdlib.h>
#include<stdio.h>
#include"remocao.h"

#define INDICADOR_DE_REMOCAO -2

void remocao(FILE *fp, int rrn, int stack_top){
    	char *registro = NULL;//váriavel que guarda o registro encontrado
	char c;//variável utilizada para verificar a condição de final de registro
	int tamanho = 0; //variável utilizada par guadar o tamanho do registro
	int aux;///variável utilizada para guadar o tamanho dos campos de tamanho variável

	
	fseek(fp, rrn, SEEK_SET);

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
	free(registro);
	tmanho++;
	fseek(fp, rrn, SEEK_SET);

	fwrite(INDICADOR_DE_REMOCAO,sizeof(char),1,fp);
	fwrite(&tamanho,sizeof(int), 1, fp);
	fwrite(&stack_top, sizeof(int), 1, fp);
}
