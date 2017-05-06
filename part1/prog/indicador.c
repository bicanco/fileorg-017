#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "registro.h"
#include "CSV.h"

/**
	CriaRegistro Com Indicador
	Armazena no arquivo um registro de tamanho variavel com indicador de tamanho,
	de acordo com dados recebidos do arquivo CSV e a documentação do trabalho, 
	quanto ao tamanho, à ordem e a forma com que cada campo é armazenado.

	PARAMETRO -csv- | campos lidos do arquivo CSV
  PARAMETRO -fds- | endereco apontado pelo ponteiro do arquivo final
 RETORNA |


**/

void criaRegistro_com_indicador(char** csv, FILE *fds){
	int tamanho;
	//lendo um registro com campos de tamanhos fixos e campos tamanhos variaveis respectivamente
	char *registro = criaRegistro(csv, &tamanho);
	fwrite(&tamanho, sizeof(int), 1, fds);
	fwrite(registro, sizeof(char), tamanho, fds);

	free(registro);
}
