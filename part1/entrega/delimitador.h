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
||  Arquivo: 													||
||   	delimitador.h											||
||   															||
|| Descrição:													||
||    	Funções que manipulam um arquivo unico de campos de 	||
||		tamanho variaveis com indicador de tamanho, campos de 	||
||		tamanho fixos e registros de tamanhos variaveis com 	||
||		delimitador de final de registro, incluindo a inserção 	||
||		e busca de dados nesse padrão.							||
\*==============================================================*/

#ifndef _DELIMITADOR_H_
#define _DELIMITADOR_H_

void insereRegistro_Delimitador(char **, FILE *);
char *buscaRegistro_Delimitador(FILE *);
char *buscaRRN_Delimitador(FILE *, int);

#endif
