/*==============================================================*\
||  Segundo Trabalho Prático - Organização de Arquivos(SCC215) ||
||   				  			       ||
|| Alunos:  				Nº USP: 	       ||
||   	David Souza Rodrigues			       4461180 ||
||   	Fernanda Tostes Marana  	               4471070 ||
||   	Gabriel Toschi de Oliveira  		       9763039 ||
||   	Marcelo de Moraes Carvalho da Silva 	       9791048 ||
||							       ||
|| Professora: 						       || 
||   	Dra. Cristina Dutra de Aguiar Ciferri  	               ||
||  							       ||
|| Entrega:						       || 
||     	02/07/2017					       ||
||   							       ||
|| Arquivo: 						       ||
||   	indice.h					       ||
||   							       ||
|| Descrição:    					       ||
||	Biblioteca com funções de tratamento de Índice         ||
\*==============================================================*/
#ifndef _INDICE_H_
#define _INDICE_H_

//estrutura de um item do índice
typedef struct IndiceItem {
	int chave;
	int offset;
} IndiceItem;

//estrutura de um índice
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
int salvaIndice(Indice *indice, char *nomeArquivo);
Indice *recuperaIndice(FILE *arquivo);

#endif
