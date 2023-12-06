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
                    getchar();
                    scanf("%c", &registroChar);
                    fprintf(arquivo, "%s: %c\n", tabela->colunas[i-1].nome, registroChar);
                    break;
                case 2: // Caso int
                    scanf("%d", &registroInt);
                    fprintf(arquivo, "%s: %d\n", tabela->colunas[i-1].nome, registroInt);
                    break;
                case 3: // Caso float
                    scanf("%f", &registroFloat);
                    fprintf(arquivo, "%s: %f\n", tabela->colunas[i-1].nome, registroFloat);
                    break;
                case 4: // Caso double
                    scanf("%lf", &registroDouble);
                    fprintf(arquivo, "%s: %f\n", tabela->colunas[i-1].nome, registroDouble);
                    break;
                case 5: // Caso string
                    getchar();
                    fgets(registroString, NOME_LIMITE, stdin);
                    registroString[strcspn(registroString, "\n")] = '\0';
                    fprintf(arquivo, "%s: %s\n", tabela->colunas[i-1].nome, registroString);
                    break;
            }
        }
    }

    printf("REGISTRO FEITO!\n");
    fclose(arquivo);
}

void listarDadosTabela(Tabela *tabela){
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
            printf("|\n| %s ", linha); // Mostra a linha com quebra de linha e sem espaço
        } else if(count == 0){
            printf("| %s ", linha); // Mostra a linha sem quebra de linha e sem espaço
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

void apagarTabela(char nomeTabela[]) {
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

int main(){
    int op;
    Tabela tabela;
    while(1){
        printf("| 0 - Encerrar | 1 - Nova Tabela | 2 - Listar Tabelas | 3 - Novo registro | 4 - Listar dados de uma tabela | 7 - Apagar uma tabela |\n");
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
            case 7:
                printf("INFORME O NOME DA TABELA QUE DESEJA APAGAR: ");
                getchar();
                fgets(tabela.nome, NOME_LIMITE, stdin);
                tabela.nome[strcspn(tabela.nome, "\n")] = '\0';
                apagarTabela(tabela.nome);
                break;
            default:
                printf("INVALIDO! \n");
                break;
        }
    }
}

