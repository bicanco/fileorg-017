#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int *creatRandomVector(int n, int limit){
	int *random, i;
	time_t t;

	srand((unsigned)time(&t));
	random = (int*)malloc(sizeof(int)*n);
	for(i=0; i<n; i++){
		random[i] = rand() % limit;
	}
	return random;
}

char **createRandomStringVector(int n, int maxLen){
	char **stringVector;
	char* newString;
	int i, randomLen, j;

	stringVector = (char**)malloc(sizeof(char*)*n);
	for(i=0; i<n; i++){
		randomLen = rand() % maxLen;
		newString = (char*)malloc(sizeof(char)*(randomLen+1));
		for(j=0; j<randomLen; j++){
			newString[j] = (rand() % 93)+33; //caracteres validos da ASCII
		}
		newString[j] = '\0';
		stringVector[i] = newString;
	}
	return stringVector;
}
		
				


int main(){
	int n, i, j, k, limit, maxLen;
	int *random;
	char** randomString;
	FILE **fp;

	//files de input a serem criados
	fp = (FILE**)malloc(sizeof(FILE*)*3);
	fp[0] = fopen("input0.in", "w+");
	fp[1] = fopen("input1.in", "w+");
	fp[2] = fopen("input2.in", "w+");

	//quantidade de valores a serem usados no RRN
	n = 100000;
	//limite do valor do rrn
	limit = 1000;
	//cria um vetor de valores aleatorios
	random = creatRandomVector(n, limit);

	//cria um vetor de strings de tamanho aleatorio de tamanho maximo = maxLen
	maxLen = 30;
	randomString = createRandomStringVector(n, maxLen); 

	//repete 3x, para cada operacao
	for(i=0; i<3; i++){
		//valor da operacao
		fprintf(fp[i], "%d\n", i);
		//arquivo de entradas
		fprintf(fp[i], "input.csv\n");
		//ler todos os registros
		fprintf(fp[i], "0\n");

		//por campo
		for(k=0; k<n; k++){
			fprintf(fp[i], "1\n");
			for(j=0; j<6; j++){
				fprintf(fp[i], "%d\n", j);
				fprintf(fp[i], "%s\n", randomString[k]);
			}
		}
		//rrn
		for(k=0; k<n; k++){
			fprintf(fp[i], "2\n");
			fprintf(fp[i], "%d\n", random[k]);
		}
		//rrn por campo
		for(k=0; k<n; k++){
			fprintf(fp[i], "3\n");
			for(j=0; j<6; j++){
				fprintf(fp[i], "%d\n", random[k]);
				fprintf(fp[i], "%d\n", j);
			}
		}
		//fim
		fprintf(fp[i], "4\n");
	}
	
	//frees
	free(random);
	fclose(fp[0]);
	fclose(fp[1]);
	fclose(fp[2]);
}
