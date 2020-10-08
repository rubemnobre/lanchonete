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
/*
cachorro quente 1,2
bauru simples 1,3
hamburguer 1,5
suco 2,2
refrigerante 1
*/
#include<stdio.h>

char menu(){
    char opcao;
    printf("1 - Ver Cardapio\n2 - Novo Pedido\n3 - Ver lista de pedidos\nDigite a escolha: ");
    scanf(" %c", &opcao);
    return opcao;
}

void mostrar_cardapio(){
    printf("100\tCachorro Quente\tRS 1,20\n101\tBauru Simples\tRS 1,30\n102\tHamburguer\tRS 2,20\n");
}

void novo_pedido(){

}

int main(char **argv, int argc){
    char opc = menu();
    switch(opc){
        case '1':
            mostrar_cardapio();
            break;
    }
    return 0;
}