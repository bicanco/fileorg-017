#ifndef _CARACTERE_H_
#define _CARACTERE_H_

#define INDICA_ACENTO -61
#define INDICA_MINUSC_ACENTO -96
#define TRANSF_MINUSC_MAIUSC_ACENTO -32

void limpaEntrada();
char *leString();
void stringMaisculaAcentos(char *str);
char acentoWin1252_UTF8(char c);

#endif