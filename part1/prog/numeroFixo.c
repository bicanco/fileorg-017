/**
    numeroFixo
    Funções que manipulam um arquivo unico de campos de tamanho
    variaveis com indicador de tamanho, campos de tamanho fixos e
    registros de tamanhos variaveis com número fixo de campos, 
    incluindo a inserção e busca de dados nesse padrão.
**/

#include <stdlib.h>
#include <stdio.h>

#include "numeroFixo.h"
#include "registro.h"

/**
	insereRegistro_NumeroFixo
	Cria e escreve no arquivo de saida um registro de tamanho variavel com numero fixo de campos,
	de acordo com dados recebidos do arquivo CSV e a documentação do trabalho, 
	quanto ao tamanho, à ordem e a forma com que cada campo é armazenado.
	
	PARAMETRO -CSV- | campos lidos do arquivo CSV
    PARAMETRO -fds- | arquivo de saida
**/
void insereRegistro_NumeroFixo(char** csv, FILE *fds){
	//CSV = string de entrada com todos os campos
	//fp  = FILE* ja aberto atenteriormente, referente ao arquivo a ser escrito
	int len; 
	char* registro;

	registro = criaRegistro(csv, &len);
	//Len = numero de bytes contidos no registro
	fwrite(registro, sizeof(char), len, fds);

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
	int tamanhoFixo; // tamanho dos campos de tamanho fixo do registro
	int tamanhoVariado; // tamanho dos campos de tamanho variavel do registro

	c = fgetc(fp);//confere se o arquivo está no fim
    if(feof(fp)) return registro;//retorna NULL se estiver no fim
    fseek(fp, -1, SEEK_CUR);//retorna para o arquivo para a posição inicial

    // calculo do tamanho dos campos fixos
	tamanhoFixo = documentoSize + dataHoraCadastroSize + dataHoraAtualizacaoSize + ticketSize;
	tamanhoVariado = 0; // zerando contador do tamanho de campos de tamanho variavel
	fseek(fp, tamanhoFixo, SEEK_CUR); // pulando os campos de tamanho fixo
	for(i=0; i<nVariavel; i++){ // para cada campo variavel
		//Le o tamanho do proximo campo e armazena o valor em tamanho;
		fread(&tamanho, sizeof(int), 1, fp);
		fseek(fp, tamanho, SEEK_CUR); // percorre esse tamanho no arquivo
		tamanhoVariado += sizeof(int)+tamanho; // guarda o tamanho do campo
	}

	reg = (char*)malloc(sizeof(char)*(tamanhoFixo+tamanhoVariado)); // aloca memoria para o registro todo
	fseek(fp, -(tamanhoFixo+tamanhoVariado), SEEK_CUR); // volta ao inicio do registro
	fread(reg, sizeof(char), tamanhoFixo+tamanhoVariado, fp); // le todo o registro

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
	while(!feof(fp) && RRNatual<RRN){//enquanto não for fim de arquivo e o RRN atual é menor do que o procurado
		reg = buscaRegistro_NumeroFixo(fp);//recupera o próxmo registro do arquivo
		if (reg == NULL) return reg;//se não achar um registro no arquivo retorna NULL
		RRNatual++;//incrementa o RRN atual
		if(RRnatual < RRN) free(reg);//se não for o registro desejado, desaloca o registro
	}
	if(feof(fp)){//se percorreu o arquivo inteiro e não achou o o registro desejado retorna NULL
		return NULL;
	}
	else return reg;//retorna o registro encontrado
}
