#ifndef _WORSTFIT_H_
#define _WORSTFIT_H_

#include <stdio.h>
#include "indice.h"

int removeRegistro_WorstFit(FILE *arquivo, Indice *indice, int chave);
void insereRegistro_WorstFit(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave);

#endif