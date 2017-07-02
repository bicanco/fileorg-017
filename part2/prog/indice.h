#ifndef _INDICE_H_
#define _INDICE_H_

typedef struct IndiceItem {
	int chave;
	int offset;
} IndiceItem;

typedef struct Indice {
	int tamanho;
	IndiceItem *dados;
} Indice;

Indice *criaIndice();
void liberaIndice(Indice *indice);
void inicializaIndice(Indice *indice, int chave, int offset);
void imprimeIndice(Indice *indice);
void ordenaIndice(Indice *indice);
void insereIndice(Indice *indice, int chave, int offset);
int buscaIndice(Indice *indice, int chave);
int offsetIndice(Indice *indice, int chave);
IndiceItem dadosPosIndice(Indice *indice, int posicao);
void removeIndice(Indice *indice, int chave);
int tamanhoIndice(Indice *indice);

#endif