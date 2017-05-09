/**
    numeroFixo
    Funções que manipulam um arquivo unico de campos de tamanho
    variaveis com indicador de tamanho, campos de tamanho fixos e
    registros de tamanhos variaveis com número fixo de campos, 
    incluindo a inserção e busca de dados nesse padrão.
**/

#ifndef _NUMERO_FIXO_H_
#define _NUMERO_FIXO_H_

void insereRegistro_NumeroFixo(char **, FILE *);
char* buscaRegistro_NumeroFixo(FILE *);
char* buscaRRN_NumeroFixo(FILE *, int);

#endif
