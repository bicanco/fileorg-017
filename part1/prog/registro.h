/**
    registro
    Funções que manipulam um registro unico e seus campos, como
    criacao de registro a partir de dados de entrada, impressao
    de um registro e busca e comparacao de campos em um registro.
**/

#ifndef _REGISTRO_H_
#define _REGISTRO_H_

/**
	CONSTANTE: TAM_DOCUMENTO
	Tamanho do campo documento.
**/
#define TAM_DOCUMENTO 20

/**
	CONSTANTE: TAM_DATAHORACADASTRO
	Tamanho do campo dataHoraCadastro.
**/
#define TAM_DATAHORACADASTRO 20

/**
	CONSTANTE: TAM_DATAHORAATUALIZA
	Tamanho do campo dataHoraAtualiza.
**/
#define TAM_DATAHORAATUALIZA 20

/**
	CONSTANTE: TAM_TICKET
	Tamanho do campo ticket.
**/
#define TAM_TICKET 4

/**
	CONSTANTE: TAM_CAMPOS_FIXOS
	Tamanho de todos os campos fixos.
**/
#define TAM_CAMPOS_FIXOS 64

/**
	CONSTANTE: NUM_CAMPOS
	Número de campos no registro.
**/
#define NUM_CAMPOS 8

/**
	CONSTANTE: NUM_CAMPOS_FIXOS
	Número de campos de tamanho fixo no registro.
**/
#define NUM_CAMPOS_FIXOS 4

/**
	CONSTANTE: NUM_CAMPOS_VARIAVEIS
	Número de campos de tamanho variável no registro.
**/
#define NUM_CAMPOS_VARIAVEIS 4

char *criaRegistro(char **, int *);
void imprimeRegistro(char *);
void imprimeCampo(char *, int);
int comparaCampo(char *, int, char *);

#endif
