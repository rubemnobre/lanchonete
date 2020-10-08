/*
    Projeto de PCE
    Lanchonete
    Features:
        - Menu principal;
        - Cardápio;
        - Fazer pedido;
        - Ver lista;
        - Controle de estoque;
*/

#include<stdio.h>
#define max_itens 10

void clrscr(){
    printf("\e[1;1H\e[2J");
}

char menu(){
    char opcao;
    printf("1 - Ver Cardapio\n2 - Novo Pedido\n3 - Ver lista de pedidos\nDigite a escolha: ");
    scanf(" %c", &opcao);
    clrscr();
    return opcao;
}

void mostrar_cardapio(){
    printf("100\tCachorro Quente\tRS 1,20\n101\tBauru Simples\tRS 1,30\n102\tHamburguer\tRS 2,20\n");
}

typedef struct{
    int codigo, qtd;
    char nome[50];
    char obs[100];
}item;

void novo_pedido(){
    item itens[max_itens];
    int i, j;
    for(i = 0; i < max_itens; i++){
        mostrar_cardapio();
        printf("0 - Sair\nEscolha o item: ");
        scanf("%d", &itens[i].codigo);
        if(itens[i].codigo == 0){
            clrscr();
            break;
        }
        printf("Digite a quantidade: ");
        scanf("%d", &itens[i].qtd);
        printf("Obs: ");
        scanf("%s", &itens[i].obs);
        clrscr();
    }
    printf("Resumo do pedido \n");
    for(j = 0; j < i; j++){
        printf("Item: %d Quantidade: %d Obs: %s\n", itens[j].codigo, itens[j].qtd, itens[j].obs);
    }
    printf("Pedido finalizado!\n");
}

int main(char **argv, int argc){
    char opc = menu();
    switch(opc){
        case '1':
            mostrar_cardapio();
            break;
        case '2':
            novo_pedido();
            break;
    }
    return 0;
}