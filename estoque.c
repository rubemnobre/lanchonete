#include<stdio.h>
#include<stdlib.h>
#include"estoque.h"

void clrscr(){ // Utiliza sequencia de escape para "limpar" o console (requer sistema POSIX)
    printf("\e[1;1H\e[2J");
}

char menu(){ // Imprime as opções do menu principal no console e recebe a escolha do usuário
    char opcao;
    printf("1 - Modificar item\n2 - Ver estoque\n3 - Adicionar item\n0 - Sair\nDigite a escolha: ");
    scanf(" %c", &opcao);
    return opcao;
}

int ver_estoque(char *nome){ // Imprime todos os itens do estoque e retorna a quantidade
    int i, len;
    estoque *lista = ler_lista(nome, &len);
    if(lista == NULL){
        printf("Estoque sem nenhum item, utilize a opcao 3 para adicionar algo.\n");
        return 0;
    }
    printf("Indice\t|Codigo\t|Qtd.\t|Preco\t|Nome\n");
    for(i = 0; i < len; i++){
        printf("%d\t|%d\t|%d\t|%.2f\t|%s\n", i, lista[i].codigo, lista[i].quantidade, lista[i].preco, lista[i].nome);
    }
    return len;
}

void ler_item(estoque *item){ // Recebe os dados do usuário e preenche o item apontado no argumento
    printf("Digite o codigo: ");
    scanf("%d", &(item->codigo));
    printf("Digite o nome: ");
    fflush(stdin);
    scanf("%49[^\n]", &(item->nome));
    printf("Digite a quantidade: ");
    scanf("%d", &(item->quantidade));
    printf("Digite o preco: ");
    scanf("%f", &(item->preco));
}

int modificar_item(char *nome){ // Seleciona um item do estoque e atualiza sua quantidade a partir de entrada do usuario
    int i, len;
    estoque *lista = ler_lista(nome, &len);
    if(lista == NULL){
        printf("Estoque vazio! Adicone um item para modificar\n");
        return -1;
    }
    len = ver_estoque(nome);
    printf("Escolher indice: ");
    scanf("%d", &i);
    printf("Digite a nova quantidade: ");
    int n;
    scanf("%d", &n);
    lista[i].quantidade = n;
    escrever_lista(nome, lista, len);
    return len;
}

void adicionar_item(char *nome){ // Adiciona item à lista do estoque
    int i, len;
    estoque *lista = ler_lista(nome, &len);
    if(lista == NULL){
        len = 0;
        printf("Criando o arquivo %s\n", nome);
    }
    estoque nova_lista[len+1];
    estoque novo;
    ler_item(&novo);
    for(i = 0; i < len; i++){
        nova_lista[i] = lista[i];
    }
    nova_lista[len] = novo;
    escrever_lista(nome, nova_lista, len+1);
}

int main(int argc, char **argv){ // Controla o fluxo principal do programa
    char *nome = "estoque.bin";
    if(argc != 2){
        printf("Utilizando o arquivo '%s' por padrao\nPara utilizar um arquivo diferente, use o nome como argumento.\n", nome);
    }else{
        nome = &argv[1][0];
    }

    while(1){
        switch(menu()){
            case '1':
                modificar_item(nome);
                break;
            case '2':
                ver_estoque(nome);
                break;
            case '3':
                adicionar_item(nome);
                break;
            case '0':
                return 0;
        }
    }
    return 0;
}