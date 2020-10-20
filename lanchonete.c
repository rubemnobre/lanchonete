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
#define min_val_desconto 7
#define min_qtd_desconto 3
#define desconto 0.2

int posix = 0;

void limpar_console(){ // Utiliza sequencia de escape para "limpar" o console (requer sistema POSIX)
    if(posix) printf("\e[1;1H\e[2J");
}

void cor_console(int cod){
    int r = cod/0xFFFF, g = (cod%0x010000)/0xFF, b = cod % 0x0100;
    if(posix) printf("\e[38;2;%d;%d;%dm", r, g, b);
}

char menu(){ // Imprime as opções do menu principal no console e retorna a escolha do usuário
    char opcao;
    printf("1 - Ver Cardapio\n2 - Novo Pedido\n3 - Ver lista de pedidos\n4 - Concluir pedido\n5 - Sair\nDigite a escolha: ");
    scanf(" %c", &opcao);
    limpar_console();
    return opcao;
}

void mostrar_cardapio(char *nome){ // Imprime as opções do cardápio
    int i, len;
    estoque *lista = ler_lista(nome, &len);
    if(lista == NULL){
        cor_console(0xff0000);
        printf("Ulilize o programa estoque.exe para criar um arquivo de estoque (adicione algo ao estoque)\n");
        cor_console(0xffffff);
        return;
    }
    printf("Codigo\t|Qtd.\t|Preco\t|Nome\n");
    for(i = 0; i < len; i++){
        if(lista[i].quantidade > 0)
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
        if(lista[i].tipo.preco >= min_val_desconto && lista[i].qtd >= min_qtd_desconto){
            total += lista[i].tipo.preco * lista[i].qtd * (1-desconto);
        }else{
            total += lista[i].tipo.preco * lista[i].qtd;
        }
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
            limpar_console();
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
            cor_console(0xff0000);
            printf("Codigo nao encontrado\n");
            cor_console(0xffffff);
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
        limpar_console();
    }

    // Finaliza a função se nenhum item foi adicionado
    if(i == 0){
        return;
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
    cor_console(0xffff00);
    printf("Valor total: RS%.2f\n", valor_total);
    cor_console(0xffffff);

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
            float pago = 0;
            do{
                printf("Quanto foi pago: ");
                scanf("%f", &pago);
            }while(pago <= valor_total);
            printf("Troco: RS%.2f\n", pago - valor_total);
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
        limpar_console();
        printf("|Qtd.\t|Unid\t|Tot.\t|Nome\n");
        i = 0;
        while(pedidos[*n_pedidos][i].qtd != -1){
            float valor;
            if(pedidos[*n_pedidos][i].tipo.preco >= min_val_desconto && pedidos[*n_pedidos][i].qtd >= min_qtd_desconto){
                valor = pedidos[*n_pedidos][i].tipo.preco * pedidos[*n_pedidos][i].qtd * (1-desconto);
            }else{
                valor = pedidos[*n_pedidos][i].tipo.preco * pedidos[*n_pedidos][i].qtd;
            }
            printf("|%d\t|%.2f\t|%.2f\t|%s\n", pedidos[*n_pedidos][i].qtd, pedidos[*n_pedidos][i].tipo.preco, valor, pedidos[*n_pedidos][i].tipo.nome);
            i++;
        }
        printf("Valor total: RS%.2f\n", valor_total);
        printf("Senha do pedido: %d\n", *n_pedidos);
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
    if(*n_pedidos == 0){
        printf("Nenhum pedido em aberto.\n");
        return;
    }
    int i, j;
    for(i = 0; i < *n_pedidos; i++){
        j = 0;
        printf("Pedido %d\n", i);
        while(pedidos[i][j].qtd != -1){
            printf("  %dx %s (%s)\n", pedidos[i][j].qtd, pedidos[i][j].tipo.nome, pedidos[i][j].obs);
            j++;
        }
    }
}

void concluir_pedido(item **pedidos, int *n_pedidos){ // Remove o item escolhido da lista de pedidos, deslocando os posteriores para preencher
    ver_pedidos(pedidos, n_pedidos);
    if(*n_pedidos == 0){
        return;
    }
    int i, n;
    do{
        printf("Digite o numero do pedido: ");
        scanf("%d", &n);
    }while(n >= *n_pedidos || n < 0);

    for(i = n; i < *n_pedidos; i++){
        pedidos[i] = pedidos[i+1];
    }
    pedidos[*n_pedidos] = NULL;
    (*n_pedidos) -= 1;
    limpar_console();
}

const char help[] = "Opcoes:\n\t-a <arquivo> : Utilizar arquivo de estoque\n\t-p : Habilitar visuais do console (para terminais compatíveis com POSIX, como os do Linux e Mac e o Powershell no Windows)\n\t-h : ver opcoes (esta tela)\n";

int main(int argc, char **argv){ // Controla o fluxo principal do programa
    char *nome = "estoque.bin";
    int novo_nome = 0;
    if(argc != 1){
        int i;
        for(i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                switch(argv[i][1]){
                    case 'p':
                        posix = 1;
                        break;
                    case 'a':
                        if(argc > i){
                            if(argv[i+1][0] != '-'){
                                nome = &argv[i+1][0];
                                i++;
                                novo_nome = 1;
                                break;
                            }
                        }
                        printf("Argumento invalido\n%s", help);
                        return 1;
                    case 'h':
                        printf("%s", help);
                        return 0;
                    default:
                        printf("Argumento invalido\n%s", help);
                        return 1;
                }
            }
        }
    }

    if(!novo_nome){
        cor_console(0xffff00);
        printf("Utilizando o arquivo '%s' para o estoque (utilize -h para ver opcoes especiais)\n", nome);
        cor_console(0xffffff);
    }

    // Inicializa a lista de pedidos com ponteiros nulos (para liberar a memória com segurança no final)
    int n_pedidos = 0, i;
    item *pedidos[max_pedidos];
    for(i = 0; i < max_pedidos; i++){
        pedidos[i] = NULL;
    }
    
    while(1){
        switch(menu()){
            case '1':
                mostrar_cardapio(nome);
                break;
            case '2':
                novo_pedido(nome, pedidos, &n_pedidos);
                break;
            case '3':
                ver_pedidos(pedidos, &n_pedidos);
                break;
            case '4':
                concluir_pedido(pedidos, &n_pedidos);
                break;
            case '5':
                for(i = 0; i < max_pedidos; i++){ // Coleta de memória
                    free(pedidos[i]);
                }
                return 0;
        }
    }
}
