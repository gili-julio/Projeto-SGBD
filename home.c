#include <stdio.h>

typedef struct {
    char nome[50];
    int tipo;
} Coluna;

typedef struct {
    char nome[50];
    int numColunas;
    Coluna colunas[10];
    int chavePrimaria;
} Tabela;

void criarTabela(Tabela *tabela){
    printf("INFORME O NOME DA TABELA \n");
    scanf("%s", tabela->nome);
    
    printf("INFORME O NUMERO DE COLUNAS DA TABELA %s \n", tabela->nome);
    scanf("%d", &(tabela->numColunas));
    
    for(int i = 0; i < tabela->numColunas; i++){
        printf("INFORME O NOME DA COLUNA %d: \n", i+1);
        scanf("%s", tabela->colunas[i].nome);
        
        int posTipo = 0;
        while(posTipo == 0){
            printf("INFORME O TIPO DA COLUNA %d: \n | 1 - char | 2 - int | 3 - float | 4 - double | 5 - string | \n", i+1);
            scanf("%d", &posTipo);
            switch (posTipo){
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    tabela->colunas[i].tipo = posTipo;
                    break;
                default:
                    printf("INVALIDO! \n");
                    posTipo = 0;
                    break;
            }
        }
    }
    
    int posPrimaria = 0;
    while(posPrimaria == 0){
        printf("INFORME QUAL A COLUNA DA CHAVE PRIMARIA (de 1 a %d): \n", tabela->numColunas);
        scanf("%d", &posPrimaria);
        if(posPrimaria >= 1 && posPrimaria <= tabela->numColunas){
            tabela->chavePrimaria = posPrimaria;
        } else {
            printf("INVALIDO! \n");
            posPrimaria = 0;
        }
    }
    
    // Nome do arquivo baseado no nome da tabela
    FILE *arquivo;
    char nomeArquivo[100];
    sprintf(nomeArquivo, "%s.txt", tabela->nome);
    
    // Abre o arquivo para escrita
    arquivo = fopen(nomeArquivo, "w");
    
    // Verifica se o arquivo foi aberto com sucesso
    if(arquivo == NULL){
        printf("ERRO AO ABRIR O ARQUIVO %s PARA ESCRITA \n", nomeArquivo);
        return;
    }
    
    // Escreve os dados da tabela no arquivo
    fprintf(arquivo, "nome: %s\n", tabela->nome);
    fprintf(arquivo, "numeroColunas: %d\n", tabela->numColunas);
    fprintf(arquivo, "chavePrimaria: %d\n", tabela->chavePrimaria);
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
}

int main(){
    int op;
    Tabela tabela;
    while(1){
        printf("===================================\n");
        printf("= 0 - Encerrar | 1 - Criar Tabela =\n");
        printf("===================================\n");
        scanf("%d", &op);
        switch (op){
            case 0:
                printf("==============\n");
                printf("= ENCERRADO! =\n");
                printf("==============\n");
                return 0;
            case 1:
                criarTabela(&tabela);
                break;
            default:
                printf("INVALIDO! \n");
                break;
        }
    }
    return 0;
}
