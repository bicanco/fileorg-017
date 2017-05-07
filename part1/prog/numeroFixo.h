#ifndef NUMERO_FIXO_H
#define NUMERO_FIXO_H

#define nVariavel 4 //numero de campos de tamanho variavel
//tamanho em byte dos campos fixos
#define documentoSize 20
#define dataHoraCadastroSize 20
#define dataHoraAtualizacaoSize 20
#define ticketSize 4


void insereRegistro_NumeroFixo(char** CSV, FILE *fp);
char* buscaRegistro_NumeroFixo(FILE *fp);
char* buscaRRN_NumeroFixo(FILE *fp, int RRN);

#endif
