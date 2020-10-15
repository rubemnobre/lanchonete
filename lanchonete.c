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
#include<string.h>
#include"estoque.h"
#define max_itens 10

void clrscr(){ // Utiliza sequencia de escape para "limpar" o console (requer sistema POSIX)
    printf("\e[1;1H\e[2J");
}

char menu(){ // Imprime as opções do menu principal no console e recebe a escolha do usuário
    char opcao;
    printf("1 - Ver Cardapio\n2 - Novo Pedido\n3 - Ver lista de pedidos\nDigite a escolha: ");
    scanf(" %c", &opcao);
    clrscr();
    return opcao;
}

void mostrar_cardapio(char *nome){ // Imprime as opções do cardápio
    int i, len;
    estoque *lista = ler_lista(nome, &len);
    printf("Codigo\t|Qtd.\t|Preco\t|Nome\n");
    for(i = 0; i < len; i++){
        printf("%d\t|%d\t|%.2f\t|%s\n", lista[i].codigo, lista[i].quantidade, lista[i].preco, lista[i].nome);
    }
}

typedef struct{
    estoque tipo;
    int qtd;
    char obs[100];
}item;

float calcular_preco(item *lista){
    int i = 0;
    float total = 0;
    while(lista[i].qtd != -1){
        total += lista[i].tipo.preco * lista[i].qtd;
        i++;
    }
    return total;
}

void atualizar_estoque(char *nome, item *pedido){
    int i = 0, j, len;
    estoque *lista_estoque = ler_lista(nome, &len);
    while(pedido[i].qtd != -1){
        for(j = 0; j < len; j++){
            if(lista_estoque[j].codigo == pedido[i].tipo.codigo){
                lista_estoque[j].quantidade -= pedido[i].qtd;
            }
        }
        i++;
    }
    escrever_lista(nome, lista_estoque, len);
    free(lista_estoque);
}

void novo_pedido(char *nome, item **pedidos, int *n_pedidos){
    item itens[max_itens];
    int i, j;
    for(i = 0; i < max_itens; i++){
        mostrar_cardapio(nome);
        printf("\n0 - Finalizar\nEscolha o item: ");
        int codigo;
        scanf("%d", &codigo);
        if(codigo == 0){
            clrscr();
            break;
        }
        int len, found = 0;
        estoque *lista = ler_lista(nome, &len);
        for(j = 0; j < len; j++){
            if(codigo == lista[j].codigo){
                itens[i].tipo = lista[j];
                found = 1;
            }
        }
        if(found == 0){
            printf("Codigo nao encontrado\n");
            break;
        }
        do{
            printf("Digite a quantidade (maximo %d): ", itens[i].tipo.quantidade);
            scanf("%d", &itens[i].qtd);
        }while(itens[i].qtd > itens[i].tipo.quantidade || itens[i].qtd < 0);
        printf("Obs: ");
        fflush(stdin);
        scanf("%99[^\n]s", &itens[i].obs);
        clrscr();
    }
    pedidos[*n_pedidos] = malloc(sizeof(item)*(i+1));
    printf("Resumo do pedido \n");
    for(j = 0; j < i; j++){
        pedidos[*n_pedidos][j] = itens[j];
        printf("%dx %s\n", pedidos[*n_pedidos][j].qtd, pedidos[*n_pedidos][j].tipo.nome);
    }
    pedidos[*n_pedidos][j].qtd = -1;
    float valor_total = calcular_preco(pedidos[*n_pedidos]);
    printf("Valor total: RS%.2f\n", valor_total);
    char concluir;
    printf("Confirmar pedido? (s/n) ");
    fflush(stdin);
    scanf("%c", &concluir);
    if(concluir == 's'){
        printf("Opcoes de pagamento:\n\t1 - Dinheiro\n\t2 - Cartao\n");
        do{
            printf("Escolha: ");
            fflush(stdin);
            scanf("%c", &concluir);
        }while(concluir != '1' && concluir != '2');
        if(concluir == '1'){
            printf("Quanto foi pago: ");
            float pago = 0;
            do{
                scanf("%f", &pago);
                printf("Troco: RS%.2f\n", pago - valor_total);
            }while(pago <= valor_total);
            printf("Doar troco? (s/n) ");
            fflush(stdin);
            scanf("%c", &concluir);
            printf("Para viagem? (s/n) ");
            fflush(stdin);
            scanf("%c", &concluir);
        }
        if(concluir == '2'){
            printf("Aguardando senha...\n");
        }
        printf("|Qtd.\t|Unid\t|Tot.\t|Nome\n");
        i = 0;
        while(pedidos[*n_pedidos][i].qtd != -1){
            printf("|%d\t|%.2f\t|%.2f\t|%s\n", pedidos[*n_pedidos][i].qtd, pedidos[*n_pedidos][i].tipo.preco, pedidos[*n_pedidos][i].tipo.preco * pedidos[*n_pedidos][i].qtd, pedidos[*n_pedidos][i].tipo.nome);
            i++;
        }
        printf("\nSenha: %d\n", *n_pedidos);
        atualizar_estoque(nome, pedidos[*n_pedidos]);
        printf("Pedido finalizado!\n");
        (*n_pedidos) += 1;
    }else{
        printf("Pedido cancelado!\n");
    }
}

void ver_pedidos(item **pedidos, int *n_pedidos){
    int i, j;
    for(i = 0; i < *n_pedidos; i++){
        j = 0;
        printf("Pedido %d\n", i);
        while(pedidos[i][j].qtd != -1){
            printf("  %dx %s\n", pedidos[i][j].qtd, pedidos[i][j].tipo.nome);
            j++;
        }
    }
}

int main(int argc, char **argv){
    if(argc != 2){
        printf("Uso: \nlanchonete.exe nome_do_arquivo");
        return 1;
    }
    int n_pedidos = 0;
    item *pedidos[50];
    while(1){
        switch(menu()){
            case '1':
                mostrar_cardapio(argv[1]);
                break;
            case '2':
                novo_pedido(argv[1], pedidos, &n_pedidos);
                break;
            case '3':
                ver_pedidos(pedidos, &n_pedidos);
                break;
        }
    }
}