#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

Tabela coletarDadosTabela(char nome[]){
    Tabela tabela;
    FILE *arquivo;
    char nomeArquivo[NOME_LIMITE + 4];
    sprintf(nomeArquivo, "%s.txt", nome);

    arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO %s \n", nomeArquivo);
        strcpy(tabela.nome, "");
        tabela.numColunas = 0;
        tabela.colunas = NULL;
        strcpy(tabela.colunaChavePrimaria, "");
        return tabela;
    }

    // Leitura dos dados do arquivo
    fscanf(arquivo, "nome: %s\n", tabela.nome);
    fscanf(arquivo, "numeroColunas: %d\n", &(tabela.numColunas));
    fscanf(arquivo, "colunaChavePrimaria: %s\n", tabela.colunaChavePrimaria);
    tabela.colunas = malloc(tabela.numColunas * sizeof(Coluna));
    if (tabela.colunas == NULL) {
        printf("ERRO AO ALOCAR MEMORIA PARA AS COLUNAS\n");
        exit(1);
    }
    for (int i = 0; i < tabela.numColunas; i++) {
        fscanf(arquivo, "nomeColuna: %s\n", tabela.colunas[i].nome);
        fscanf(arquivo, "tipoColuna: %d\n", &tabela.colunas[i].tipo);
    }

    fclose(arquivo);
    return tabela;
}

int existeChavePrimaria(Tabela *tabela, int num){
    FILE *arquivo;
    char nomeArquivo[NOME_LIMITE + 4];
    sprintf(nomeArquivo, "%s.txt", tabela->nome);

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO %s \n", nomeArquivo);
        return 0;
    }

    char chavePrimaria[NOME_LIMITE];
    int valor;
    // Modificando o formato de leitura para se adaptar ao conteúdo do arquivo
    while (fscanf(arquivo, "%[^:]: ", chavePrimaria) == 1) {
        // Leitura do valor da chave primária
        if (strcmp(chavePrimaria, tabela->colunaChavePrimaria) == 0) {
            fscanf(arquivo, "%d", &valor);
            // Verificar se o valor corresponde à chave primária
            if (valor == num) {
                fclose(arquivo);
                return 1; // Retorna 1 se encontrar a chave primária com valor igual
            }
        } else {
            // Se a chave primária não corresponde, avança para a próxima linha
            fgets(chavePrimaria, NOME_LIMITE, arquivo);
        }
    }

    fclose(arquivo);
    return 0; // Retorna 0 se não encontrar a chave primária com valor igual
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
    while(fgets(linha, sizeof(linha), tabelaPrincipal) != NULL){
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

    int count = 0;
    while (fgets(nomeTabela, sizeof(nomeTabela), tabelaPrincipal) != NULL) {
        if(count == 0){
            printf("=== LISTA DE TABELAS ===\n");
        }
        nomeTabela[strcspn(nomeTabela, "\n")] = '\0';
        printf("%s\n", nomeTabela);
        count++;
    }
    if(count == 0){
        printf("NAO FORAM ENCONTRADAS TABELAS \n");
    }

    fclose(tabelaPrincipal);
}

void criarRegistro(Tabela *tabela){
    getchar();
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
    sprintf(nomeArquivo, "%s.txt", tabela->nome);
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

void listarDadosTabela(Tabela *tabela){
    fflush(stdin);
    printf("INFORME O NOME DA TABELA: ");
    fgets(tabela->nome, NOME_LIMITE, stdin);
    tabela->nome[strcspn(tabela->nome, "\n")] = '\0';
    if(!existeTabela(tabela->nome)){
        printf("A TABELA %s NAO EXISTE!\n", tabela->nome);
        return;
    }

    *tabela = coletarDadosTabela(tabela->nome);

    printf("=== DADOS DA TABELA %s ===\n", tabela->nome);

    // Abrir o arquivo da tabela para leitura dos registros
    FILE *arquivo;
    char nomeArquivo[NOME_LIMITE + 4];
    sprintf(nomeArquivo, "%s.txt", tabela->nome);
    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO %s \n", nomeArquivo);
        return;
    }

    int linhasParaPular = 3 + (2 * tabela->numColunas);
    char linha[NOME_LIMITE * 2]; // buffer para armazenar a linha lida
    int count = 0;
    // Pular as linhas iniciais
    while(count < linhasParaPular){
        if(fgets(linha, sizeof(linha), arquivo) == NULL){
            printf("ERRO: Não há registros nesta tabela.\n");
            fclose(arquivo);
            return;
        }
        count++;
    }
    // Exibir os registros
    count = 0;
    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        // Remove a quebra de linha dos registros
        linha[strcspn(linha, "\n")] = '\0';
        if(count % (tabela->numColunas + 1) == 0 && count != 0){
            printf("|\nLinha %d: | %s ", (count/(tabela->numColunas + 1))+1, linha); // Mostra a linha com quebra de linha e sem espaço
        } else if(count == 0){
            printf("Linha %d: | %s ", (count/(tabela->numColunas + 1))+1, linha); // Mostra a linha sem quebra de linha e sem espaço
        } else{
            printf("| %s ", linha); // Mostra a linha sem quebra de linha e com espaço
        }
        count++;
    }
    if(count != 0){
        printf("|\n");
    }
    fclose(arquivo);
}

void apagarTabela() {

    char nomeTabela[NOME_LIMITE];
    printf("INFORME O NOME DA TABELA QUE DESEJA APAGAR: ");
    getchar();
    fgets(nomeTabela, NOME_LIMITE, stdin);
    nomeTabela[strcspn(nomeTabela, "\n")] = '\0';
    if (existeTabela(nomeTabela) != 1) {
        printf("A TABELA %s NAO EXISTE\n", nomeTabela);
        return;
    }

    FILE *tabelas;
    FILE *tempFile;
    char tabelastxt[] = "tabelas.txt";
    char nomeTabelaTxt[NOME_LIMITE+4];
    sprintf(nomeTabelaTxt, "%s.txt", nomeTabela);
    char linha[NOME_LIMITE];

    tabelas = fopen(tabelastxt, "r");
    if (tabelas == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO %s\n", tabelastxt);
        return;
    }
    tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("ERRO AO CRIAR O ARQUIVO TEMPORÁRIO\n");
        fclose(tabelas);
        return;
    }

    // Procura e remove a tabela do arquivo original
    while (fgets(linha, sizeof(linha), tabelas) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';
        if (strcmp(nomeTabela, linha) != 0) {
            fprintf(tempFile, "%s\n", linha);
        }
    }
    fclose(tabelas);
    fclose(tempFile);

    remove(nomeTabelaTxt);
    remove(tabelastxt);
    rename("temp.txt", tabelastxt);
    printf("TABELA '%s' REMOVIDA COM SUCESSO\n", nomeTabela);
}

void procurarValor(Tabela *tabela){
    // Verifica se existe a tabela desejada
    fflush(stdin);
    printf("INFORME O NOME DA TABELA: ");
    fgets(tabela->nome, NOME_LIMITE, stdin);
    tabela->nome[strcspn(tabela->nome, "\n")] = '\0';
    if(!existeTabela(tabela->nome)){
        printf("A TABELA %s NAO EXISTE!\n", tabela->nome);
        return;
    }
    *tabela = coletarDadosTabela(tabela->nome);

    // Imprime o nome das colunas existentes
    printf("> COLUNAS EXISTENTES:\n");
    for(int i = 0; i <= tabela->numColunas; i++){
        if(i == 0){ // Coluna primária
            printf("> %d - %s\n", i, tabela->colunaChavePrimaria);
        } else { // Outras colunas
            printf("> %d - %s\n", i, tabela->colunas[i-1].nome);
        }
    }

    // Solicita o nome da coluna e verifica se existe
    int numeroColuna;
    while(1){
        printf("INFORME A COLUNA QUE DESEJA BUSCAR O VALOR: ");
        scanf("%d", &numeroColuna);
        if(numeroColuna > tabela->numColunas || numeroColuna < 0){
            printf("COLUNA %d NAO ENCONTRADA!\n", numeroColuna);
        } else {
            if(numeroColuna == 0){
                printf("--> A COLUNA %s FOI SELECIONADA\n", tabela->colunaChavePrimaria);
            } else {
                printf("--> A COLUNA %s FOI SELECIONADA\n", tabela->colunas[numeroColuna-1].nome);
            }
            break;
        }
    }

    // Verifica o tipo da coluna e solicita o valor a ser buscado de acordo com o tipo
    int tipoDaColuna;
    if(numeroColuna == 0){
        tipoDaColuna = 2;
    } else {
        tipoDaColuna = tabela->colunas[numeroColuna-1].tipo;
    }
    char valorChar;
    int valorInt;
    float valorFloat;
    double valorDouble;
    char valorString[NOME_LIMITE];
    fflush(stdin);
    printf("INFORME O VALOR A SER BUSCADO: ");
    switch(tipoDaColuna){
        case 1: // Caso char
            scanf("%c", &valorChar);
            break;
        case 2: // Caso int
            scanf("%d", &valorInt);
            break;
        case 3: // Caso float
            scanf("%f", &valorFloat);
            break;
        case 4: // Caso double
            scanf("%lf", &valorDouble);
            break;
        case 5: // Caso string
            fgets(valorString, NOME_LIMITE, stdin);
            valorString[strcspn(valorString, "\n")] = '\0';
            break;
    }

    // Abrir o arquivo da tabela para leitura dos registros (SIMILAR A FUNÇÃO LISTAR DADOS DA TABELA)
    FILE *arquivo;
    char nomeArquivo[NOME_LIMITE + 4];
    sprintf(nomeArquivo, "%s.txt", tabela->nome);
    arquivo = fopen(nomeArquivo, "r");
    if(arquivo == NULL){
        printf("ERRO AO ABRIR O ARQUIVO DA TABELA %s \n", nomeArquivo);
        return;
    }

    int linhasParaPular = 3 + (2 * tabela->numColunas);
    char linha[NOME_LIMITE * 2]; // buffer para armazenar a linha lida
    int count = 0;
    // Pular as linhas iniciais
    while(count < linhasParaPular){
        if(fgets(linha, sizeof(linha), arquivo) == NULL){
            printf("ERRO: Não há registros nesta tabela.\n");
            fclose(arquivo);
            return;
        }
        count++;
    }

    // Solicita uma opção
    int continuar = 1;
    int escolha;
    int *linhasResultado = NULL;
    int resultadosQtd = 0;
        while(continuar){
            // Mostra somente as opções disponíveis para cada tipo
            switch(tipoDaColuna){
                case 1: // Caso char
                    printf("> 3 - VALORES IGUAIS\n");
                    break;
                case 2: // Caso int
                case 3: // Caso float
                case 4: // Caso double
                    printf("> 1 - VALORES MAIORES\n");
                    printf("> 2 - VALORES IGUAIS ou MAIORES\n");
                    printf("> 3 - VALORES IGUAIS\n");
                    printf("> 4 - VALORES MENORES\n");
                    printf("> 5 - VALORES IGUAIS ou MENORES\n");
                    break;
                case 5: // Caso string
                    printf("> 3 - VALORES IGUAIS\n");
                    printf("> 6 - VALORES PROXIMOS\n");
                    break;
                default:
                    printf("--> TIPO DA COLUNA INVALIDO <--\n");
                    break;
            }
            printf("--> ESCOLHA UM MEIO DE BUSCA: ");
            fflush(stdin);
            scanf("%d", &escolha);

            // Verifica se a escolha existe e se está disponível para o tipo especifico
            switch(escolha){
                case 1: // VALORES MAIORES
                    if(tipoDaColuna == 2){
                        continuar = 0;
                        //Agora começamos a busca pelos valores MAIORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            int valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %d", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile > valorInt){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 3) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores MAIORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            float valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %f", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile > valorFloat){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 4) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores MAIORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            double valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %lf", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile > valorDouble){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else {
                        printf("--> MEIO INVALIDO <--\n");
                    }
                    break;
                case 2: // VALORES IGUAIS ou MAIORES
                    if(tipoDaColuna == 2){
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS ou MAIORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            int valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %d", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile >= valorInt){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 3) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS ou MAIORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            float valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %f", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile >= valorFloat){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 4) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS ou MAIORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            double valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %lf", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile >= valorDouble){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else {
                        printf("--> MEIO INVALIDO <--\n");
                    }
                    break;
                case 3: // VALORES IGUAIS
                    if(tipoDaColuna == 1){
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            char valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %c", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(tolower(valorWhile) == tolower(valorChar)){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 2){
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            int valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %d", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile == valorInt){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 3) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            float valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %f", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile == valorFloat){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 4) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            double valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %lf", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile == valorDouble){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 5) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            char valorWhile[NOME_LIMITE];
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %[^\n]", colunaWhile, valorWhile); // Leitura dos dados da linha
                                if(strcmp(valorWhile, valorString) == 0){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else {
                        printf("--> MEIO INVALIDO <--\n");
                    }
                    break;
                case 4: // VALORES MENORES
                    if(tipoDaColuna == 2){
                        continuar = 0;
                        //Agora começamos a busca pelos valores MENORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            int valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %d", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile < valorInt){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 3) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores MENORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            float valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %f", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile < valorFloat){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 4) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores MENORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            double valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %lf", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile < valorDouble){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else {
                        printf("--> MEIO INVALIDO <--\n");
                    }
                    break;
                case 5: // VALORES IGUAIS ou MENORES
                    if(tipoDaColuna == 2){
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS ou MENORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            int valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %d", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile <= valorInt){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 3) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS ou MENORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            float valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %f", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile <= valorFloat){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else if(tipoDaColuna == 4) {
                        continuar = 0;
                        //Agora começamos a busca pelos valores IGUAIS ou MENORES
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            double valorWhile;
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %lf", colunaWhile, &valorWhile); // Leitura dos dados da linha
                                if(valorWhile <= valorDouble){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else {
                        printf("--> MEIO INVALIDO <--\n");
                    }
                    break;
                case 6: // VALORES PROXIMOS (considero: PROXIMOS = a palavra se encontra na string)
                    if(tipoDaColuna == 5){
                        continuar = 0;
                        //Agora começamos a busca pelos valores PROXIMOS (considero: PROXIMOS = a palavra se encontra na string)
                        count = 0;
                        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
                            char colunaWhile[NOME_LIMITE];
                            char valorWhile[NOME_LIMITE];
                            // Remove a quebra de linha dos registros
                            linha[strcspn(linha, "\n")] = '\0';
                            if ((count) % (tabela->numColunas+1) == numeroColuna) {
                                sscanf(linha, "%[^:]: %[^\n]", colunaWhile, valorWhile); // Leitura dos dados da linha
                                if(existeValorNaString(valorWhile, valorString)){
                                    linhasResultado = realloc(linhasResultado, (resultadosQtd + 1) * sizeof(int));
                                    linhasResultado[resultadosQtd] = count/(tabela->numColunas+1);
                                    resultadosQtd++;
                                }
                            }
                            count++;
                        }
                    } else {
                        printf("--> MEIO INVALIDO <--\n");
                    }
                    break;
                default:
                    printf("--> MEIO INVALIDO <--\n");
                    break;
            }
        }

    // Imprimir linhas encontradas

    rewind(arquivo);
    count = 0;
    // Pular as linhas iniciais
    while(count < linhasParaPular){
        if(fgets(linha, sizeof(linha), arquivo) == NULL){
            printf("ERRO: Não há registros nesta tabela.\n");
            fclose(arquivo);
            return;
        }
        count++;
    }

    // de fato Imprimir linhas encontradas
    count = 0;
    int resultadosImpressos = 0;
    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        // Remove a quebra de linha dos registros
        linha[strcspn(linha, "\n")] = '\0';
        for(int i = 0; i < resultadosQtd; i++){
            if(count/(tabela->numColunas + 1) == linhasResultado[i]){
                if(count % (tabela->numColunas + 1) == 0 && resultadosImpressos != 0){
                    printf("|\nLinha %d: | %s ", (count/(tabela->numColunas + 1))+1, linha); // Mostra a linha com quebra de linha e sem espaço
                } else if(resultadosImpressos == 0){
                    printf("-> LINHAS ENCONTRADAS <-\n");
                    printf("Linha %d: | %s ", (count/(tabela->numColunas + 1))+1, linha); // Mostra a linha sem quebra de linha e sem espaço
                } else{
                    printf("| %s ", linha); // Mostra a linha sem quebra de linha e com espaço
                }
                resultadosImpressos++;
            }
        }
        count++;
    }
    if((resultadosImpressos/(tabela->numColunas + 1)) != 0){
        printf("|\n");
        printf("-> LINHAS ENCONTRADAS <-\n");
    } else {
        printf("-> NENHUM RESULTADO <-\n");
    }
    fclose(arquivo);


    //...
}

int escolhas(int op){
    Tabela tabela;
    switch(op){
        case 0:
            return 0;
        case 1:
            criarTabela(&tabela);
            break;
        case 2:
            listarTabelas();
            break;
        case 3:
            criarRegistro(&tabela);
            break;
        case 4:
            listarDadosTabela(&tabela);
            break;
        case 5:
            procurarValor(&tabela);
            break;
        case 7:
            apagarTabela();
            break;
        default:
            printf("INVALIDO! \n");
            break;
    }
    return 1;
}

int main(){
    int op;
    while(op != 0){
        printf("| 0 - Encerrar | 1 - Nova Tabela | 2 - Listar Tabelas | 3 - Novo registro | 4 - Listar dados de uma tabela | 5 - Procurar valor | 7 - Apagar uma tabela |\n");
        scanf("%d", &op);
        op = escolhas(op);
    }
    return 0;
}

