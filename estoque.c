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

int ver_estoque(char *nome){
    int i, len;
    estoque *lista = ler_lista(nome, &len);
    printf("Indice\t|Codigo\t|Qtd.\t|Preco\t|Nome\n");
    for(i = 0; i < len; i++){
        printf("%d\t|%d\t|%d\t|%.2f\t|%s\n", i, lista[i].codigo, lista[i].quantidade, lista[i].preco, lista[i].nome);
    }
    return len;
}

void ler_item(estoque *item){
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

int modificar_item(char *nome){
    int i, len;
    estoque *lista = ler_lista(nome, &len);
    if(lista == NULL){
        printf("Estoque vazio! Adicone um item para modificar\n");
        return -1;
    }
    len = ver_estoque(nome);
    printf("Escolher indice: ");
    scanf("%d", &i);
    ler_item(&lista[i]);
    escrever_lista(nome, lista, len);
    return len;
}

void adicionar_item(char *nome){
    int i, len;
    estoque *lista = ler_lista(nome, &len);
    if(lista == NULL){
        len = 0;
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

int main(int argc, char **argv){
    if(argc != 2){
        printf("Uso: \n\testoque.exe nome_do_arquivo");
        return 1;
    }
    while(1){
        switch(menu()){
            case '1':
                modificar_item(argv[1]);
                break;
            case '2':
                ver_estoque(argv[1]);
                break;
            case '3':
                adicionar_item(argv[1]);
                break;
            case '0':
                return 0;
        }
    }
    return 0;
}