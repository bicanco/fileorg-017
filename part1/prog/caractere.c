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
\*==============================================================*/

/**
	caractere
	Biblioteca com funções auxiliares para o tratamento de texto.
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "caractere.h"

/**
	CONSTANTE: INDICA_MINUSC_ACENTO
	O menor dos valores numéricos de um caractere acentuado,
	em UTF-8, usado para verificação.
**/
#define INDICA_MINUSC_ACENTO -96

/**
	CONSTANTE: TRANSF_MINUSC_MAIUSC_ACENTO
	Número que deve ser somado ao valor numérico de um
	caractere acentuado em UTF-8 para transformá-lo de minúscula
	para maiúscula.
**/
#define TRANSF_MINUSC_MAIUSC_ACENTO -32

/**
	limpaEntrada
	Limpa o retentor da entrada padrão (teclado) até uma quebra de linha.
**/
void limpaEntrada(){
	char c; // caractere auxiliar para leitura de caracteres
	do {
		c = fgetc(stdin); // leia um caractere da entrada padrão
	} while (c != '\n'); // até que seja lido uma quebra de linha
}

/**
	leString
	Le uma string da entrada padrão até uma quebra de linha.

	RETORNA | endereço de memória da string lida
**/
char *leString(){
	char *str = NULL; // ponteiro para armazenar o endereço da string
	char c; // caractere auxiliar para leitura de caracteres
	int contador = 0; // contador de caracteres lidos

	do {
		c = fgetc(stdin); // leia um caractere da entrada padrão
		// realoque ḿemória para colocar o novo caractere lido
		str = (char *) realloc(str, sizeof(char) * (contador + 1));
		// coloque o caractere lido na string, atualize o contador
		str[contador++] = c;
	} while (c != '\n'); // até que seja lido uma quebra de linha

	// coloca o indicador de fim de string
	str[--contador] = '\0';
	return str;
}

/**
	stringMaiusculaAcentos
	Converte todas as letras de uma string para letras maiúsculas,
	incluindo letras acentuadas.

	PARAMETRO -str- | ponteiro para a string a ser alterada
**/
void stringMaiusculaAcentos(char *str){ 
	int tam = strlen(str); // calcula o tamanho da string

	int i; // variavel para iteração do laço
	for(i=0; i<tam;i++){ // para cada um dos caracteres da string
		if (str[i] == INDICA_ACENTO){ // caso ele tenha um byte indicador de acento
			if (str[++i] >= INDICA_MINUSC_ACENTO) // e o próximo byte seja uma letra acentuada
				// some o valor de transformação da letra acentuada
				str[i] += TRANSF_MINUSC_MAIUSC_ACENTO; 
		} else if (isalpha(str[i])){ // caso seja uma letra não acentuada
			str[i] = toupper(str[i]); // faça a transformação para letra maiúscula
		}
	}
}

/**
	acentoWin1252_UTF8
	Retorna o valor numérico do segundo byte de um caractere acentuado em UTF-8
	a partir do valor numérico do mesmo caractere acentuado em Windows-1252.

	PARAMETRO -c- | valor numérico do caractere em Windows-1252
	RETORNA | valor numérico do segundo byte do caractere em UTF-8
			| OU 0, caso não seja um caractere acentuado
**/
char acentoWin1252_UTF8(char c){
	// letras minúsculas
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

	// LETRAS MAISCÚLAS (valores minúsculos -32)
	if (c == -31 -32) return -95 -32; // Á
	if (c == -23 -32) return -87 -32; // É
	if (c == -19 -32) return -83 -32; // Í
	if (c == -13 -32) return -77 -32; // Ó
	if (c ==  -6 -32) return -70 -32; // Ú

	if (c == -32 -32) return -96 -32; // À

	if (c == -30 -32) return -94 -32; // Â
	if (c == -22 -32) return -86 -32; // Ê
	if (c == -12 -32) return -76 -32; // Ô

	if (c == -29 -32) return -93 -32; // Ã
	if (c == -11 -32) return -75 -32; // Õ

	if (c == -25 -32) return -89 -32; // Ç

	// caso não seja um caractere acentuado em Windows-1252, retorne 0
	return 0;
}
