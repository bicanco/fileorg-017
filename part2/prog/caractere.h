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
||   	caractere.h					       ||
||   							       ||
|| Descri��o:    					       ||
||	Biblioteca com fun��es auxiliares para o tratamento    ||
||		de texto.                                      ||
\*==============================================================*/

#ifndef _CARACTERE_H_
#define _CARACTERE_H_

/**
	CONSTANTE: INDICA_ACENTO
	O valor numérico do primeiro byte de todos os caracteres
	acentuados da língua portuguesa em UTF-8.
**/
#define INDICA_ACENTO -61

void limpaEntrada();
char *leString();
char *leStringFixa(int tamanho);
void stringMaiusculaAcentos(char *);
char acentoWin1252_UTF8(char);

#endif
