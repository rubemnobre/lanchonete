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

char menu(){
    printf("1 - Ver Cardapio\n2 - Novo Pedido\n3 - Ver lista de pedidos\nDigite a escolha: ");
    return getchar();
}

int main(char **argv, int argc){
    char opc = menu();
    return 0;
}