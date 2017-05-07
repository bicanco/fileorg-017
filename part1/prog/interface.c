#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "CSV.h"
#include "registro.h"
#include "indicador.h"
#include "caractere.h"

typedef enum {
	METODO_INDICADOR_TAMANHO = 1,
	METODO_DELIMITADOR,
	METODO_NUMERO_FIXO,
	METODO_QUANT
} Menu_Metodo;

#define SAIDA_INDICADORTAMANHO 	"dados_indicadortamanho.dat"
#define SAIDA_DELIMITADOR		"dados_delimitador.dat"
#define SAIDA_NUMEROFIXO		"dados_numerofixo.dat"

int main (int argc, char *argv[]){
	int metodoRegistro = -1;
	char *nomeArquivoEntrada = NULL;
	FILE *arquivoEntrada, *arquivoSaida;
	char **dadosEntrada; 

	char *debug, *debug2;
	int debugt;

	// escolhendo o metodo para separar os registros
	while (metodoRegistro == -1){
		printf("\nCONFIGURAÇÃO: método de separação dos registros\n");
		printf("%d. Indicadores de tamanho\n", METODO_INDICADOR_TAMANHO);
		printf("%d. Delimitadores entre registros\n", METODO_DELIMITADOR);
		printf("%d. Número fixo de registros\n", METODO_NUMERO_FIXO);
		printf("Escolha um método: ");
		
		if (scanf("%d", &metodoRegistro) == 0){
			limpaEntradaInt();
			printf("\nEntrada inválida. Digite novamente.\n");
			continue;
		}

		if (metodoRegistro < 0 || metodoRegistro >= METODO_QUANT){
			printf("\nMétodo inválido. Escolha outro.\n");
			metodoRegistro = -1;
			continue;
		}
	}

	fgetc(stdin); // limpando o buffer do teclado

	// encontrando o arquivo de entrada
	while (nomeArquivoEntrada == NULL){
		printf("\nCONFIGURAÇÃO: arquivo de entrada de dados\n");
		printf("Digite o nome do arquivo de entrada: ");
		nomeArquivoEntrada = leString();

		arquivoEntrada = fopen(nomeArquivoEntrada, "r");
		if (arquivoEntrada == NULL){
			printf("\nErro na abertura do arquivo. Verifique se o nome dele está correto.\n");
			free(nomeArquivoEntrada);
			nomeArquivoEntrada = NULL;
			continue;
		}
	}

	// criando o arquivo de saida
	switch (metodoRegistro){ // escolhendo o nome do arquivo de saida
		case 1:	// para indicador de tamanho
			arquivoSaida = fopen(SAIDA_INDICADORTAMANHO, "w+");
			break;

		case 2: // para delimitador
			arquivoSaida = fopen(SAIDA_DELIMITADOR, "w+");
			break;

		case 3: // para numero fixo de campos
			arquivoSaida = fopen(SAIDA_NUMEROFIXO, "w+");
			break;
	}

	// preparando o arquivo de saida
	while (!feof(arquivoEntrada)){ // enquanto ainda houver dados
		dadosEntrada = leCSV(arquivoEntrada);
		if (!feof(arquivoEntrada)){
			switch (metodoRegistro){ // escolhendo a funcao de acordo com o metodo
				case 1:	// para indicador de tamanho
					criaRegistro_com_indicador(dadosEntrada, arquivoSaida);
					break;

				case 2: // para delimitador
					debug = leString();
					debug2 = leString();
					stringMaisculaAcentos(debug);
					stringMaisculaAcentos(debug2);
					printf("*1: %s\n", debug);
					printf("*2: %s\n", debug2);
					printf(">> %d\n", strcmp(debug, debug2));
					break;

				case 3: // para numero fixo de campos
					break;
			}
		}

		liberaCSV(dadosEntrada);
	}
	
	free(nomeArquivoEntrada);
	fclose(arquivoEntrada);
	fclose(arquivoSaida);
}