#include<stdlib.h>
#include<stdio.h>

#include"delimitador.h"


void worstFit(FILE *fp, char **csv, int tamanho, int list_top){
	int espaco;
	char delimitador = DELIMITADOR;

	fseek(fp, list_top, SEEK_SET);
	fgetc(fp);
	fread(&espaco, sizeof(int), 1, fp);

	if(espaco < tamanho){
		fseek(fp,0,SEEK_END);
		insereRegistro_Delimitador(csv,fp);
		return;
	}else{
		fseek(fp, list_top+(espaco-tamanho-1),SEEK_SET);
		fwrite(&delimitador,sizeof(char),1,fp);
		insereRegistro_Delimitador(csv,fp);
		return;
	}
}
