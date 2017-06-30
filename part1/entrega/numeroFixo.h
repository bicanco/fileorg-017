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
||   	numeroFixo.h											||
||   															||
|| Descrição:													||
||   	Funções que manipulam um arquivo unico de campos de		||	 
||   	tamanho variaveis com indicador de tamanho, campos de   ||
||   	tamanho fixos e registros de tamanhos variaveis com     ||
||   	número fixo de campos, incluindo a inserção e busca     ||
||   	de dados nesse padrão.                                  ||
\*==============================================================*/

#ifndef _NUMERO_FIXO_H_
#define _NUMERO_FIXO_H_

void insereRegistro_NumeroFixo(char **, FILE *);
char* buscaRegistro_NumeroFixo(FILE *);
char* buscaRRN_NumeroFixo(FILE *, int);

#endif
