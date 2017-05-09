/**
	caractere
	Biblioteca com funções auxiliares para o tratamento de texto.
**/

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
void stringMaiusculaAcentos(char *);
char acentoWin1252_UTF8(char);

#endif