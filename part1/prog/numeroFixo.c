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

   /*
	* ler até o fim do registro para determinar seu tamanho
	* allocar um vetor do tamanho encontrado
	* dar fseek() para o inicio do registrador 
	* fread() do fp para o vetor allocado
	* dar fseek() para o final do registrador
	* retornar o char* 
	* obs: a funcao que chama busca() deve, na primeira chamada, mandar a posicao de fp no inicio do FILE* (rewind(fp))
	*/

char* buscaRegistro_NumeroFixo(FILE *fp){
	char *reg = NULL; //registro lido
	int tamanho, i, tamanhoFixo, tamanhoVariado;

	char c = fgetc(fp);
   	if(feof(fp)) return reg;
  	fseek(fp, -1, SEEK_CUR);

	tamanhoFixo = documentoSize + dataHoraCadastroSize + dataHoraAtualizacaoSize + ticketSize;
	tamanhoVariado = 0;
	fseek(fp, tamanhoFixo, SEEK_CUR);
	for(i=0; i<nVariavel; i++){
		//Le o tamanho do proximo campo e armazena o valor em tamanho;
		fread(&tamanho, sizeof(int), 1, fp);
		fseek(fp, tamanho, SEEK_CUR);
		tamanhoVariado += sizeof(int)+tamanho;
	}

	reg = (char*)malloc(sizeof(char)*(tamanhoFixo+tamanhoVariado)); //+1 por causa do '\0'
	fseek(fp, -(tamanhoFixo+tamanhoVariado), SEEK_CUR); //volta ao inicio do registro
	fread(reg, sizeof(char), tamanhoFixo+tamanhoVariado, fp);

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
