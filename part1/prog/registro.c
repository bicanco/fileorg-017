/**
    registro
    Funções que manipulam um registro unico e seus campos, como
    criacao de registro a partir de dados de entrada, impressao
    de um registro e busca e comparacao de campos em um registro.
**/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    int sequenciaEntradas[] = {1,5,6,7,0,2,3,4};

    char *campo;  
    int contadorCampos = 0;
    char *registro;    
    
    // calculando tamanho do registro
    int tamDominio = strlen(entradas[0]) + 1;  
    int tamNome = strlen(entradas[2]) + 1; 
    int tamCidade = strlen(entradas[3]) + 1;   
    int tamUF =  strlen(entradas[4]) + 1;  
    *tamanhoRegistro = 20+20+20+4+16+tamNome+tamDominio+tamCidade+tamUF;

    // alocando memoria para guardar temporariamente o registro
    registro = (char *) malloc(sizeof(char)*(*tamanhoRegistro));  
    int indice = 0;
    int tam, campoInt;
    
    // para cada um dos campos, na ordem de serem colocados no registro
    for (contadorCampos = 0; contadorCampos < 8; contadorCampos++){

        // pegue o registro certo, de acordo com a sequencia     
        campo = entradas[sequenciaEntradas[contadorCampos]];
        
        // pegue o tamanho do registro correto, de acordo com a sequencia
        switch (sequenciaEntradas[contadorCampos]){
            case 0: tam = tamDominio; break; // dominio
            case 1: tam = 20; break; // documento
            case 2: tam = tamNome; break; // nome
            case 3: tam = tamCidade; break; // cidade
            case 4: tam = tamUF; break; // uf
            case 5: case 6: tam = 20; break; // dataHoraCadastro, dataHoraAtualiza
            case 7: tam = sizeof(int); break; // ticket
        }
        
        if (sequenciaEntradas[contadorCampos] == 7){ // colocar um inteiro no registro
            // colocando no formato de inteiro
            campoInt = atoi(campo);
            
            // escrever inteiro no registro
            memcpy(&registro[indice], &campoInt, tam);

        } else { // colocar uma string no registro
            if (contadorCampos >= 4){
            	// o campo eh de tamanho variavel, colocar indicador de tamanho
                memcpy(&registro[indice], &tam, sizeof(int));
            	indice += sizeof(int);
            }
            
            // escrever string no registro
            memcpy(&registro[indice], campo, tam);
        }
        
        // atualizar indice percorrido no registro
        indice += tam;      
    }

    return registro;
}

int *mapeiaRegistro(char *registro){
    int *mapaIndices = (int *) malloc(sizeof(int) * 8);
    int i, indiceAux, tamanhoCampo;

    mapaIndices[0] = 0;
    mapaIndices[1] = 20;
    mapaIndices[2] = 40;
    mapaIndices[3] = 60;
    mapaIndices[4] = 68;
    indiceAux = 64;

    for (i = 5; i < 8; i++){
        memcpy(&tamanhoCampo, &registro[indiceAux], sizeof(int));
        indiceAux += sizeof(int) + tamanhoCampo;
        mapaIndices[i] = indiceAux + sizeof(int);
    }

    return mapaIndices;
}

void imprimeRegistro(char *registro){
    int sequenciaImpressao[] = {4, 3, 1, 2, 5, 6, 7, 0};
    int contadorCampos, campoInt;

    int *indicesCampos = mapeiaRegistro(registro);
    int indice;

    for (contadorCampos = 0; contadorCampos < 8; contadorCampos++){
        indice = indicesCampos[sequenciaImpressao[contadorCampos]];

        // imprimindo o campo certo
        switch (sequenciaImpressao[contadorCampos]){
            case 0: printf("CNPJ: %s\n\n\n", &registro[indice]); break;
            case 1: printf("Cadastro feito em %s\n", &registro[indice]); break;
            case 2: printf("Última atualização em %s\n", &registro[indice]); break;
            case 3:
                memcpy(&campoInt, &registro[indice], sizeof(int));
                printf("(ticket %d)\n", campoInt);
                break;
            case 4: printf("DOMÍNIO: %s ", &registro[indice]); break;
            case 5: printf("\nÓRGÃO/ENTIDADE:\n%s\n", &registro[indice]); break;
            case 6: printf("%s ", &registro[indice]); break;
            case 7: printf("(%s)\n", &registro[indice]); break;
        }
    }

    free(indicesCampos);
}

void imprimeCampo(char *registro, int campo){
   
    int *indicesCampos = mapeiaRegistro(registro);
    int indice =  indicesCampos[campo];
    int campoInt;

    switch (indice){
            case 0: printf("Documento: %s\n\n\n", &registro[indice]); break;
            case 1: printf("Cadastro feito em %s\n", &registro[indice]); break;
            case 2: printf("Última atualização em %s\n", &registro[indice]); break;
            case 3:
                memcpy(&campoInt, &registro[indice], sizeof(int));
                printf("(ticket %d)\n", campoInt);
                break;
            case 4: printf("Domínio: %s ", &registro[indice]); break;
            case 5: printf("\nÓrgão/Entidade:\n%s\n", &registro[indice]); break;
            case 6: printf("Cidade: %s ", &registro[indice]); break;
            case 7: printf("UF: %s\n", &registro[indice]); break;
        }
   free(indicesCampos);  

}
