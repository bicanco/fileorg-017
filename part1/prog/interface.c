#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "CSV.h"
#include "registro.h"
#include "indicador.h"
#include "delimitador.h"
#include "caractere.h"

typedef enum {
	METODO_VAZIO = -1,
	METODO_INDICADOR_TAMANHO = 0,
	METODO_DELIMITADOR,
	METODO_NUMERO_FIXO,
	METODO_QUANT
} Menu_Metodo;

typedef enum {
	FUNCAO_VAZIO = -1,
	FUNCAO_MOSTRAR_TODOS = 0,
	FUNCAO_BUSCA_CAMPO,
	FUNCAO_BUSCA_RRN,
	FUNCAO_CAMPO_RRN,
	FUNCAO_SAIR,
	FUNCAO_QUANT,
} Menu_Funcao;

#define SAIDA_INDICADORTAMANHO 	"dados_indicadortamanho.dat"
#define SAIDA_DELIMITADOR		"dados_delimitador.dat"
#define SAIDA_NUMEROFIXO		"dados_numerofixo.dat"

int main (int argc, char *argv[]){
	int metodoRegistro = METODO_VAZIO;
	int funcaoMenu = FUNCAO_VAZIO;
	char *nomeArquivoEntrada = NULL;
	FILE *arquivoEntrada, *arquivoSaida;
	char **dadosEntrada, *registroAux;
	int RRNAux;

	void (*insereRegistro)(char **, FILE *);
	char *(*buscaRegistro)(FILE *);
	char *(*buscaRRN)(FILE *, int);

	char *debug, *debug2;
	int debugt;

	// escolhendo o metodo para separar os registros
	while (metodoRegistro == METODO_VAZIO){
		printf("\nCONFIGURAÇÃO: método de separação dos registros\n");
		printf("%d. Indicadores de tamanho\n", METODO_INDICADOR_TAMANHO);
		printf("%d. Delimitadores entre registros\n", METODO_DELIMITADOR);
		printf("%d. Número fixo de registros\n", METODO_NUMERO_FIXO);
		printf(">> Escolha um método: ");
		
		if (scanf("%d", &metodoRegistro) == 0){
			limpaEntrada();
			printf("\nEntrada inválida. Digite novamente.\n");
			continue;
		}

		if (metodoRegistro <= METODO_VAZIO || metodoRegistro >= METODO_QUANT){
			printf("\nMétodo inválido. Escolha outro.\n");
			metodoRegistro = METODO_VAZIO;
			continue;
		}
	}

	fgetc(stdin); // limpando o buffer do teclado

	// encontrando o arquivo de entrada
	while (nomeArquivoEntrada == NULL){
		printf("\nCONFIGURAÇÃO: arquivo de entrada de dados\n");
		printf(">> Digite o nome do arquivo de entrada: ");
		nomeArquivoEntrada = leString();

		arquivoEntrada = fopen(nomeArquivoEntrada, "r");
		if (arquivoEntrada == NULL){
			printf("\nErro na abertura do arquivo. Verifique se o nome dele está correto.\n");
			free(nomeArquivoEntrada);
			nomeArquivoEntrada = NULL;
			continue;
		}
	}

	// criando e inicializando variaveis para o programa,
	// de acordo com o metodo escolhido
	switch (metodoRegistro){ 
		case METODO_INDICADOR_TAMANHO:
			arquivoSaida = fopen(SAIDA_INDICADORTAMANHO, "w+");
			insereRegistro = (void (*)(char **, FILE *)) &insereRegistro_Indicador;
			buscaRegistro = (char *(*)(FILE *)) &buscaRegistro_Indicador;
			buscaRRN = (char *(*)(FILE *, int)) &buscaRRN_Indicador;
			break;

		case METODO_DELIMITADOR:
			arquivoSaida = fopen(SAIDA_DELIMITADOR, "w+");
			insereRegistro = (void (*)(char **, FILE *)) &insereRegistro_Delimitador;
			buscaRegistro = (char *(*)(FILE *)) &buscaRegistro_Delimitador;
			buscaRRN = (char *(*)(FILE *, int)) &buscaRRN_Delimitador;
			break;

		case METODO_NUMERO_FIXO:
			arquivoSaida = fopen(SAIDA_NUMEROFIXO, "w+");
			break;
	}

	// preparando o arquivo de saida
	while (!feof(arquivoEntrada)){ // enquanto ainda houver dados
		dadosEntrada = leCSV(arquivoEntrada);
		if (!feof(arquivoEntrada)){
			insereRegistro(dadosEntrada, arquivoSaida);
		}

		liberaCSV(dadosEntrada);
	}

	while (funcaoMenu != FUNCAO_SAIR){
		// escolhendo o metodo para separar os registros
		while (funcaoMenu == FUNCAO_VAZIO){
			printf("\n**BANCO DE DADOS DE DOMÍNIOS GOVERNAMENTAIS DE INTERNET**\n");
			printf("%d. Mostrar todos os registros\n", FUNCAO_MOSTRAR_TODOS);
			printf("%d. Buscar um registro por um campo\n", FUNCAO_BUSCA_CAMPO);
			printf("%d. Buscar um registro por identificação numérica\n", FUNCAO_BUSCA_RRN);
			printf("%d. Buscar um campo por identificação numérica\n", FUNCAO_CAMPO_RRN);
			printf("%d. Sair do programa\n", FUNCAO_SAIR);
			printf(">> Escolha uma função para ser executada: ");
			
			if (scanf("%d", &funcaoMenu) == 0){
				limpaEntrada();
				printf("\nEntrada inválida. Digite novamente.\n");
				continue;
			}

			if (funcaoMenu <= FUNCAO_VAZIO || funcaoMenu >= FUNCAO_QUANT){
				printf("\nMétodo inválido. Escolha outro.\n");
				funcaoMenu = FUNCAO_VAZIO;
				continue;
			}
		}

		limpaEntrada();

		switch(funcaoMenu){
			case FUNCAO_MOSTRAR_TODOS:
				fseek(arquivoSaida, 0, SEEK_SET);
				printf("\n\n");

				while(!feof(arquivoSaida)){
					registroAux = buscaRegistro(arquivoSaida);
					if (registroAux != NULL){
						imprimeRegistro(registroAux);
						free(registroAux);

						printf("\n\nAperte ENTER para mostrar o próximo registro...\n");
						limpaEntrada();
					}
				}
			break;

			case FUNCAO_BUSCA_CAMPO:
			printf("SOON");
			break;

			case FUNCAO_BUSCA_RRN:
				RRNAux = -1;

				while (RRNAux == -1){
					printf(">> Digite a identificação númerica do registro: ");

					if (scanf("%d", &RRNAux) == 0){
						limpaEntrada();
						printf("\nEntrada inválida. Digite novamente.\n");
						continue;
					}
				}

				printf("\n\n");
				registroAux = buscaRRN(arquivoSaida, RRNAux);
				if (registroAux != NULL){
					imprimeRegistro(registroAux);
					free(registroAux);
				} else {
					printf("Registro não encontrado.\n");
				}

			break;
			
			case FUNCAO_CAMPO_RRN:
			printf("SOON");
			break;
		}

		if (funcaoMenu != FUNCAO_SAIR) funcaoMenu = FUNCAO_VAZIO;
	}

	free(nomeArquivoEntrada);
	fclose(arquivoEntrada);
	fclose(arquivoSaida);
}