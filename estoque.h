#include<stdio.h>
#include<stdlib.h>
#include"estoque_fonte.c"
#ifndef ESTOQUE_H
#define ESTOQUE_H

estoque *ler_lista(char* nome, int *len); // Função que lê do arquivo e retorna um vetor com o estoque

void escrever_lista(char* nome, estoque* lista, int len); // Recebe um vetor com os itens de estoque e armazena no arquivo especificado

#endif