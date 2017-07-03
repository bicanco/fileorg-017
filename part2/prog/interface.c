/*==============================================================*\
||  Primeiro Trabalho Prático - Organização de Arquivos(SCC215) ||
||   															||
|| Alunos:  									Nº USP: 	    ||
||   	David Souza Rodrigues							4461180 ||
||   	Fernanda Tostes Marana  						4471070 ||
||   	Gabriel Toschi de Oliveira  					9763039 ||
||   	Marcelo de Moraes Carvalho da Silva 			9791048 ||
||   															||
|| Professora:  												|| 
||   	Dra. Cristina Dutra de Aguiar Ciferri  					||
||   															||
|| Entrega:														|| 
||     	09/05/2017				 								||
||   															||
|| Arquivo: 													||
||   	interface.c												||
||   															||
|| Descrição:													||
||		Biblioteca que gerencia a interface textual do programa.||
\*==============================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "CSV.h"
#include "registro.h"
#include "delimitador.h"
#include "caractere.h"
#include "indice.h"
#include "firstFit.h"
#include "bestFit.h"
#include "worstFit.h"

/**
	CONSTANTES: Menu_Funcao
	Indica as opções de funções do menu principal do
	programa. FUNCAO_VAZIO indica que nenhuma função foi
	escolhida e FUNCAO_QUANT representa a quantidade
	de opções.
**/
typedef enum {
	FUNCAO_VAZIO = -1,
	FUNCAO_REMOVER = 0,
	FUNCAO_INSERIR,
	FUNCAO_IND_ESTAT,
	FUNCAO_IND_SIMUL,
	FUNCAO_ESTAT_REMOV,
	FUNCAO_SAIR,
	FUNCAO_QUANT
} Menu_Funcao;

/**
	CONSTANTES: Menu_Estatisticas
	Indica os campos que podem ser escolhidos para
	mostrar estatisticas sobre os arquivos de dados
	e os indices, de acordo com o algoritmo usado
	para a abordagem dinamica.
**/
typedef enum {
	ESTAT_VAZIO = -1,
	ESTAT_FIRST = 0,
	ESTAT_BEST,
	ESTAT_WORST,
	ESTAT_SAIR,
	ESTAT_QUANT
} Menu_Estatisticas;

#define DADOS_FIRSTFIT "firstfit.dat"
#define DADOS_BESTFIT  "bestfit.dat"
#define DADOS_WORSTFIT "worstfit.dat"

#define INDICE_FIRSTFIT "firstfit.idx"
#define INDICE_BESTFIT "bestfit.idx"
#define INDICE_WORSTFIT "worstfit.idx"

int main (int argc, char *argv[]){
	Indice *indiceFirst, *indiceBest, *indiceWorst; // estruturas de indice para cada um dos tipos
	FILE *dadosFirst, *dadosBest, *dadosWorst; // arquivos de dados para cada um dos tipos
	IndiceItem itemFirst, itemBest, itemWorst; // itens de indice auxiliares para cada um dos tipos

	FILE *indiceFirstArq, *indiceBestArq, *indiceWorstArq; // arquivos de indice para cada um dos tipos
	char *nomeArquivoEntrada = NULL; // guarda o nome do arquivo de entrada dos dados
	FILE *arquivoEntrada = NULL; // representa o arquivo de entrada dos dados
	char **dadosEntrada = NULL; // guarda os dados de entrada lidos pelo programa

	char *registroAux; // ponteiro auxiliar para guardar temporariamente registros
	int tamanhoAux; // auxiliar para guardar tamanhos temporariamente
	int offsetAux; // auxiliar para guardar deslocamentos temporariamente
	int chaveAux; // auxiliar para guardar chaves de indice temporariamente

	int funcaoMenu = FUNCAO_VAZIO; // guarda a funcao escolhida no menu principal
	int estatMenu = ESTAT_VAZIO; // guarda o tipo de arquivo escolhido nas estatisticas

	int indCont; // contador para percorrer os indices
	int indTam; // tamanho do indice percorrido
	int pararMostrar; // indicador de que deve parar de mostrar as estatisticas de indice
	char comandoParada; // guarda o comando de parada nas estatisticas de indice

	char **dadosInsercao; // guarda os dados a serem inseridos como novo registro
	int insCont; // contador para percorrer os campos da insercao

	/******** INICIALIZACAO DO PROGRAMA **********/
	// tenta recuperar os dados/indices do disco
	dadosFirst = fopen(DADOS_FIRSTFIT, "r+");
	dadosBest = fopen(DADOS_BESTFIT, "r+");
	dadosWorst = fopen(DADOS_WORSTFIT, "r+");

	indiceFirstArq = fopen(INDICE_FIRSTFIT, "r");
	indiceBestArq = fopen(INDICE_BESTFIT, "r");
	indiceWorstArq = fopen(INDICE_WORSTFIT, "r");

	// caso todos tenham sido abertos com sucesso (nenhum deles == NULL)
	if (dadosFirst && dadosBest && dadosWorst && indiceFirstArq && indiceBestArq && indiceWorstArq){
		// leitura dos indices para memoria primaria
		indiceFirst = recuperaIndice(indiceFirstArq);
		indiceBest = recuperaIndice(indiceBestArq);
		indiceWorst = recuperaIndice(indiceWorstArq);

		// fechando arquivos de indice antigos
		fclose(indiceFirstArq);
		fclose(indiceBestArq);
		fclose(indiceWorstArq);
	} else { // caso algum esteja faltando, recrie os arquivos a partir de uma nova entrada
		// fechando os arquivos que estivessem presentes
		if (dadosFirst != NULL) fclose(dadosFirst);
		if (dadosBest != NULL) fclose(dadosBest);
		if (dadosWorst != NULL) fclose(dadosWorst);
		if (indiceFirstArq != NULL) fclose(indiceFirstArq);
		if (indiceBestArq != NULL) fclose(indiceBestArq);
		if (indiceWorstArq != NULL) fclose(indiceWorstArq);

		// encontrando o arquivo de entrada
		printf("\nArquivos de dados/indices nao encontrados.\n");
		printf("Para continuar a execucao, novos arquivos de dados e indices devem ser criados.\n");
		while (nomeArquivoEntrada == NULL){
			printf(">> Digite o nome de um novo arquivo de entrada: ");
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

		// criando e inicializando arquivos de saida e estruturas de indice
		dadosFirst = inicializaArquivo(DADOS_FIRSTFIT); // First-Fit
		indiceFirst = criaIndice();

		dadosBest = inicializaArquivo(DADOS_BESTFIT); // Best-Fit
		indiceBest = criaIndice();

		dadosWorst = inicializaArquivo(DADOS_WORSTFIT); // Worst-Fit
		indiceWorst = criaIndice();

		// preparando os arquivos de dados
		while (!feof(arquivoEntrada)){ // enquanto ainda houver dados
			dadosEntrada = leCSV(arquivoEntrada); // leia um registro do arquivo de entrada
			
			// caso tenha chegado ao fim do arquivo, pare de ler
			if (feof(arquivoEntrada)){
				if (dadosEntrada != NULL) liberaCSV(dadosEntrada);
				break;
			}

			// insere nos arquivos de dados e libera a memória alocada para ele
			insereRegistro_Delimitador(dadosEntrada, dadosFirst);
			insereRegistro_Delimitador(dadosEntrada, dadosBest);
			insereRegistro_Delimitador(dadosEntrada, dadosWorst);

			liberaCSV(dadosEntrada);
			dadosEntrada = NULL;	
		}

		// preparando as estruturas de indice
		fseek(dadosFirst, sizeof(int), SEEK_SET); // voltando o arquivo a posicao inicial,
												  // depois do registro de cabecalho

		// enquanto ainda houver dados no arquivo de saída
		while (!feof(dadosFirst)){
			// recupera o byte offset do registro
			offsetAux = ftell(dadosFirst);

			// pegue um registro do arquivo de saída
			registroAux = buscaRegistro_Delimitador(dadosFirst, &tamanhoAux);


			if (!feof(dadosFirst) && registroAux != NULL){ // caso ele tenha dados válidos
				// recupera a chave utilizada no indice
				chaveAux = retornaTicket(registroAux);

				// adiciona o registro nas estruturas de indice
				inicializaIndice(indiceFirst, chaveAux, offsetAux);
				inicializaIndice(indiceBest, chaveAux, offsetAux);
				inicializaIndice(indiceWorst, chaveAux, offsetAux);

				// ordene os indices
				ordenaIndice(indiceFirst);
				ordenaIndice(indiceBest);
				ordenaIndice(indiceWorst);

				// libera a memória alocada para armazená-lo
				free(registroAux);
				registroAux = NULL;
			}
		}

		if (registroAux != NULL) free(registroAux);
	}

	/*********** MENU PRINCIPAL E FUNÇÕES DE MANIPULAÇÃO ***********/

	while (funcaoMenu != FUNCAO_SAIR){ // enquanto a opção de sair não for escolhida
		// escolhendo a função do programa a ser executada
		while (funcaoMenu == FUNCAO_VAZIO){
			printf("\n**BANCO DE DADOS DE DOMINIOS GOVERNAMENTAIS DE INTERNET**\n");
			printf("%d. Remover um registro a partir do ticket\n", FUNCAO_REMOVER);
			printf("%d. Inserir um novo registro\n", FUNCAO_INSERIR);
			printf("%d. Visualizar informacoes sobre os indices\n", FUNCAO_IND_ESTAT);
			printf("%d. Visualizar os indices simultaneamente\n", FUNCAO_IND_SIMUL);
			printf("%d. Visualizar estatisticas sobre os registros removidos\n", FUNCAO_ESTAT_REMOV);
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
			case FUNCAO_REMOVER:
				// Função: REMOVER REGISTRO

				// lendo o ticket do registro a ser removido
				chaveAux = -1; 
				while (chaveAux == -1){
					printf("\nDigite o ticket do dominio a ser removido: ");

					// caso não tenha sido escrito um número, peça outra entrada
					if (scanf("%d", &chaveAux) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						continue;
					}

					// caso não seja um numero positivo, peça outra entrada
					if (chaveAux < 0){
						printf("\nA entrada deve ser positiva. Digite novamente.\n");
						chaveAux = -1;
						continue;
					}
				}

				// tenta remover do primeiro arquivo
				if (removeRegistro_FirstFit(dadosFirst, indiceFirst, chaveAux)){
					// a remocao foi feita com sucesso. como a base de dados é igual
					// nos tres arquivos, mantenha o processo de remocao
					removeRegistro_BestFit(dadosBest, indiceBest, chaveAux);
					removeRegistro_WorstFit(dadosWorst, indiceWorst, chaveAux);

					// mostre uma mensagem informando sobre o sucesso da remocao
					printf("Registro removido com sucesso.\n");
				} else {
					// houve falha na remocao, registro nao existe
					printf("Registro nao foi encontrado no arquivo. Falha na remocao. Tente novamente.\n");
				}
			break;

			case FUNCAO_INSERIR:
				// Função: INSERIR REGISTRO

				// inicializando vetor de campos
				dadosInsercao = (char **) calloc(NUM_CAMPOS, sizeof(char *));

				// leia cada um dos campos de um registro
				for (insCont = 0; insCont < NUM_CAMPOS; insCont++){
					// escolha a frase a ser mostrada para o usuario
					switch (insCont){
						case 0:	printf("\nDigite o dominio: "); break;
						case 1: printf("\nDigite o documento (xxx.xxx.xxx/xxxx-xx):\n"); break;
						case 2: printf("\nDigite o nome do orgao/entidade: "); break;
						case 3: printf("\nDigite o UF do orgao/entidade: "); break;
						case 4: printf("\nDigite a cidade do orgao/entidade: "); break;
						case 5: printf("\nDigite a data e hora do cadastro (dd/mm/aaaa hh:mm:ss):\n"); break;
						case 6: printf("\nDigite a data e hora da ultima atualizacao (dd/mm/aaaa hh:mm:ss):\n"); break;
						case 7: printf("\nDigite o ticket: "); break;
					}

					// faca a leitura do dado
					while (dadosInsercao[insCont] == NULL){
						// cheque leitura de strings com tamanho fixo
						switch (insCont){
							case 1: dadosInsercao[insCont] = leStringFixa(TAM_DOCUMENTO-1); break;
							case 5: dadosInsercao[insCont] = leStringFixa(TAM_DATAHORACADASTRO-1); break;
							case 6: dadosInsercao[insCont] = leStringFixa(TAM_DATAHORAATUALIZA-1); break;
							default: dadosInsercao[insCont] = leString();
						}

						// caso a entrada digitada esteja vazia, peça outra entrada
						if (dadosInsercao[insCont] == NULL || dadosInsercao[insCont][0] == '\0'){
							printf("\nEntrada invalida. Digite novamente.\n");
							if (dadosInsercao[insCont] != NULL){
								free(dadosInsercao[insCont]);
								dadosInsercao[insCont] = NULL;
							}
						}
					}
				}

				// gera o registro a partir dos dados
				registroAux = criaRegistro(dadosInsercao, &tamanhoAux);

				// pega o ticket do registro criado
				chaveAux = retornaTicket(registroAux);

				// insere o novo registro no sistema
				insereRegistro_FirstFit(dadosFirst, indiceFirst, registroAux, tamanhoAux, chaveAux);
				insereRegistro_BestFit(dadosBest, indiceBest, registroAux, tamanhoAux, chaveAux);
				insereRegistro_WorstFit(dadosWorst, indiceWorst, registroAux, tamanhoAux, chaveAux);

				printf("Registro inserido com sucesso.\n");

				free(registroAux);
				liberaCSV(dadosInsercao);
			break;

			case FUNCAO_IND_ESTAT:
				// Função: ESTATISTICAS SOBRE OS ARQUIVOS DE INDICE

				// mostra tamanho de cada estrutura de indice
				printf("\n\n**********ARQUIVOS DE INDICE**********\n");
				printf("Indice usando First-Fit: %d registro(s) indexados\n",
					tamanhoIndice(indiceFirst));
				printf("Indice usando Best-Fit: %d registro(s) indexados\n",
					tamanhoIndice(indiceBest));
				printf("Indice usando Worst-Fit: %d registro(s) indexados\n",
					tamanhoIndice(indiceFirst));
				printf("\n\n");
			break;
			
			case FUNCAO_IND_SIMUL:
				// Função: VISUALIZAR INDICES SIMULTANEAMENTE

				// inicializar variaveis auxiliares
				indTam = tamanhoIndice(indiceFirst);
				pararMostrar = 0;

				// percorre todas as posicoes da estrutura de dados
				for (indCont = 0; indCont < indTam && !pararMostrar; indCont++){
					// recupera os dados da posicao atual
					itemFirst = dadosPosIndice(indiceFirst, indCont);
					itemBest = dadosPosIndice(indiceBest, indCont);
					itemWorst = dadosPosIndice(indiceWorst, indCont);

					// imprime em qual posicao esta
					printf("\n>>> POSICAO %d DOS INDICES:\n", indCont);

					// imprime o registro de indice nesta posicao e o registro correspondente
					printf("> FIRST-FIT || Ticket: %d | Byte Offset: %d |\n",
						itemFirst.chave, itemFirst.offset);
					fseek(dadosFirst, itemFirst.offset, SEEK_SET);
					registroAux = buscaRegistro_Delimitador(dadosFirst, &tamanhoAux);
					imprimeRegistro(registroAux);
					free(registroAux);

					printf("> BEST-FIT  || Ticket: %d | Byte Offset: %d |\n",
						itemBest.chave, itemBest.offset);
					fseek(dadosBest, itemBest.offset, SEEK_SET);
					registroAux = buscaRegistro_Delimitador(dadosBest, &tamanhoAux);
					imprimeRegistro(registroAux);
					free(registroAux);

					printf("> WORST-FIT || Ticket: %d | Byte Offset: %d |\n",
						itemWorst.chave, itemWorst.offset);
					fseek(dadosWorst, itemWorst.offset, SEEK_SET);
					registroAux = buscaRegistro_Delimitador(dadosWorst, &tamanhoAux);
					imprimeRegistro(registroAux);
					free(registroAux);

					// caso ainda tenha registros a serem impressos,
					// espere uma ação do usuário para mostrar o próximo
					if (indCont < indTam - 1){
						printf("\n\nDigite 'v' para voltar ao menu ou qualquer outra coisa para mostrar o proximo: ");
						comandoParada = fgetc(stdin);
						switch (comandoParada){
							case '\n': break; // entrada do teclado ja foi limpa
							case  'v': pararMostrar = 1; limpaEntrada(); break; // voltar ao menu
							  default: limpaEntrada(); break; // deve-se limpar a entrada e continuar
						}
					}
				}
			break;

			case FUNCAO_ESTAT_REMOV:
				// Função: ESTATISTICAS SOBRE OS REGISTROS REMOVIDOS

				// mostra tamanho de cada lista de registro removido
				printf("\n\n**********LISTAS DE REGISTROS REMOVIDOS**********\n");
				printf("Arquivo usando First-Fit: %d registro(s) removidos\n",
					tamanhoListaArquivo(dadosFirst));
				printf("Arquivo usando Best-Fit: %d registro(s) removidos\n",
					tamanhoListaArquivo(dadosBest));
				printf("Arquivo usando Worst-Fit: %d registro(s) removidos\n",
					tamanhoListaArquivo(dadosFirst));
				printf("\n\n");

				// escolhendo uma lista para ser mostrada
				estatMenu = ESTAT_VAZIO;
				while (estatMenu == ESTAT_VAZIO){
					printf("%d. Visualizar a lista First-Fit\n", ESTAT_FIRST);
					printf("%d. Visualizar a lista Best-Fit\n", ESTAT_BEST);
					printf("%d. Visualizar a lista Worst-Fit\n", ESTAT_WORST);
					printf("%d. Voltar ao menu principal\n", ESTAT_SAIR);
					printf("Deseja ver alguma das listas de registros removidos? ");

					// caso não tenha sido escrito um número, peça outra entrada
					if (scanf("%d", &estatMenu) == 0){
						limpaEntrada();
						printf("\nEntrada invalida. Digite novamente.\n");
						continue;
					}

					// caso não seja uma opção válida no menu, peça outra entrada
					if (estatMenu <= ESTAT_VAZIO || estatMenu >= ESTAT_QUANT){
						printf("\nLista invalida. Escolha outra.\n");
						estatMenu = ESTAT_VAZIO;
						continue;
					}
				}

				// caso uma lista tenha sido escolhida
				if (estatMenu != ESTAT_SAIR){
					printf("\n\n");
					switch (estatMenu){
						case ESTAT_FIRST: estatisticasLista(dadosFirst); break;
						case ESTAT_BEST: estatisticasLista(dadosBest); break;
						case ESTAT_WORST: estatisticasLista(dadosWorst); break;
					}
					printf("\n");
				}
			break;
		}

		// caso a função escolhida nao tenha sido a de sair,
		// zere a escolha para que outra função possa ser escolhida
		if (funcaoMenu != FUNCAO_SAIR) funcaoMenu = FUNCAO_VAZIO;
	}

	// salvar indices em disco
	if (!salvaIndice(indiceFirst, INDICE_FIRSTFIT))
		printf("Ocorreu um erro durante o salvamento do arquivo %s.\n", INDICE_FIRSTFIT);

	if (!salvaIndice(indiceBest, INDICE_BESTFIT))
		printf("Ocorreu um erro durante o salvamento do arquivo %s.\n", INDICE_BESTFIT);

	if (!salvaIndice(indiceWorst, INDICE_WORSTFIT))
		printf("Ocorreu um erro durante o salvamento do arquivo %s.\n", INDICE_WORSTFIT);

	// liberacao de memoria alocada
	fclose(dadosFirst);
	fclose(dadosBest);
	fclose(dadosWorst);

	liberaIndice(indiceFirst);
	liberaIndice(indiceBest);
	liberaIndice(indiceWorst);

	if (nomeArquivoEntrada != NULL) free(nomeArquivoEntrada);
	if (arquivoEntrada != NULL) fclose(arquivoEntrada);
}
