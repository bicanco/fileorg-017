#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define INDICA_ACENTO -61
#define INDICA_MINUSC_ACENTO -96
#define TRANSF_MINUSC_MAIUSC_ACENTO -32

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