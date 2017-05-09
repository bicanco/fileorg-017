/**
    registro
    Funções que manipulam um registro unico e seus campos, como
    criacao de registro a partir de dados de entrada, impressao
    de um registro e busca e comparacao de campos em um registro.
**/

#ifndef _REGISTRO_H_
#define _REGISTRO_H_

char *criaRegistro(char **, int *);
void imprimeRegistro(char *);
void imprimeCampo(char *, int);
int comparaCampo(char *, int, char *);

#endif
