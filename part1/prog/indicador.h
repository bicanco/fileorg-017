/**
    indicador
    Funções que manipulam um arquivo único de campos de tamanho
    variáveis com indicador de tamanho, campos de tamanho fixos e
    registros de tamanhos variáveis com indicadores de tamanho, 
    incluindo a inserção e busca de dados nesse padrão.
**/

#ifndef _INDICADOR_H_
#define _INDICADOR_H_

void insereRegistro_Indicador(char **, FILE *);
char *buscaRegistro_Indicador(FILE *);
char *buscaRRN_Indicador(FILE *, int);

#endif
