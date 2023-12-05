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

typedef struct {
    Tabela tabela;
    Coluna coluna;
    char valor[NOME_LIMITE];
} Registro;

Tabela coletarDadosTabela(char nome[]){
    Tabela tabela;
    FILE *arquivo;
    char nomeArquivo[NOME_LIMITE + 4];
    sprintf(nomeArquivo, "%s.txt", nome);

    arquivo = fopen(nomeArquivo, "r");
    if(arquivo == NULL){
        printf("ERRO AO ABRIR O ARQUIVO %s \n", nomeArquivo);
        strcpy(tabela.nome, "");
        tabela.numColunas = 0;
        tabela.colunas = NULL;
        strcpy(tabela.colunaChavePrimaria, "");
        return tabela;
    }

    fscanf(arquivo, "nome: %s\n", tabela.nome);
    fscanf(arquivo, "numeroColunas: %d\n", &tabela.numColunas);
    fscanf(arquivo, "colunaChavePrimaria: %s\n", tabela.colunaChavePrimaria);

    tabela.colunas = malloc(tabela.numColunas * sizeof(Coluna));
    if(tabela.colunas == NULL){
        printf("ERRO AO ALOCAR MEMORIA PARA AS COLUNAS\n");
        exit(1);
    }

    for(int i = 0; i < tabela.numColunas; i++){
        fscanf(arquivo, "nomeColuna: %s\n", tabela.colunas[i].nome);
        fscanf(arquivo, "tipoColuna: %d\n", &tabela.colunas[i].tipo);
    }
    // Precisa de um while (parecido com o de listarTabelas) para coletar os registros

    fclose(arquivo);
    return tabela;
}

int existeTabela(char nome[]){
    FILE *tabelaPrincipal;
    char tabelas[] = "tabelas.txt";
    char linha[NOME_LIMITE];
    
    // Se não existir o arquivo tabelas.txt
    tabelaPrincipal = fopen(tabelas, "r");
    if (tabelaPrincipal == NULL) {
        return 0;
    }

    // Procura o nome da tabela no arquivo de tabelas
    while (fgets(linha, sizeof(linha), tabelaPrincipal) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        if (strcmp(nome, linha) == 0) {
            fclose(tabelaPrincipal);
            return 1;
        }
    }

    fclose(tabelaPrincipal);
    return 0;
}

void criarTabela(Tabela *tabela){
    getchar();
    while(1){
        printf("INFORME O NOME DA TABELA: ");
        fgets(tabela->nome, NOME_LIMITE, stdin);
        tabela->nome[strcspn(tabela->nome, "\n")] = '\0';
        if(existeTabela(tabela->nome)){
            printf("A TABELA %s JA EXISTE!\n", tabela->nome);
        } else {
            break;
        }
    }

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
        fprintf(arquivo, "nomeColuna: %s\n", tabela->colunas[i].nome);
        fprintf(arquivo, "tipoColuna: %d\n", tabela->colunas[i].tipo);
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

void listarTabelas(){
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

void criarRegistro(Tabela *tabela){
    *tabela = coletarDadosTabela(tabela->nome);
    // Falta completar...
}

void listarDadosTabela(Tabela *tabela){
    *tabela = coletarDadosTabela(tabela->nome);

    printf("=== DADOS DA TABELA %s ===\n", tabela->nome);
    // Mostra o nome das colunas da tabela
    printf("| %s |", tabela->colunaChavePrimaria);
    for(int i = 0; i < tabela->numColunas; i++){
        printf(" %s |", tabela->colunas[i].nome);
    }
    printf("\n");
    // Precisa mostrar os registros
}

int main(){
    int op;
    Tabela tabela;
    while(1){
        printf("| 0 - Encerrar | 1 - Nova Tabela | 2 - Listar Tabelas | 3 - Novo registro | 4 - Listar dados de uma tabela |\n");
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
            case 3:
                getchar();
                printf("INFORME O NOME DA TABELA: ");
                fgets(tabela.nome, NOME_LIMITE, stdin);
                tabela.nome[strcspn(tabela.nome, "\n")] = '\0';
                if(existeTabela(tabela.nome)){
                    criarRegistro(&tabela);
                } else {
                    printf("A TABELA %s NAO EXISTE!\n", tabela.nome);
                }
                break;
            case 4:
                getchar();
                printf("INFORME O NOME DA TABELA: ");
                fgets(tabela.nome, NOME_LIMITE, stdin);
                tabela.nome[strcspn(tabela.nome, "\n")] = '\0';
                if(existeTabela(tabela.nome)){
                    listarDadosTabela(&tabela);
                } else {
                    printf("A TABELA %s NAO EXISTE!\n", tabela.nome);
                }
                break;
            default:
                printf("INVALIDO! \n");
                break;
        }
    }
}

