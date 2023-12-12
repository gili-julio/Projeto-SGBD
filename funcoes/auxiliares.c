#include "auxiliares.h"

Tabela coletarDadosTabela(char nome[]){
    Tabela tabela;
    FILE *arquivo;
    char nomeArquivo[NOME_LIMITE * 2];
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

int existeValorNaString(char valor[], char string[]){
    if(strlen(valor) > strlen(string)){
        return 0;
    }
    for(unsigned int i = 0; i <= strlen(string) - strlen(valor); i++){
        int esta = 1;
        for(unsigned int j = 0; j < strlen(valor); j++){
            if (tolower(valor[j]) != tolower(string[i+j])) {
                esta = 0;
                break;
            }
        }
        if(esta){
            return 1; // Valor encontrado na string
        }
    }
    return 0; // Valor não encontrado na string
}
