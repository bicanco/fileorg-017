#include <numeroFixo.h>
#include <registro.h>

void escrever(char** CSV, FILE *fp){
	//CSV = string de entrada com todos os campos
	//fp  = FILE* ja aberto atenteriormente, referente ao arquivo a ser escrito
	int len; 
	char* registro;

	registro = criaRegistro(CSV);
	//Len = numero de bytes contidos no 
	len = strlen(registro);
	fwrite(registro, sizeof(char), len, fp);
}




//   campoFixo1campoFixo2campoFixo4campoFixo4\SIZE\CampoVariado1\SIZE\CampoVariado2\SIZE\CampoVariado3\SIZE\CampoVariado4
//   \SIZE\ = 4 bytes (int)
   /*
	* ler até o fim do registro para determinar seu tamanho
	* allocar um vetor do tamanho encontrado
	* dar fseek() para o inicio do registrador 
	* fread() do fp para o vetor allocado
	* dar fseek() para o final do registrador
	* retornar o char* 
	* obs: a funcao que chama busca() deve, na primeira chamada, mandar a posicao de fp no inicio do FILE* (rewind(fp))
	*/

char* busca(FILE *fp){
	char *reg; //registro lido
	int tamanho, i, tamanhoFixo, tamanhoVariado;

	tamanhoFixo = documentoSize + dataHoraCadastroSize + dataHoraAtualizacaoSize + ticketSize;
	tamanhoVariado = 0;
	fseek(fp, tamanhoFixo, SEEK_CUR);
	for(i=0; i<nVariavel; i++){
		//Le o tamanho do proximo campo e armazena o valor em tamanho;
		fread(&tamanho, sizeof(int), 1, fp);
		fseek(fp, tamanho, SEEK_CUR);
		tamanhoVariado += sizeof(int)+tamanho;
	}

	reg = (char*)malloc(sizeof(char)*(tamanhoFixo+tamanhoVariado)); //+1 por causa do '\0'
	fseek(fp, -(tamanhoFixo+tamanhoVariado), SEEK_CUR); //volta ao inicio do registro
	fread(reg, sizeof(char), tamanhoFixo+tamanhoVariado, fp);

	return reg;
}
