/**
    registro
    Funções que manipulam um registro unico e seus campos, como
    criacao de registro a partir de dados de entrada, impressao
    de um registro e busca e comparacao de campos em um registro.
**/

#ifndef _REGISTRO_H_
#define _REGISTRO_H_

char *criaRegistro(char **entradas, int *tamanhoRegistro);
void imprimeRegistro(char *registro);
void imprimeCampo(char *registro, int campo);
int comparacao_string(char *registro, char *buscando, int campo);
int comparacao(char *registro, int campo, char *busca);

#endif
