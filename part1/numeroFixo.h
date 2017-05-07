#ifndef NUMERO_FIXO_H
#define NUMERO_FIXO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nVariavel 4 //numero de campos de tamanho variavel
//tamanho em byte dos campos fixos
#define documentoSize 19
#define dataHoraCadastroSize 20
#define dataHoraAtualizacaoSize 20
#define ticketSize 4


void escrever(char* CSV, FILE *fp);
char* busca(FILE *fp);

#endif
