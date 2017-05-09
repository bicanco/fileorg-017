/**
	interface
	Biblioteca que gerencia a interface textual do programa.
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "CSV.h"
#include "registro.h"
#include "indicador.h"
#include "delimitador.h"
#include "numeroFixo.h"
#include "caractere.h"

/**
	CONSTANTES: Menu_Metodo
	Indica as opções de configuração para a organização dos
	registros em um arquivo. METODO_VAZIO indica que nenhuma
	opção foi escolhida e METODO_QUANT representa a quantidade
	de opções.
**/
typedef enum {
	METODO_VAZIO = -1,
	METODO_INDICADOR_TAMANHO = 0,
	METODO_DELIMITADOR,
	METODO_NUMERO_FIXO,
	METODO_QUANT
} Menu_Metodo;

/**
	CONSTANTES: Menu_Funcao
	Indica as opções de funções do menu principal do
	programa. FUNCAO_VAZIO indica que nenhuma função foi
	escolhida e FUNCAO_QUANT representa a quantidade
	de opções.
**/
typedef enum {
	FUNCAO_VAZIO = -1,
	FUNCAO_MOSTRAR_TODOS = 0,
	FUNCAO_BUSCA_CAMPO,
	FUNCAO_BUSCA_RRN,
	FUNCAO_CAMPO_RRN,
	FUNCAO_SAIR,
	FUNCAO_QUANT
} Menu_Funcao;

/**
	CONSTANTES: Menu_CampoImprimir
	Indica os campos que podem ser escolhidos para
	serem impressos na função da busca de campos
	por identificação numérica (RRN). CAMPOI_VAZIO indica 
	que nenhum campo foi escolhido e CAMPOI_QUANT representa
	a quantidade de campos a serem escolhidos.
**/
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


/**
	CONSTANTES: Menu_CampoBuscar
	Indica os campos que podem ser escolhidos como chaves
	de busca na função de busca de registro por chave de
	busca. CAMPOB_VAZIO indica que nenhum campo foi escolhido
	e CAMPOB_QUANT representa a quantidade de campos 
	a serem escolhidos.
**/
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

/**
	CONSTANTES: SAIDA_INDICADORTAMANHO
	Nome do arquivo binário gerado pela organização
	de registros por indicadores de tamanho.
**/
#define SAIDA_INDICADORTAMANHO 	"dados_indicadortamanho.dat"

/**
	CONSTANTES: SAIDA_DELIMITADOR
	Nome do arquivo binário gerado pela organização
	de registros por delimitadores.
**/
#define SAIDA_DELIMITADOR		"dados_delimitador.dat"

/**
	CONSTANTES: SAIDA_NUMEROFIXO
	Nome do arquivo binário gerado pela organização
	de registros por número fixo de campos.
**/
#define SAIDA_NUMEROFIXO		"dados_numerofixo.dat"

int main (int argc, char *argv[]){
	int metodoRegistro = METODO_VAZIO; // guarda o metodo escolhido para organizar os registros
	int funcaoMenu = FUNCAO_VAZIO; // guarda a funcao escolhida no menu principal
	int campoImprimir; // guarda o campo a ser impresso na funcao de busca de campo por RRN
	int campoBuscar; // guarda o campo a ser usado como chave de busca
	int buscaConcluida; // indicador de que a busca retornou resultados
	char *stringBusca; // guarda a chave de busca digitada pelo usuario
	char *nomeArquivoEntrada = NULL; // guarda o nome do arquivo de entrada dos dados
	FILE *arquivoEntrada; // representa o arquivo de entrada dos dados
	FILE *arquivoSaida; // representa o arquivo binário de saída
	char **dadosEntrada; // guarda os dados de entrada lidos pelo programa
	char *registroAux; // ponteiro auxiliar para guardar temporariamente registros
	int RRNAux; // variavel auxiliar para iterar a busca por RRN

	// FUNÇÕES GENÉRICAS DE MANIPULAÇÃO DE REGISTRO
	void (*insereRegistro)(char **, FILE *); // inserir um registro no arquivo binário
	char *(*buscaRegistro)(FILE *); // buscar o próximo registro do arquivo binário
	char *(*buscaRRN)(FILE *, int); // buscar um registro a partir do seu RRN

	/******** CONFIGURAÇÕES DO PROGRAMA **********/

	// escolhendo o metodo para separar os registros
	while (metodoRegistro == METODO_VAZIO){
		printf("\nCONFIGURACAO: metodo de separacao dos registros\n");
		printf("%d. Indicadores de tamanho\n", METODO_INDICADOR_TAMANHO);
		printf("%d. Delimitadores entre registros\n", METODO_DELIMITADOR);
		printf("%d. Numero fixo de registros\n", METODO_NUMERO_FIXO);
		printf(">> Escolha um metodo: ");
		
		// caso não tenha sido escrito um número, peça outra entrada
		if (scanf("%d", &metodoRegistro) == 0){
			limpaEntrada(); // limpando o retentor do teclado
			printf("\nEntrada invalida. Digite novamente.\n");
			continue;
		}

 		// caso não seja uma opção válida no menu, peça outra entrada
		if (metodoRegistro <= METODO_VAZIO || metodoRegistro >= METODO_QUANT){
			printf("\nMetodo invalido. Escolha outro.\n");
			metodoRegistro = METODO_VAZIO;
			continue;
		}
	}

	fgetc(stdin); // limpando o retentor do teclado

	// encontrando o arquivo de entrada
	while (nomeArquivoEntrada == NULL){
		printf("\nCONFIGURACAO: arquivo de entrada de dados\n");
		printf(">> Digite o nome do arquivo de entrada: ");
		nomeArquivoEntrada = leString();

		// tentando abrir o arquivo indicado pelo usuário
		arquivoEntrada = fopen(nomeArquivoEntrada, "r");

		// caso tenha erros na abertura do arquivo, peça outra entrada
		if (arquivoEntrada == NULL){
			printf("\nErro na abertura do arquivo. Verifique se o nome dele esta correto.\n");
			if (nomeArquivoEntrada != NULL) free(nomeArquivoEntrada);
			nomeArquivoEntrada = NULL;
			continue;
		}
	}

	// criando e inicializando variaveis para o programa,
	// de acordo com o metodo escolhido
	switch (metodoRegistro){ 
		case METODO_INDICADOR_TAMANHO: // para indicadores de tamanho
			arquivoSaida = fopen(SAIDA_INDICADORTAMANHO, "w+");
			insereRegistro = (void (*)(char **, FILE *)) &insereRegistro_Indicador;
			buscaRegistro = (char *(*)(FILE *)) &buscaRegistro_Indicador;
			buscaRRN = (char *(*)(FILE *, int)) &buscaRRN_Indicador;
			break;

		case METODO_DELIMITADOR: // para delimitadores
			arquivoSaida = fopen(SAIDA_DELIMITADOR, "w+");
			insereRegistro = (void (*)(char **, FILE *)) &insereRegistro_Delimitador;
			buscaRegistro = (char *(*)(FILE *)) &buscaRegistro_Delimitador;
			buscaRRN = (char *(*)(FILE *, int)) &buscaRRN_Delimitador;
			break;

		case METODO_NUMERO_FIXO: // para número fixo de campos
			arquivoSaida = fopen(SAIDA_NUMEROFIXO, "w+");
			insereRegistro = (void (*)(char **, FILE *)) &insereRegistro_NumeroFixo;
			buscaRegistro = (char *(*)(FILE *)) &buscaRegistro_NumeroFixo;
			buscaRRN = (char *(*)(FILE *, int)) &buscaRRN_NumeroFixo;
			break;
	}

	// preparando o arquivo de saida
	while (!feof(arquivoEntrada)){ // enquanto ainda houver dados
		dadosEntrada = leCSV(arquivoEntrada); // leia um registro do arquivo de entrada
		
		// insere no arquivo de saída e libera a memória alocada para ele
		insereRegistro(dadosEntrada, arquivoSaida);
		liberaCSV(dadosEntrada);

		// caso tenha chegado ao fim do arquivo, pare de ler
		if (feof(arquivoEntrada)) break;
	}

	/*********** MENU PRINCIPAL E FUNÇÕES DE MANIPULAÇÃO ***********/

	while (funcaoMenu != FUNCAO_SAIR){ // enquanto a opção de sair não for escolhida

		// escolhendo a função do programa a ser executada
		while (funcaoMenu == FUNCAO_VAZIO){
			printf("\n**BANCO DE DADOS DE DOMINIOS GOVERNAMENTAIS DE INTERNET**\n");
			printf("%d. Mostrar todos os registros\n", FUNCAO_MOSTRAR_TODOS);
			printf("%d. Buscar um registro por um campo\n", FUNCAO_BUSCA_CAMPO);
			printf("%d. Buscar um registro por identificacao numerica\n", FUNCAO_BUSCA_RRN);
			printf("%d. Buscar um campo por identificacao numerica\n", FUNCAO_CAMPO_RRN);
			printf("%d. Sair do programa\n", FUNCAO_SAIR);
			printf(">> Escolha uma funcao para ser executada: ");
			
			// caso não tenha sido escrito um número, peça outra entrada
			if (scanf("%d", &funcaoMenu) == 0){
				limpaEntrada();
				printf("\nEntrada invalida. Digite novamente.\n");
				continue;
			}

			// caso não seja uma opção válida no menu, peça outra entrada
			if (funcaoMenu <= FUNCAO_VAZIO || funcaoMenu >= FUNCAO_QUANT){
				printf("\nFunção invalida. Escolha outro.\n");
				funcaoMenu = FUNCAO_VAZIO;
				continue;
			}
		}

		limpaEntrada(); // limpando o retentor do teclado

 		// escolha a rotina a ser executada de acordo com a função
		switch(funcaoMenu){
			case FUNCAO_MOSTRAR_TODOS:
				// Função: MOSTRAR TODOS OS REGISTROS

				// levar o arquivo de saída para o começo
				fseek(arquivoSaida, 0, SEEK_SET);
				printf("\n\n");

				// enquanto ainda houver dados no arquivo de saída
				while(!feof(arquivoSaida)){
					// pegue um registro do arquivo de saída
					registroAux = buscaRegistro(arquivoSaida);

					if (registroAux != NULL){ // caso ele tenha dados válidos
						// imprime o registro na saída padrão
						imprimeRegistro(registroAux);

						// libera a memória alocada para armazená-lo
						free(registroAux);

						// caso ainda tenha registros a serem impressos,
						// espere uma ação do usuário para mostrar o próximo
						if (!feof(arquivoSaida)){
							printf("\n\nAperte ENTER para mostrar o proximo registro...\n");
							limpaEntrada();
						}
					}
				}
			break;

			case FUNCAO_BUSCA_CAMPO:
				// Função: BUSCAR UM REGISTRO POR UM CAMPO

				// inicialização de variáveis auxiliares
				campoBuscar = CAMPOB_VAZIO;
				stringBusca = NULL;
				buscaConcluida = 0;

				printf("\n\n");

				// escolhendo o campo a ser usado como chave de busca
				while (campoBuscar == CAMPOB_VAZIO){
					printf("%d. Numero do documento\n", CAMPOB_DOCUMENTO);
					printf("%d. Ticket de cadastro\n", CAMPOB_TICKET);
					printf("%d. Dominio\n", CAMPOB_DOMINIO);
					printf("%d. Nome do(a) orgao/entidade\n", CAMPOB_NOME);
					printf("%d. Cidade\n", CAMPOB_CIDADE);
					printf("%d. UF\n", CAMPOB_UF);
					printf(">> Escolha que campo vai ser usado como critério de busca: ");
					
					// caso não tenha sido escrito um número, peça outra entrada
					if (scanf("%d", &campoBuscar) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						continue;
					}

					// caso não seja uma opção válida no menu, peça outra entrada
					if (campoBuscar <= CAMPOB_VAZIO || campoBuscar >= CAMPOB_QUANT){
						printf("\nMetodo invalido. Escolha outro.\n");
						campoBuscar = CAMPOB_VAZIO;
						continue;
					}
				}

				// atualizando valor da posição do campo no registro,
				// de acordo com a opção escolhida no menu
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

				limpaEntrada(); // limpando o retentor do teclado
				printf("\n\n");

				// lendo a chave de busca digitada pelo usuário
				while (stringBusca == NULL){
					printf(">> Digite o que deve ser buscado: ");
					stringBusca = leString();

					// caso a chave digitada esteja vazia, peça outra entrada
					if (stringBusca == NULL || stringBusca[0] == '\0'){
						printf("\nEntrada invalida. Digite novamente.\n");
						if (stringBusca != NULL){
							free(stringBusca);
							stringBusca = NULL;
						}
					}
				}

				// levar o arquivo de saída para o começo
				fseek(arquivoSaida, 0, SEEK_SET);

				// enquanto ainda houver dados no arquivo de saída
				while(!feof(arquivoSaida)){
					// pegue um registro do arquivo de saída
					registroAux = buscaRegistro(arquivoSaida);

					if (registroAux != NULL){ // caso ele tenha dados válidos

						// cheque se esse registro bate com a chave de busca
						if (comparaCampo(registroAux, campoBuscar, stringBusca) == 0){
							// imprima o registro
							imprimeRegistro(registroAux);

							// indique que a busca teve resultados
							buscaConcluida = 1;

							// caso ainda tenha registros a serem buscados,
							// espere uma ação do usuário para mostrar o próximo
							if (!feof(arquivoSaida)){
								printf("\n\nAperte ENTER para mostrar o proximo resultado da busca...\n");
								limpaEntrada();
							}
						}

						// libera a memória alocada para armazenar o registro
						free(registroAux);

						// pare de ler caso o arquivo de saída esteja no final
						if (feof(arquivoSaida)) break;
					}
				}

				// caso a busca nao tenha tido resultados, mostre uma mensagem
				if (!buscaConcluida)
					printf("\n\nNenhum registro foi encontrado.\n");

				// libere a memoria alocada para guardar a chave de busca
				free(stringBusca);
			break;

			case FUNCAO_BUSCA_RRN:
				// Função: BUSCAR UM REGISTRO PELO SEU RRN

				// inicialização de variáveis auxiliares
				RRNAux = -1;

				// escolhendo o RRN do registro a ser buscado
				while (RRNAux == -1){
					printf(">> Digite a identificacao numerica do registro: ");

					// caso não tenha sido escrito um número, peça outra entrada
					if (scanf("%d", &RRNAux) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						continue;
					}

					// caso não seja um número positivo, peça outra entrada
					if (RRNAux < 0){
						printf("\nA identificacao numerica deve ser positiva. Digite novamente.\n");
						RRNAux = -1;
						continue;
					}
				}

				printf("\n\n");

				// busca o registro pelo RRN
				registroAux = buscaRRN(arquivoSaida, RRNAux);

				if (registroAux != NULL){ // caso tenha encontrado o registro
					// imprima o registro na tela
					imprimeRegistro(registroAux);

					// libera a memória alocada para o registro
					free(registroAux);
				} else { // caso não tenha encontrado
					// imprima uma mensagem de erro
					printf("Registro nao encontrado.\n");
				}
			break;
			
			case FUNCAO_CAMPO_RRN:
				// Função: BUSCAR UM CAMPO PELO SEU RRN

				// inicialização de variáveis auxiliares
				RRNAux = -1;
				campoImprimir = CAMPOI_VAZIO;

				// escolhendo o RRN do registro a ser buscado
				while (RRNAux == -1){
					printf(">> Digite a identificacao numerica do registro: ");

					// caso não tenha sido escrito um número, peça outra entrada
					if (scanf("%d", &RRNAux) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						continue;
					}

					// caso não seja um número positivo, peça outra entrada
					if (RRNAux < 0){
						printf("\nA identificacao numerica deve ser positiva. Digite novamente.\n");
						RRNAux = -1;
						continue;
					}
				}

				printf("\n\n");

				// escolhendo o campo a ser impresso depois da busca por RRN
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
					
					// caso não tenha sido escrito um número, peça outra entrada
					if (scanf("%d", &campoImprimir) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						continue;
					}

					// caso não seja uma opção válida no menu, peça outra entrada
					if (campoImprimir <= CAMPOI_VAZIO || campoImprimir >= CAMPOI_QUANT){
						printf("\nMetodo invalido. Escolha outro.\n");
						campoImprimir = CAMPOI_VAZIO;
						continue;
					}
				}

				// busca o registro pelo RRN
				registroAux = buscaRRN(arquivoSaida, RRNAux);

				if (registroAux != NULL){ // caso tenha encontrado o registro
					// imprime o campo na tela
					imprimeCampo(registroAux, campoImprimir);

					// libera a memória alocada para o registro
					free(registroAux);
				} else { // caso não tenha encontrado
					// imprima uma mensagem de erro
					printf("Registro nao encontrado.\n");
				}

				printf("\n\n");
			break;
		}

		// caso a função escolhida nao tenha sido a de sair,
		// zere a escolha para que outra função possa ser escolhida
		if (funcaoMenu != FUNCAO_SAIR) funcaoMenu = FUNCAO_VAZIO;
	}

	// liberação de memória alocada
	free(nomeArquivoEntrada);
	fclose(arquivoEntrada);
	fclose(arquivoSaida);
}