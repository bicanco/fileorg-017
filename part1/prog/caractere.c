#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "caractere.h"

void limpaEntrada(){
	char c;
	do {
		c = fgetc(stdin);
	} while (c != '\n');
}

char *leString(){
	char *str = NULL;
	char c;
	int contador = 0;

	do {
		c = fgetc(stdin);
		str = (char *) realloc(str, sizeof(char) * (contador + 1));
		str[contador++] = c;
	} while (c != '\n');

	str[--contador] = '\0';
	return str;
}

void stringMaisculaAcentos(char *str){ 
	int tam = strlen(str); 

	int i;
	for(i=0; i<tam;i++){
		if (str[i] == INDICA_ACENTO){
			if (str[++i] >= INDICA_MINUSC_ACENTO)
				str[i] += TRANSF_MINUSC_MAIUSC_ACENTO;
		} else if (isalpha(str[i])){
			str[i] = toupper(str[i]); 
		}
	}
}

/*
á : Á | -61, -95 : -61, -127	| -31 : -63
é : É | -61, -87 : -61, -119	| -23 : -55
í : Í | -61, -83 : -61, -115	| -19 : -51
ó : Ó | -61, -77 : -61, -109	| -13 : -45
ú : Ú | -61, -70 : -61, -102	|  -6 : -38	

à : À | -61, -96 : -61, -128	| -32 : -64

â : Â |	-61, -94 : -61, -126	| -30 : -62
ê : Ê |	-61, -86 : -61, -118	| -22 : -54
ô : Õ | -61, -76 : -61, -108	| -12 : -44

ã : Ã |	-61, -93 : -61, -125	| -29 : -61
õ : Õ | -61, -75 : -61, -107	| -11 : -43

ç : Ç | -61, -89 : -61, -121	| -25 : -57
*/

char acentoWin1252_UTF8(char c){
	if (c == -31) return -95; // á
	if (c == -23) return -87; // é
	if (c == -19) return -83; // í
	if (c == -13) return -77; // ó
	if (c ==  -6) return -70; // ú

	if (c == -32) return -96; // à

	if (c == -30) return -94; // â
	if (c == -22) return -86; // ê
	if (c == -12) return -76; // ô

	if (c == -29) return -93; // ã
	if (c == -11) return -75; // õ

	if (c == -25) return -89; // ç

	// MAISCULAS (-32)
	if (c == -31 -32) return -95 -32; // á
	if (c == -23 -32) return -87 -32; // é
	if (c == -19 -32) return -83 -32; // í
	if (c == -13 -32) return -77 -32; // ó
	if (c ==  -6 -32) return -70 -32; // ú

	if (c == -32 -32) return -96 -32; // à

	if (c == -30 -32) return -94 -32; // â
	if (c == -22 -32) return -86 -32; // ê
	if (c == -12 -32) return -76 -32; // ô

	if (c == -29 -32) return -93 -32; // ã
	if (c == -11 -32) return -75 -32; // õ

	if (c == -25 -32) return -89 -32; // ç

	return 0;
}