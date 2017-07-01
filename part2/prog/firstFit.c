#include<stdlib.h>
#include<stdio.h>

#include"delimitador.h"

void firstFit(FILE* fp, char **csv, int tamanho, int rrn, int rrn_anterior){
	int espaco, proximo;
	char delimitador = DELIMITADOR;

	if(rrn == -1){
		fseek(fp, 0,SEEK_END);
		insereRegistro_Delimitador(csv, fp);
		return;
	}


	fseek(fp, rrn, SEEK_SET);

	fgetc(fp);
	fread(&espaco, sizeof(int), 1, fp):
	fread(&proximo, sizeof(int), 1, fp);

	if( espaço == tamanho){
		fseek(fp, rrn, SEEK_SET);
		insereRegistro_Delimitador(csv,fp);
		fseek(fp, rrn_anterior+5,SEEK_SET);
		fwrite(&proximo, sizeof(int),1,fp);
		return;
	}else if(espaço >= tamanho+10){
		aux = espaco-(tamanho+1);

		fseek(fp, rrn+1, SEEK_SET);
		fwrite(&aux,sizeof(int),1,fp);
		fseek(fp, rrn+aux, SEEK_SET);
		fwrite(delimitador, sizeof(char),1,fp);
		insereRegistro_Delimitador(csv,fp);
		fseek(fp, rrn_anterior+5, SEEK_SET);
		fwrite(&proximo, sizeof(int), 1, fp):
		return;
	}else{
		firstFit(fp, csv, tamanho, proximo, rrn);
	}
}
