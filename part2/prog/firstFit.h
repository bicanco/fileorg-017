#ifndef _FIRSTFIT_H_
#define _FIRSTFIT_H_

#include <stdio.h>
#include "indice.h"

int removeRegistro_FirstFit(FILE *arquivo, Indice *indice, int chave);
void insereRegistro_FirstFit(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave);

#endif
