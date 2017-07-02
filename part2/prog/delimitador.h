/*==============================================================*\
||  Segundo Trabalho Prático - Organização de Arquivos(SCC215) ||
||   				  			       ||
|| Alunos:  				Nº USP: 	       ||
||   	David Souza Rodrigues			       4461180 ||
||   	Fernanda Tostes Marana  	               4471070 ||
||   	Gabriel Toschi de Oliveira  		       9763039 ||
||   	Marcelo de Moraes Carvalho da Silva 	       9791048 ||
||							       ||
|| Professora: 						       || 
||   	Dra. Cristina Dutra de Aguiar Ciferri  	               ||
||  							       ||
|| Entrega:						       || 
||     	02/07/2017					       ||
||   							       ||
|| Arquivo: 						       ||
||   	delimitador.h					       ||
||   							       ||
|| Descrição:    					       ||
||	Funções que manipulam um arquivo unico de campos de    ||
||	tamanho variaveis com indicador de tamanho, campos de  ||
||	tamanho fixos e registros de tamanhos variaveis com    ||
||	delimitador de final de registro, incluindo a inserção ||
||	e busca de dados nesse padrão.	           	       ||
\*==============================================================*/


#ifndef _DELIMITADOR_H_
#define _DELIMITADOR_H_

/**
	CONSTANTE: DELIMITADOR
	Caractere usado como delimitador entre registros.
**/
#define DELIMITADOR -1
#define FIM_DE_LISTA -1
#define REMOVIDO -2

#include "indice.h"

int insereRegistro_Delimitador(char **, FILE *);
void insereRegistro_Inicializa(char **csv, FILE *fds, Indice *indice);
FILE *inicializaArquivo(char *nomeArquivo);
int retornaTopoArquivo(FILE *arquivo);
void atualizaTopoArquivo(FILE *arquivo, int novoTopo);
int tamanhoListaArquivo(FILE *arquivo);
int tamanhoRegistro_Delimitador(FILE *fp);
void estatisticasLista(FILE *arquivo);
char *buscaRegistro_Delimitador(FILE *fp, int *tamanhoRegistro);

#endif
