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
	char **dados; // armazena os dados de entrada, separados por campo, no formato de strings
	int tamanho; // variavel auxiliar para armazenar o tamanho de cada campo durante a leitura
	char c; // varivavel auxiliar para armazenar temporariamente um caractere lido
	char acento; // variavel que recebe o novo valor do char, caso seja acentuado
	int i; // variavel de interação de laço

	// alocando memória para guardar ponteiros para todos as strings com valores dos campos
	dados = (char **) calloc(NUM_CAMPOS, sizeof(char *));

	for (i = 0; i < NUM_CAMPOS; i++){ // leia os 8 campos de um registro
		tamanho = 0;
		do {
			// pegue um caractere do arquivo
			c = fgetc(arquivo);

			// cheque se é um caractere acentuado e deve ser convertido
			acento = acentoWin1252_UTF8(c);

			// caso nao seja acentuado
			if (acento == 0){
				// apenas coloque-o na string, realocando memoria para tal
				dados[i] = (char *) realloc(dados[i],sizeof(char)*(tamanho+1));
				dados[i][tamanho++] = c;	
			} else {
				// realoque memoria para dois caracteres, incluindo o indicador de caractere acentuado
				dados[i] = (char *) realloc(dados[i],sizeof(char)*(tamanho+2));
				dados[i][tamanho++] = INDICA_ACENTO;
				dados[i][tamanho++] = acento;
			}
		// enquanto o registro nao tenha acabado	
		} while (c != ';' && c != '\n' && c != EOF);

		// coloque caractere de fim de string
		dados[i][tamanho-1] = '\0';
	}

	return dados;
}

/**
	liberaCSV
	Libera a memoria alocada para um conjunto de dados de entrada.

	PARAMETRO -dados- | ponteiro para o conjunto dos dados de entrada
**/
void liberaCSV(char **dados){
	int i; // variavel de interação de laço

	// para cada uma das strings, libere a memoria alocada para ela
	for (i = 0; i < NUM_CAMPOS; i++) free(dados[i]);

	// libere o vetor que guardava os ponteiros das strings
	free(dados);
}
