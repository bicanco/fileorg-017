/*==============================================================*\
||  Segundo Trabalho Pr�tico - Organiza��o de Arquivos(SCC215) ||
||   				  			       ||
|| Alunos:  				N� USP: 	       ||
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
||   	bestFit.h					       ||
||   							       ||
|| Descri��o:    					       ||
||     Biblioteca que aplica o m�todo de Best-Fit              ||
\*==============================================================*/
#ifndef _BESTFIT_H_
#define _BESTFIT_H_

#include <stdio.h>
#include "indice.h"

int removeRegistro_BestFit(FILE *arquivo, Indice *indice, int chave);
void insereRegistro_BestFit(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave);

#endif
