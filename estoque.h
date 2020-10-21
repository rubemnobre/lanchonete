#include<stdio.h>
#include<stdlib.h>
#ifndef ESTOQUE_H
#define ESTOQUE_H

typedef struct{ // Estrutura que armazena os itens do estoque
    int codigo, quantidade;
    float preco;
    char nome[50];
}estoque;

estoque *ler_lista(char* nome, int *len){ // Função que lê do arquivo e retorna um vetor com o estoque
    estoque *lista;
    FILE *arquivo_estoque = fopen(nome, "rb");
    if(arquivo_estoque == NULL){
        printf("Tentativa de abrir o arquivo %s falhou\n", nome);
        *len = 0;
        return NULL;
    }
    fscanf(arquivo_estoque, "%d\n", len);
    lista = malloc((*len)*sizeof(estoque));
    fread(lista, sizeof(estoque), *len, arquivo_estoque);
    fclose(arquivo_estoque);
    return lista;
}

int compare(const void *a, const void *b){ // Função auxiliar para o qsort() entre dois objetos de estoque
    return (*(estoque*)a).codigo - (*(estoque*)b).codigo;
}

void escrever_lista(char* nome, estoque* lista, int len){ // Recebe um vetor com os itens de estoque e armazena no arquivo especificado
    qsort(lista, len, sizeof(estoque), compare);
    FILE *arquivo_estoque = fopen(nome, "wb");
    fprintf(arquivo_estoque, "%d\n", len);
    fwrite(lista, sizeof(estoque), len, arquivo_estoque);
    fclose(arquivo_estoque);
}

#endif