#include<stdlib.h>
#include<stdio.h>

#include"delimitador.h"

void firstFit(FILE* fp, char **csv, int tamanho, int byteoffset, int byteoffset_anterior){
	int espaco, proximo;
	char delimitador = DELIMITADOR;

	if(byteoffset == -1){
		fseek(fp, 0,SEEK_END);
		insereRegistro_Delimitador(csv, fp);
		return;
	}


	fseek(fp, byteoffset, SEEK_SET);

	fgetc(fp);
	fread(&espaco, sizeof(int), 1, fp):
	fread(&proximo, sizeof(int), 1, fp);

	if( espaco == tamanho){
		fseek(fp, byteoffset, SEEK_SET);
		insereRegistro_Delimitador(csv,fp);
		fseek(fp, byteoffset_anterior+5,SEEK_SET);
		fwrite(&proximo, sizeof(int),1,fp);
		return;
	}else if(espaco >= tamanho){
		aux = espaco-(tamanho+1);

		fseek(fp, byteoffset+1, SEEK_SET);
		fwrite(&aux,sizeof(int),1,fp);
		fseek(fp, byteoffset+aux, SEEK_SET);
		fwrite(delimitador, sizeof(char),1,fp);
		insereRegistro_Delimitador(csv,fp);
		return;
	}else{
		firstFit(fp, csv, tamanho, proximo, byteoffset);
	}
}
