#ifndef _BESTFIT_H_
#define _BESTFIT_H_

#include <stdio.h>
#include "indice.h"

int removeRegistro_BestFit(FILE *arquivo, Indice *indice, int chave);
void insereRegistro_BestFit(FILE *arquivo, Indice *indice, char *reg, int tamanho, int chave);

#endif