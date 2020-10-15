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
#define max_pedidos 50

void clrscr(){ // Utiliza sequencia de escape para "limpar" o console (requer sistema POSIX)
    printf("\e[1;1H\e[2J");
}

char menu(){ // Imprime as opções do menu principal no console e retorna a escolha do usuário
    char opcao;
    printf("1 - Ver Cardapio\n2 - Novo Pedido\n3 - Ver lista de pedidos\n4 - Sair\nDigite a escolha: ");
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

typedef struct{ // Tipo de struct para conter um item em um pedido
    estoque tipo;
    int qtd;
    char obs[100];
}item;

float calcular_preco(item *lista){ // Retorna o preço de uma lista de itens (um pedido)
    int i = 0;
    float total = 0;
    while(lista[i].qtd != -1){
        total += lista[i].tipo.preco * lista[i].qtd;
        i++;
    }
    return total;
}

void atualizar_estoque(char *nome, item pedido, int mult){ // Atualiza a quantidade no estoque do item
    int j, len;
    estoque *lista_estoque = ler_lista(nome, &len);
    for(j = 0; j < len; j++){
        if(lista_estoque[j].codigo == pedido.tipo.codigo){
            lista_estoque[j].quantidade += mult*pedido.qtd;
        }
    }
    escrever_lista(nome, lista_estoque, len);
    free(lista_estoque);
}

void novo_pedido(char *nome, item **pedidos, int *n_pedidos){ // Procedimento interativo que adiciona novo pedido a lista
    item itens[max_itens];
    int i, j;

    // Receber item a item do pedido
    for(i = 0; i < max_itens; i++){
        mostrar_cardapio(nome);
        printf("\n0 - Finalizar\nEscolha o item: ");
        int codigo;
        scanf("%d", &codigo);
        if(codigo == 0){
            clrscr();
            break;
        }

        // Procura codigo digitado dentre os existentes no estoque
        int len, found = 0;
        estoque *lista = ler_lista(nome, &len);
        for(j = 0; j < len; j++){
            if(codigo == lista[j].codigo){
                itens[i].tipo = lista[j];
                found = 1;
            }
        }
        free(lista);
        if(found == 0){
            printf("Codigo nao encontrado\n");
            break;
        }

        // Recebe a quantidade solicitada, repete se a quantidade for inválida
        do{
            printf("Digite a quantidade (maximo %d): ", itens[i].tipo.quantidade);
            scanf("%d", &itens[i].qtd);
        }while(itens[i].qtd > itens[i].tipo.quantidade || itens[i].qtd < 0);

        // Recebe observação do item
        printf("Obs: ");
        fflush(stdin);
        scanf("%99[^\n]s", &itens[i].obs);
        atualizar_estoque(nome, itens[i], -1);
        clrscr();
    }

    // Imprimir resumo do pedido
    pedidos[*n_pedidos] = malloc(sizeof(item)*(i+1));
    printf("Resumo do pedido \n");
    for(j = 0; j < i; j++){
        pedidos[*n_pedidos][j] = itens[j];
        printf("%dx %s\n", pedidos[*n_pedidos][j].qtd, pedidos[*n_pedidos][j].tipo.nome);
    }
    pedidos[*n_pedidos][j].qtd = -1;
    float valor_total = calcular_preco(pedidos[*n_pedidos]);
    printf("Valor total: RS%.2f\n", valor_total);

    // Confirmação e conclusão do pedido
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
        printf("Valor total: RS%.2f\n", valor_total);
        printf("Senha: %d\n", *n_pedidos);
        printf("Pedido finalizado!\n");
        (*n_pedidos) += 1;
    }else{
        for(j = 0; j < i; j++){
            atualizar_estoque(nome, itens[j], +1);
        }
        printf("Pedido cancelado!\n");
    }
}

void ver_pedidos(item **pedidos, int *n_pedidos){ // Imprime os pedidos da lista
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

int main(int argc, char **argv){ // Controla o fluxo principal do programa
    if(argc != 2){
        printf("Uso: \nlanchonete.exe nome_do_arquivo");
        return 1;
    }

    // Inicializa a lista de pedidos com ponteiros nulos (para liberar a memória com segurança no final)
    int n_pedidos = 0, i;
    item *pedidos[max_pedidos];
    for(i = 0; i < max_pedidos; i++){
        pedidos[i] = NULL;
    }
    printf("Teste");

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
            case '4':
                for(i = 0; i < max_pedidos; i++){ // Coleta de memória
                    free(pedidos[i]);
                }
                return 0;
        }
    }
}
