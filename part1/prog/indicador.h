/**
    indicador
    Funções que manipulam um arquivo unico de campos de tamanho
    variaveis com indicador de tamanho, campos de tamanho fixos e
    registros de tamanhos variaveis com indicadores de tamanho, 
    incluindo a inserção e busca de dados nesse padrão.
**/

#ifndef _INDICADOR_H_
#define _INDICADOR_H_

void insereRegistro_Indicador(char **, FILE *);
char *buscaRegistro_Indicador(FILE *);
char *buscaRRN_Indicador(FILE *, int);

#endif
