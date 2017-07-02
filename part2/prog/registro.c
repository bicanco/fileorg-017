/*==============================================================*\
||  Segundo Trabalho Pr�tico - Organiza��o de Arquivos(SCC215)   ||
||   				  			         ||
|| Alunos:  				N� USP: 	         ||
||   	David Souza Rodrigues			       4461180   ||
||   	Fernanda Tostes Marana  	               4471070   ||
||   	Gabriel Toschi de Oliveira  		       9763039   ||
||   	Marcelo de Moraes Carvalho da Silva 	       9791048   ||
||							         ||
|| Professora: 						         || 
||   	Dra. Cristina Dutra de Aguiar Ciferri  	                 ||
||  							         ||
|| Entrega:						         || 
||     	02/07/2017					         ||
||   							         ||
|| Arquivo: 						         ||
||   	registro.c					         ||
||   							         ||
|| Descri��o:    					         ||
||	Fun��es que manipulam um registro din�mico e seus campos,||
||     	como cria��o de registro a partir de dados de entrada,   ||
||   	impress�o de um registro e busca e compara��o de campos  ||
||   	em um registro. 	           	                 ||
\*==============================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "caractere.h"
#include "registro.h"

/**
    criaRegistro
    Cria e arruma um registro, de acordo com dados recebidos do arquivo CSV e 
    a documentação do trabalho, quanto ao tamanho, à ordem e a forma com que
    cada campo é armazenado.
    PARAMETRO -entradas- | campos lidos do arquivo CSV
    PARAMETRO -tamanhoRegistro- | endereco de um inteiro, para ser retornado o tamanho do registro
    RETORNA | vetor de bytes, que contém o registro criado
**/
char *criaRegistro(char **entradas, int *tamanhoRegistro){
    // sequencia de campos da entrada do arquivo CSV a ser
    // escrita no arquivo     
    int sequenciaEntradas[] = {1,5,6,7,0,2,4,3};

    char *campo; // ponteiro auxiliar para um campo
    int campoNulo; // indicador de campo nulo
    int contadorCampos = 0; // contador de campos
    char *registro; // ponteiro para o registro
    
    // calculando tamanho do registro
    int tamDominio = strlen(entradas[0]) + 1;  
    int tamNome = strlen(entradas[2]) + 1; 
    int tamUF = strlen(entradas[3]) + 1;   
    int tamCidade =  strlen(entradas[4]) + 1;  
    *tamanhoRegistro = 20+20+20+4+16+tamNome+tamDominio+tamCidade+tamUF;

    // alocando memoria para guardar temporariamente o registro
    registro = (char *) malloc(sizeof(char)*(*tamanhoRegistro));  
    int indice = 0;
    int tam, campoInt;
    
    // para cada um dos campos, na ordem de serem colocados no registro
    for (contadorCampos = 0; contadorCampos < NUM_CAMPOS; contadorCampos++){
        campoNulo = 0;

        // pegue o campo certo, de acordo com a sequencia     
        campo = entradas[sequenciaEntradas[contadorCampos]];
        
        // pegue o tamanho do campo correto, de acordo com a sequencia
        switch (sequenciaEntradas[contadorCampos]){
            case 0: tam = tamDominio; break; // dominio
            case 1: tam = TAM_DOCUMENTO; break; // documento
            case 2: tam = tamNome; break; // nome
            case 3: tam = tamUF; break; // cidade
            case 4: tam = tamCidade; break; // uf
            case 5: case 6: tam = TAM_DATAHORACADASTRO; break; // dataHoraCadastro, dataHoraAtualiza
            case 7: tam = sizeof(int); break; // ticket
        }
        
        if (sequenciaEntradas[contadorCampos] == 7){ // colocar um inteiro no registro (ticket)
            // colocando no formato de inteiro
            campoInt = atoi(campo);
            
            // escrever inteiro no registro
            memcpy(&registro[indice], &campoInt, tam);

        } else { // colocar uma string no registro
            if (contadorCampos >= NUM_CAMPOS_FIXOS){
            	// o campo eh de tamanho variavel
                // colocar indicador de tamanho
                memcpy(&registro[indice], &tam, sizeof(int));
            	indice += sizeof(int);
            } else {
                // o campo eh de tamanho fixo, verifique se eh nulo
                if (strcmp(campo, "null") == 0){
                    // o campo eh nulo
                    campo = (char *) calloc(tam, sizeof(char));
                    campoNulo = 1;
                }
            }
            
            // escrever string no registro
            memcpy(&registro[indice], campo, tam);

            if (campoNulo) free(campo);
        }
        
        // atualizar indice percorrido no registro
        indice += tam;      
    }

    return registro;
}

/**
    mapeiaRegistro
    Localiza e armaneza o endereco do primeiro byte de cada campo do registro.

    PARAMETRO -registro- | vetor de bytes, que contém o registro organizado na ordem dos 
                           campos do arquivo binário
    RETORNA | vetor de inteiros, que contém o endereço do primeiro byte de cada campo
**/
int *mapeiaRegistro(char *registro){
    int *mapaIndices = (int *) malloc(sizeof(int) * 8);
    int i, indiceAux, tamanhoCampo;
    //armazenando o endereco dos campos de tamanho fixo a partir dos tamanhos pré-definidos 
    mapaIndices[0] = 0; //documento
    mapaIndices[1] = TAM_DOCUMENTO; //dataHoraCadastro
    mapaIndices[2] = mapaIndices[1] + TAM_DATAHORACADASTRO; //dataHoraAtualiza
    mapaIndices[3] = mapaIndices[2] + TAM_DATAHORAATUALIZA; //ticket
    //sabendo que o dominio vem depois de seu indicador de tamanho - cujo primeiro byte do endereço é 64 
    //e o tamanho é de 4 bytes - é possível saber que o endereço do domínio será no byte 68 do arquivo binário
    mapaIndices[4] = mapaIndices[3] + TAM_TICKET + sizeof(int); // domínio
    indiceAux = TAM_CAMPOS_FIXOS; // endereço do primeiro indicador de tamanho

    // loop até percorrer todos campos e indicadores de tamanho do registro
    for (i = NUM_CAMPOS_FIXOS + 1; i < NUM_CAMPOS; i++){ 
         // tamanhoCampo recebe o valor do indicador de tamanho na posição indiceAux
        memcpy(&tamanhoCampo, &registro[indiceAux], sizeof(int)); 
        // indiceAux aponta para o próximo indicador de tamanho do campo seguinte
        indiceAux += sizeof(int) + tamanhoCampo;
        //o vetor de mapaIndices no indice i aponta para o endereço do primeiro byte do próximo campo de tamanho variável
        mapaIndices[i] = indiceAux + sizeof(int);
    }

    return mapaIndices;
}
/**
	retornaTicket

	Fun��o que retorna o campo Ticket
	PARAMTRO -registro- | o registro de que se deseja o ticket
	RETORNA | o ticket
**/
int retornaTicket(char *registro){
    int *indicesCampos = mapeiaRegistro(registro);
    int ticket;

    memcpy(&ticket, &registro[indicesCampos[3]], sizeof(int));
    free(indicesCampos);
    return ticket;
}

/**
    imprimeRegistro
    Imprime todos campos de um registro numa ordem compreensível ao usuário.

    PARAMETRO -registro- | vetor de bytes, que contém o registro organizado
                           na ordem dos campos do arquivo binário

**/
void imprimeRegistro(char *registro){
    // sequência de campos da entrada do arquivo binário a ser
    // impressa na tela     
    int sequenciaImpressao[] = {4, 3, 1, 2, 5, 6, 7, 0}; 
    int contadorCampos, campoInt;

    int *indicesCampos = mapeiaRegistro(registro);
    int indice;

     // para cada um dos campos, na ordem de serem colocados no registro
    for (contadorCampos = 0; contadorCampos < NUM_CAMPOS; contadorCampos++){ 
         // pegue o endereço do campo certo, de acordo com a sequencia   
        indice = indicesCampos[sequenciaImpressao[contadorCampos]];

        // pegue o campo correto, de acordo com a sequencia
        switch (sequenciaImpressao[contadorCampos]){
            case 0: printf("CNPJ: %s\n\n\n", &registro[indice]); break; // documento
            case 1: printf("Cadastro feito em %s\n", &registro[indice]); break; //dataHoraCadastro
            case 2: printf("Ultima atualizacao em %s\n", &registro[indice]); break; //dataHoraAtualiza
            case 3:                                                 // ticket
                memcpy(&campoInt, &registro[indice], sizeof(int));
                printf("(ticket %d)\n", campoInt);
                break;
            case 4: printf("DOMINIO: %s ", &registro[indice]); break; // dominio
            case 5: printf("\nORGAO/ENTIDADE:\n%s\n", &registro[indice]); break; //nome
            case 6: printf("%s ", &registro[indice]); break; //cidade
            case 7: printf("(%s)\n", &registro[indice]); break; //UF
        }
    }

    free(indicesCampos);
}

/**
    imprimeCampo
    Imprime um campo - escolhido pelo usuário - de um registro.

    PARAMETRO -registro- | vetor de bytes, que contém o registro organizado na
                           ordem dos campos do arquivo binário
    PARAMETRO -campo- | valor inteiro que representa o campo desejado a ser impresso
**/
void imprimeCampo(char *registro, int campo){
    int *indicesCampos = mapeiaRegistro(registro);
    int indice =  indicesCampos[campo]; // indice recebe o endereço do primeiro byte do campo escolhido
    int campoInt;

    switch (campo){
        case 0: printf("Documento: %s\n", &registro[indice]); break; // documento
        case 1: printf("Cadastro feito em %s\n", &registro[indice]); break; //dataHoraCadastro
        case 2: printf("Ultima atualizacao em %s\n", &registro[indice]); break; //dataHoraAtualiza
        case 3:                                                 // ticket
            memcpy(&campoInt, &registro[indice], sizeof(int));
            printf("Ticket: %d\n", campoInt);
            break;
        case 4: printf("Domínio: %s\n", &registro[indice]); break; //domínio
        case 5: printf("Orgao/Entidade: %s\n", &registro[indice]); break; //nome
        case 6: printf("Cidade: %s\n", &registro[indice]); break; //cidade
        case 7: printf("UF: %s\n", &registro[indice]); break; //UF
    }
   free(indicesCampos);  

}

/**
    comparaCampo
    Compara um campo de um registro com uma chave - escolhida pelo usuário.

    PARAMETRO -registro- | vetor de bytes, que contém o registro organizado na 
                           ordem dos campos do arquivo binário
    PARAMETRO -campo- | valor inteiro que representa o campo desejado a ser impresso
    PARAMETRO -chave- | vetor de bytes, que contém a chave que será comparada com o campo escolhido
    RETORNA | 0, caso a chave seja igual ao campo

**/
int comparaCampo(char *registro, int campo, char *chave){
    int tam_chave = strlen(chave);
    char *campo_cpy;
    
    int *indicesCampos = mapeiaRegistro(registro);
    int indice = indicesCampos[campo]; // indice recebe o endereço do primeiro byte do campo escolhido
    free(indicesCampos);

    int tam_campo, campoInt, chaveInt;

    int result;
    //Comparando um campo com a chave
    if (campo == 3){ // caso o campo a ser comparado seja o do ticket

        memcpy(&campoInt, &registro[indice], sizeof(int)); //passando o valor do ticket contido no registro para um tipo int
        chaveInt = atoi(chave); // passando o valor inteiro contido na chave para um tipo int

        //comparando os dois tipos int
        if (campoInt == chaveInt) return 0; 
            else return 1;
    } else { // caso o campo a ser comparado seja qualquer um - com exceção do ticket

        tam_campo = strlen(&registro[indice]); // recebendo o tamanho do campo sem contar o '\0'
        tam_campo++; // adicionando '\0' na conta     
        campo_cpy = (char*)malloc(sizeof(char)*tam_campo);

        //armazenando o campo a ser comparado numa cópia
        memcpy(campo_cpy, &registro[indice], sizeof(char) * tam_campo);
        //transformando todos os bytes do vetor chave e do vetor campo_cpy da caixa baixa para a caixa alta
        stringMaiusculaAcentos(campo_cpy);
        stringMaiusculaAcentos(chave);
        result = strcmp(campo_cpy, chave); // armazenando o resultado da comparação entre a nova chave com a nova cópia do campo

        free(campo_cpy);
        return result;
    }
}
