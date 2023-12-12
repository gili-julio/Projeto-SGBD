#include "listar.h"
#include "auxiliares.h"
#include "criar.h"
#include "busca.h"

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
        case 6:
            apagarRegistro(&tabela);
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
