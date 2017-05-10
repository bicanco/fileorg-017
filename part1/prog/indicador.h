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
||   	indicador.h												||
||   															||
||	Descrição:													||
||    	Funções que manipulam um arquivo único de campos de 	||
||		tamanho variáveis com indicador de tamanho, campos de 	||
||		tamanho fixos e registros de tamanhos variáveis com 	||
||		indicadores de tamanho, incluindo a inserção e busca 	||
||		de dados nesse padrão.									||
\*==============================================================*/

#ifndef _INDICADOR_H_
#define _INDICADOR_H_

void insereRegistro_Indicador(char **, FILE *);
char *buscaRegistro_Indicador(FILE *);
char *buscaRRN_Indicador(FILE *, int);

#endif
