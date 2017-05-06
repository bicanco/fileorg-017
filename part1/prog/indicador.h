/**
    IndicadorTamanho
    Funções que manipulam um arquivo unico de campos de tamanho
    variaveis com indicador de tamanho,campos de tamanho fixos e
    registros de tamanhos variaveis com indicador de tamanho, 
    incluindo a criacao de arquivo e busca de um registro.

**/
#ifndef _INDICADOR_H_
#define _INDICADOR_H_

char* criaRegistro_com_indicador(char** string, int* tam_registro);
char* criarArquivo_Indicador(FILE *origin);
#endif