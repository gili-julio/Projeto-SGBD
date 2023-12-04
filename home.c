#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME_LIMITE 50 // Definindo o limite máximo dos nomes para 50

typedef struct {
    char nome[NOME_LIMITE];
    int tipo;
} Coluna;

typedef struct {
    char nome[NOME_LIMITE];
    int numColunas;
    Coluna *colunas;
    char colunaChavePrimaria[NOME_LIMITE];
} Tabela;

void criarTabela(Tabela *tabela){
    getchar();
    printf("INFORME O NOME DA TABELA: ");
    fgets(tabela->nome, NOME_LIMITE, stdin);
    tabela->nome[strcspn(tabela->nome, "\n")] = '\0';

    printf("INFORME O NOME DA COLUNA DA CHAVE PRIMARIA: ");
    fgets(tabela->colunaChavePrimaria, NOME_LIMITE, stdin);
    tabela->colunaChavePrimaria[strcspn(tabela->colunaChavePrimaria, "\n")] = '\0';
    
    // Criação de colunas da tabela
    int op;
    tabela->numColunas = 0;
    tabela->colunas = NULL;
    while(op != 0){
        printf("| 0 - Finalizar tabela | 1 - Adicionar coluna |\nINFORME O QUE DESEJA: ");
        scanf("%d", &op);
        getchar();
        switch(op){
            case 0:
                break;
            case 1:

                // Alocação de memória para as colunas
                tabela->colunas = realloc(tabela->colunas, (tabela->numColunas+1) * sizeof(Coluna));
                if (tabela->colunas == NULL) {
                    printf("ERRO AO ALOCAR MEMORIA PARA AS COLUNAS");
                    exit(1);
                }

                printf("INFORME O NOME DA COLUNA: ");
                fgets(tabela->colunas[tabela->numColunas].nome, NOME_LIMITE, stdin);
                tabela->colunas[tabela->numColunas].nome[strcspn(tabela->colunas[tabela->numColunas].nome, "\n")] = '\0';

                int posTipo = 0;
                while(posTipo == 0){
                    printf("| 1 - char | 2 - int | 3 - float | 4 - double | 5 - string |\nINFORME O TIPO DA COLUNA: ");
                    scanf("%d", &posTipo);
                    getchar();
                    switch (posTipo){
                        case 1:
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                            tabela->colunas[tabela->numColunas].tipo = posTipo;
                            break;
                        default:
                            printf("INVALIDO!\n");
                            posTipo = 0;
                            break;
                    }
                }
                tabela->numColunas++;

                break;
            default:
                printf("INVALIDO!\n");
                break;
        }
    }
    
    // Nome do arquivo baseado no nome da tabela
    FILE *arquivo;
    char nomeArquivo[NOME_LIMITE+4];
    sprintf(nomeArquivo, "%s.txt", tabela->nome);
    
    // Abre o arquivo para escrita
    arquivo = fopen(nomeArquivo, "w");
    
    // Verifica se o arquivo foi aberto com sucesso
    if(arquivo == NULL){
        printf("ERRO AO ABRIR O ARQUIVO %s PARA ESCRITA", nomeArquivo);
        return;
    }
    
    // Escreve os dados da tabela no arquivo
    fprintf(arquivo, "nome: %s\n", tabela->nome);
    fprintf(arquivo, "numeroColunas: %d\n", tabela->numColunas);
    fprintf(arquivo, "colunaChavePrimaria: %s\n", tabela->colunaChavePrimaria);
    for(int i = 0; i < tabela->numColunas; i++){
        fprintf(arquivo, "nomeColuna %d: %s\n", i+1, tabela->colunas[i].nome);
        fprintf(arquivo, "tipoColuna %d: %d\n", i+1, tabela->colunas[i].tipo);
    }
    fclose(arquivo);


    FILE *tabelaPrincipal;
    char nome[] = "tabelas.txt";
    // Verifica se o arquivo existe
    tabelaPrincipal = fopen(nome, "r");
    if (tabelaPrincipal == NULL){
        // Se o arquivo não existir, cria um novo
        tabelaPrincipal = fopen(nome, "w");
        if(tabelaPrincipal == NULL){
            printf("ERRO AO CRIAR 'tabelas.txt' \n");
            return;
        }
    } else {
        // Se o arquivo existir, abre para adicionar conteúdo no final
        tabelaPrincipal = fopen(nome, "a");
        if(tabelaPrincipal == NULL){
            printf("ERRO AO ABRIR 'tabelas.txt' \n");
            return;
        }
    }
    
    // Adiciona o nome da tabela ao final da tabela principal
    fprintf(tabelaPrincipal, "%s\n", tabela->nome);
    fclose(tabelaPrincipal);


    printf("==============================\n");
    printf("= TABELA CRIADA COM SUCESSO! =\n");
    printf("==============================\n");

    // Liberar memória alocada
    free(tabela->colunas);
}

void listarTabelas(){\
    FILE *tabelaPrincipal;
    char nome[] = "tabelas.txt";
    char nomeTabela[NOME_LIMITE];

    tabelaPrincipal = fopen(nome, "r");
    if (tabelaPrincipal == NULL) {
        printf("NAO FORAM ENCONTRADAS TABELAS \n");
        return;
    }

    printf("=== LISTA DE TABELAS ===\n");
    while (fgets(nomeTabela, sizeof(nomeTabela), tabelaPrincipal) != NULL) {
        nomeTabela[strcspn(nomeTabela, "\n")] = '\0';
        printf("%s\n", nomeTabela);
    }

    fclose(tabelaPrincipal);
}

int main() {
    int op;
    Tabela tabela;
    while (1) {
        printf("========================================================\n");
        printf("| 0 - Encerrar | 1 - Criar Tabela | 2 - Listar Tabelas |\n");
        printf("========================================================\n");
        scanf("%d", &op);
        switch (op) {
            case 0:
                return 0;
            case 1:
                criarTabela(&tabela);
                break;
            case 2:
                listarTabelas();
                break;
            default:
                printf("INVALIDO! \n");
                break;
        }
    }
    return 0;
}

