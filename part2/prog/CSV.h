/*==============================================================*\
||  Segundo Trabalho Pr�tico - Organiza��o de Arquivos(SCC215) ||
||   				  			       ||
|| Alunos:  				N� USP: 	       ||
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
||   	CSV.h						       ||
||   							       ||
|| Descri��o:    					       ||
||	Fun��es que manipulam arquivos CSV para leitura e      ||
||		escrita.                                       ||
\*==============================================================*/


#ifndef _CSV_H_
#define _CSV_H_

char **leCSV(FILE *);
void liberaCSV(char **);

#endif
