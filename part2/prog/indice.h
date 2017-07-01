#ifndef _INDICE_H_
#define _INDICE_H_

typedef struct IndiceItem {
	int chave;
	int offset;
} IndiceItem;

typedef struct Indice {
	int tamanho;
	IndiceItem *dados;
	FILE *arquivo;
} Indice;

Indice *criaIndice(char *nomeArquivo);
void liberaIndice(Indice *indice);
void inicializaIndice(Indice *indice, int chave, int offset);
void imprimeIndice(Indice *indice);
void ordenaIndice(Indice *indice);
void insereIndice(Indice *indice, int chave, int offset);

#endif