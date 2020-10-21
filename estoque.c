#include<stdio.h>
#include<stdlib.h>
#include"estoque.h"

int posix = 0;

void limpar_console(){ // Utiliza sequencia de escape para "limpar" o console (requer sistema POSIX)
    if(posix) printf("\e[1;1H\e[2J");
}

void cor_console(int cod){  // Utiliza sequencia de escape para mudar a cor do console (requer sistema POSIX)
    int r, g, b;
    r = (cod >> 16) & 0xFF;
    g = (cod >> 8) & 0xFF;
    b = (cod) & 0xFF;
    
    if(posix) printf("\e[38;2;%d;%d;%dm", r, g, b);
}

char menu(){ // Imprime as opções do menu principal no console e recebe a escolha do usuário
    char opcao;
    printf("1 - Modificar quantidade do item\n2 - Ver estoque\n3 - Adicionar item\n4 - Sair\nDigite a escolha: ");
    scanf(" %c", &opcao);
    return opcao;
}

int ver_estoque(char *nome){ // Imprime todos os itens do estoque e retorna a quantidade
    int i, len;
    estoque *lista = ler_lista(nome, &len);
    if(lista == NULL){
        cor_console(0xffff00);
        printf("Estoque sem nenhum item, utilize a opcao 3 para adicionar algo.\n");
        cor_console(0xffffff);
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

const char help[] = "Opcoes:\n\t-a <arquivo> : Utilizar arquivo de estoque\n\t-p : Habilitar visuais do console (para terminais compatíveis com POSIX, como os do Linux e Mac e o Powershell no Windows)\n\t-h : ver opcoes (esta tela)\n";

int main(int argc, char **argv){ // Controla o fluxo principal do programa
    char *nome = "estoque.bin";
    int novo_nome = 0;

    // Parser dos argumentos da linha de comando
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
        cor_console(0xff0000);
        printf("Utilizando o arquivo '%s' para o estoque (utilize -h para ver opcoes especiais)\n", nome);
        cor_console(0xffffff);
    }

    while(1){
        switch(menu()){
            case '1':
                limpar_console();
                modificar_item(nome);
                break;
            case '2':
                limpar_console();
                ver_estoque(nome);
                break;
            case '3':
                limpar_console();
                adicionar_item(nome);
                break;
            case '4':
                return 0;
        }
    }
    return 0;
}