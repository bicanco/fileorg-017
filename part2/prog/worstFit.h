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
||   	worstFit.h					       ||
||   							       ||
|| Descrição:    					       ||
||     Biblioteca que aplica o método de Worst-Fit             ||
\*==============================================================*/
#ifndef _WORSTFIT_H_
#define _WORSTFIT_H_

#include <stdio.h>
#include "indice.h"

int removeRegistro_WorstFit(FILE *arquivo, Indice *indice, int chave);
void insereRegistro_WorstFit(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave);

#endif
