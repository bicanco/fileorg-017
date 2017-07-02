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
||   	caractere.h					       ||
||   							       ||
|| Descrição:    					       ||
||	Biblioteca com funções auxiliares para o tratamento    ||
||		de texto.                                      ||
\*==============================================================*/

#ifndef _CARACTERE_H_
#define _CARACTERE_H_

/**
	CONSTANTE: INDICA_ACENTO
	O valor numÃ©rico do primeiro byte de todos os caracteres
	acentuados da lÃ­ngua portuguesa em UTF-8.
**/
#define INDICA_ACENTO -61

void limpaEntrada();
char *leString();
char *leStringFixa(int tamanho);
void stringMaiusculaAcentos(char *);
char acentoWin1252_UTF8(char);

#endif
