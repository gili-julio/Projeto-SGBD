#include "criar.h"

void criarTabela(Tabela *tabela){
    getchar();
    while(1){
        printf("INFORME O NOME DA TABELA: ");
        fgets(tabela->nome, NOME_LIMITE, stdin);
        tabela->nome[strcspn(tabela->nome, "\n")] = '\0';
        // Converter para minúsculas
        for (int i = 0; tabela->nome[i]; i++) {
            tabela->nome[i] = tolower(tabela->nome[i]);
        }
        //Verifica se o nome da tabela já existe
        if(existeTabela(tabela->nome)){
            printf("A TABELA %s JA EXISTE!\n", tabela->nome);
        } else if(strcmp(tabela->nome, "tabelas") == 0){
            printf("O NOME %s NAO PODE SER UTILIZADO!\n", tabela->nome);
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
    sprintf(nomeArquivo, "tabelas/%s.txt", tabela->nome);
    
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
    char nome[] = "tabelas/tabelas.txt";
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

void criarRegistro(Tabela *tabela){
    //Verifica se existe a tabela informada
    fflush(stdin);
    printf("INFORME O NOME DA TABELA: ");
    fgets(tabela->nome, NOME_LIMITE, stdin);
    tabela->nome[strcspn(tabela->nome, "\n")] = '\0';
    if(!existeTabela(tabela->nome)){
        printf("A TABELA %s NAO EXISTE!\n", tabela->nome);
        return;
    }

    *tabela = coletarDadosTabela(tabela->nome);

    FILE *arquivo;
    char nomeArquivo[NOME_LIMITE + 4];
    sprintf(nomeArquivo, "tabelas/%s.txt", tabela->nome);
    arquivo = fopen(nomeArquivo, "a");
    if(arquivo == NULL){
        printf("ERRO AO ABRIR O ARQUIVO %s \n", nomeArquivo);
        return;
    }

    // Solicita o valor de cada coluna
    for(int i = 0; i < tabela->numColunas+1; i++){
        if(i == 0){ // Chave primária
            int registro;
            printf("INFORME O VALOR DA COLUNA %s: ", tabela->colunaChavePrimaria);
            scanf("%d", &registro);
            if(registro >= 0 && existeChavePrimaria(tabela, registro) == 0){
                fprintf(arquivo, "%s: %d\n", tabela->colunaChavePrimaria, registro);
            } else {
                printf("VALOR INVALIDO!\n");
                break;
                return;
            }
        } else {
            printf("INFORME O VALOR DA COLUNA %s: ", tabela->colunas[i-1].nome);
            char registroChar;
            int registroInt;
            float registroFloat;
            double registroDouble;
            char registroString[NOME_LIMITE];
            switch(tabela->colunas[i-1].tipo){
                case 1: // Caso char
                    scanf("%c", &registroChar);
                    fprintf(arquivo, "%s: %c\n", tabela->colunas[i-1].nome, registroChar);
                    break;
                case 2: // Caso int
                    scanf("%d", &registroInt);
                    fprintf(arquivo, "%s: %d\n", tabela->colunas[i-1].nome, registroInt);
                    break;
                case 3: // Caso float
                    scanf("%f", &registroFloat);
                    fprintf(arquivo, "%s: %.2f\n", tabela->colunas[i-1].nome, registroFloat);
                    break;
                case 4: // Caso double
                    scanf("%lf", &registroDouble);
                    fprintf(arquivo, "%s: %f\n", tabela->colunas[i-1].nome, registroDouble);
                    break;
                case 5: // Caso string
                    fgets(registroString, NOME_LIMITE, stdin);
                    registroString[strcspn(registroString, "\n")] = '\0';
                    fprintf(arquivo, "%s: %s\n", tabela->colunas[i-1].nome, registroString);
                    break;
            }
        }
        fflush(stdin);
    }

    printf("REGISTRO FEITO!\n");
    fclose(arquivo);
}
