/**
	CSV
	Funções que manipulam arquivos CSV para leitura e escrita.
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "caractere.h"

/**
	leCSV
	Le um registro completo de um arquivo, organizado no padrao CSV.

	PARAMETRO -arquivo- | ponteiro do arquivo CSV
	RETORNA | vetor de strings, cada uma com o valor de um campo do registro
**/
char **leCSV(FILE *arquivo){
	char **dados;
	int tamanho;
	char c, acento;
	int i;

	dados = (char **) calloc(8,sizeof(char *));
	for (i = 0; i < 8; i++){ // leia os 8 campos de um registro
		tamanho = 0;
		do {
			c = fgetc(arquivo);
			acento = acentoWin1252_UTF8(c);

			if (acento == 0){
				dados[i] = (char *) realloc(dados[i],sizeof(char)*(tamanho+1));
				dados[i][tamanho++] = c;	
			} else {
				dados[i] = (char *) realloc(dados[i],sizeof(char)*(tamanho+2));
				dados[i][tamanho++] = INDICA_ACENTO;
				dados[i][tamanho++] = acento;
			}			
		} while (c != ';' && c != '\n' && c != EOF);
		dados[i][tamanho-1] = '\0';
	}

	return dados;
}

void liberaCSV(char **dados){
	int i;
	for (i = 0; i < 8; i++) free(dados[i]);
	free(dados);
}
