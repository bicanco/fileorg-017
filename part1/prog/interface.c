#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#include "CSV.h"
#include "registro.h"
#include "indicador.h"
#include "delimitador.h"
#include "numeroFixo.h"
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
	FUNCAO_QUANT
} Menu_Funcao;

typedef enum {
	CAMPOI_VAZIO = -1,
	CAMPOI_DOCUMENTO,
	CAMPOI_DATA_CADASTRO,
	CAMPOI_DATA_ATUALIZA,
	CAMPOI_TICKET,
	CAMPOI_DOMINIO,
	CAMPOI_NOME,
	CAMPOI_CIDADE,
	CAMPOI_UF,
	CAMPOI_QUANT
} Menu_CampoImprimir;

typedef enum {
	CAMPOB_VAZIO = -1,
	CAMPOB_DOCUMENTO,
	CAMPOB_TICKET,
	CAMPOB_DOMINIO,
	CAMPOB_NOME,
	CAMPOB_CIDADE,
	CAMPOB_UF,
	CAMPOB_QUANT
} Menu_CampoBuscar;

#define SAIDA_INDICADORTAMANHO 	"dados_indicadortamanho.dat"
#define SAIDA_DELIMITADOR		"dados_delimitador.dat"
#define SAIDA_NUMEROFIXO		"dados_numerofixo.dat"

int main (int argc, char *argv[]){
	setlocale(LC_ALL,"utf-8");

	int metodoRegistro = METODO_VAZIO;
	int funcaoMenu = FUNCAO_VAZIO;
	int campoImprimir, campoBuscar, buscaConcluida;
	char *stringBusca;
	char *nomeArquivoEntrada = NULL;
	FILE *arquivoEntrada, *arquivoSaida;
	char **dadosEntrada, *registroAux;
	int RRNAux;

	void (*insereRegistro)(char **, FILE *);
	char *(*buscaRegistro)(FILE *);
	char *(*buscaRRN)(FILE *, int);

	char *debug, *debug2;
	int debugt, debugi;

	// escolhendo o metodo para separar os registros
	while (metodoRegistro == METODO_VAZIO){
		printf("\nCONFIGURACAO: metodo de separacao dos registros\n");
		printf("%d. Indicadores de tamanho\n", METODO_INDICADOR_TAMANHO);
		printf("%d. Delimitadores entre registros\n", METODO_DELIMITADOR);
		printf("%d. Numero fixo de registros\n", METODO_NUMERO_FIXO);
		printf(">> Escolha um metodo: ");
		
		if (scanf("%d", &metodoRegistro) == 0){
			limpaEntrada();
			printf("\nEntrada invalida. Digite novamente.\n");
			continue;
		}

		if (metodoRegistro <= METODO_VAZIO || metodoRegistro >= METODO_QUANT){
			printf("\nMetodo invalido. Escolha outro.\n");
			metodoRegistro = METODO_VAZIO;
			continue;
		}
	}

	fgetc(stdin); // limpando o buffer do teclado

	// encontrando o arquivo de entrada
	while (nomeArquivoEntrada == NULL){
		printf("\nCONFIGURACAO: arquivo de entrada de dados\n");
		printf(">> Digite o nome do arquivo de entrada: ");
		nomeArquivoEntrada = leString();

		arquivoEntrada = fopen(nomeArquivoEntrada, "r");
		if (arquivoEntrada == NULL){
			printf("\nErro na abertura do arquivo. Verifique se o nome dele esta correto.\n");
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
			insereRegistro = (void (*)(char **, FILE *)) &insereRegistro_NumeroFixo;
			buscaRegistro = (char *(*)(FILE *)) &buscaRegistro_NumeroFixo;
			buscaRRN = (char *(*)(FILE *, int)) &buscaRRN_NumeroFixo;
			break;
	}

	// preparando o arquivo de saida
	while (!feof(arquivoEntrada)){ // enquanto ainda houver dados
		dadosEntrada = leCSV(arquivoEntrada);
		if (!feof(arquivoEntrada)){
			insereRegistro(dadosEntrada, arquivoSaida);
			liberaCSV(dadosEntrada);
		} else {
			insereRegistro(dadosEntrada, arquivoSaida);
			liberaCSV(dadosEntrada);
			break;
		}	
	}

	while (funcaoMenu != FUNCAO_SAIR){
		// escolhendo o metodo para separar os registros
		while (funcaoMenu == FUNCAO_VAZIO){
			printf("\n**BANCO DE DADOS DE DOMINIOS GOVERNAMENTAIS DE INTERNET**\n");
			printf("%d. Mostrar todos os registros\n", FUNCAO_MOSTRAR_TODOS);
			printf("%d. Buscar um registro por um campo\n", FUNCAO_BUSCA_CAMPO);
			printf("%d. Buscar um registro por identificacao numerica\n", FUNCAO_BUSCA_RRN);
			printf("%d. Buscar um campo por identificacao numerica\n", FUNCAO_CAMPO_RRN);
			printf("%d. Sair do programa\n", FUNCAO_SAIR);
			printf(">> Escolha uma funcao para ser executada: ");
			
			if (scanf("%d", &funcaoMenu) == 0){
				limpaEntrada();
				printf("\nEntrada invalida. Digite novamente.\n");
				continue;
			}

			if (funcaoMenu <= FUNCAO_VAZIO || funcaoMenu >= FUNCAO_QUANT){
				printf("\nMetodo invalido. Escolha outro.\n");
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

						if (!feof(arquivoSaida)){
							printf("\n\nAperte ENTER para mostrar o proximo registro...\n");
							limpaEntrada();
						}
						
					}
				}
			break;

			case FUNCAO_BUSCA_CAMPO:
				campoBuscar = CAMPOB_VAZIO;
				stringBusca = NULL;
				buscaConcluida = 0;

				printf("\n\n");
				while (campoBuscar == CAMPOB_VAZIO){
					printf("%d. Numero do documento\n", CAMPOB_DOCUMENTO);
					printf("%d. Ticket de cadastro\n", CAMPOB_TICKET);
					printf("%d. Dominio\n", CAMPOB_DOMINIO);
					printf("%d. Nome do(a) orgao/entidade\n", CAMPOB_NOME);
					printf("%d. Cidade\n", CAMPOB_CIDADE);
					printf("%d. UF\n", CAMPOB_UF);
					printf(">> Escolha que campo vai ser usado como critério de busca: ");
					
					if (scanf("%d", &campoBuscar) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						continue;
					}

					if (campoBuscar <= CAMPOB_VAZIO || campoBuscar >= CAMPOB_QUANT){
						printf("\nMetodo invalido. Escolha outro.\n");
						campoBuscar = CAMPOB_VAZIO;
						continue;
					}
				}

				// arrumando campo
				switch (campoBuscar){
					case CAMPOB_TICKET:
						campoBuscar = CAMPOI_TICKET; break;

					case CAMPOB_DOMINIO:
						campoBuscar = CAMPOI_DOMINIO; break;

					case CAMPOB_NOME:
						campoBuscar = CAMPOI_NOME; break;

					case CAMPOB_CIDADE:
						campoBuscar = CAMPOI_CIDADE; break;

					case CAMPOB_UF:
						campoBuscar = CAMPOI_UF; break;
				}

				limpaEntrada();
				printf("\n\n");
				while (stringBusca == NULL){
					printf(">> Digite o que deve ser buscado: ");
					stringBusca = leString();

					if (stringBusca == NULL || stringBusca[0] == '\0'){
						printf("\nEntrada invalida. Digite novamente.\n");
						if (stringBusca != NULL){
							free(stringBusca);
							stringBusca = NULL;
						}
					}
				}

				fseek(arquivoSaida, 0, SEEK_SET);
				while(!feof(arquivoSaida)){
					registroAux = buscaRegistro(arquivoSaida);
					if (registroAux != NULL){
						if (comparaCampo(registroAux, campoBuscar, stringBusca) == 0){
							// achou um campo valido
							imprimeRegistro(registroAux);
							buscaConcluida = 1;

							if (!feof(arquivoSaida)){
								printf("\n\nAperte ENTER para mostrar o proximo resultado da busca...\n");
								limpaEntrada();
							}
						}

						free(registroAux);
						if (feof(arquivoSaida)) break;
					}
				}

				if (!buscaConcluida)
					printf("\n\nNenhum registro foi encontrado.\n");
			break;

			case FUNCAO_BUSCA_RRN:
				RRNAux = -1;

				while (RRNAux == -1){
					printf(">> Digite a identificacao numerica do registro: ");

					if (scanf("%d", &RRNAux) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						RRNAux = -1;
						continue;
					}

					if (RRNAux < 0){
						printf("\nA identificacao numerica deve ser positiva. Digite novamente.\n");
						RRNAux = -1;
						continue;
					}
				}

				printf("\n\n");
				registroAux = buscaRRN(arquivoSaida, RRNAux);
				if (registroAux != NULL){
					imprimeRegistro(registroAux);
					free(registroAux);
				} else {
					printf("Registro nao encontrado.\n");
				}
			break;
			
			case FUNCAO_CAMPO_RRN:
				RRNAux = -1;
				campoImprimir = CAMPOI_VAZIO;

				while (RRNAux == -1){
					printf(">> Digite a identificacao numerica do registro: ");

					if (scanf("%d", &RRNAux) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						RRNAux = -1;
						continue;
					}

					if (RRNAux < 0){
						printf("\nA identificacao numerica deve ser positiva. Digite novamente.\n");
						RRNAux = -1;
						continue;
					}
				}

				printf("\n\n");

				while (campoImprimir == CAMPOI_VAZIO){
					printf("%d. Numero do documento\n", CAMPOI_DOCUMENTO);
					printf("%d. Data e hora do cadastro\n", CAMPOI_DATA_CADASTRO);
					printf("%d. Data e hora da ultima atualizacao\n", CAMPOI_DATA_ATUALIZA);
					printf("%d. Ticket de cadastro\n", CAMPOI_TICKET);
					printf("%d. Dominio\n", CAMPOI_DOMINIO);
					printf("%d. Nome do(a) orgao/entidade\n", CAMPOI_NOME);
					printf("%d. Cidade\n", CAMPOI_CIDADE);
					printf("%d. UF\n", CAMPOI_UF);
					printf(">> Escolha um campo a ser buscado: ");
					
					if (scanf("%d", &campoImprimir) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						continue;
					}

					if (campoImprimir <= CAMPOI_VAZIO || campoImprimir >= CAMPOI_QUANT){
						printf("\nMetodo invalido. Escolha outro.\n");
						campoImprimir = CAMPOI_VAZIO;
						continue;
					}
				}

				printf("\n\n");
				registroAux = buscaRRN(arquivoSaida, RRNAux);
				if (registroAux != NULL){
					imprimeCampo(registroAux, campoImprimir);
					free(registroAux);
				} else {
					printf("Registro nao encontrado.\n");
				}

				printf("\n\n");
			break;
		}

		if (funcaoMenu != FUNCAO_SAIR) funcaoMenu = FUNCAO_VAZIO;
	}

	free(nomeArquivoEntrada);
	fclose(arquivoEntrada);
	fclose(arquivoSaida);
}