/**
    Delimitador de final de registro
    Funções que manipulam um arquivo unico de campos de tamanho
    variaveis com indicador de tamanho,campos de tamanho fixos e
    registros de tamanhos variaveis com delimitador de final de registro, 
    incluindo a criacao de arquivo e busca de um registro.
**/

#ifndef _DELIMITADOR_H_
#define _DELIMITADOR_H_

#define FIXOS 64

void criaRegistro_Delimitador(char**, FILE*);
char *buscaRegistro_Delimitador(FILE*);
char *buscaRRN_Delimitador(FILE*, int);

#endif
