#include "busca.h"

void apagarTabela() {

    char nomeTabela[NOME_LIMITE];
    printf("INFORME O NOME DA TABELA QUE DESEJA APAGAR: ");
    fflush(stdin);
    fgets(nomeTabela, NOME_LIMITE, stdin);
    nomeTabela[strcspn(nomeTabela, "\n")] = '\0';
    if (existeTabela(nomeTabela) != 1) {
        printf("A TABELA %s NAO EXISTE\n", nomeTabela);
        return;
    }

    FILE *tabelas;
    FILE *tempFile;
    char tabelastxt[] = "tabelas/tabelas.txt";
    char nomeTabelaTxt[NOME_LIMITE+4];
    sprintf(nomeTabelaTxt, "tabelas/%s.txt", nomeTabela);
    char linha[NOME_LIMITE];

    tabelas = fopen(tabelastxt, "r");
    if (tabelas == NULL) {
        printf("ERRO AO ABRIR O ARQUIVO %s\n", tabelastxt);
        return;
    }
    tempFile = fopen("tabelas/temp.txt", "w");
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
    rename("tabelas/temp.txt", "tabelas/tabelas.txt");
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
    sprintf(nomeArquivo, "tabelas/%s.txt", tabela->nome);
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
                                if(existeValorNaString(valorString, valorWhile)){
                                    printf("%s\n%s\n", valorString, valorWhile);
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

void apagarRegistro(Tabela *tabela){
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

    //Verifica se existe a chave primária informada
    int valorInformado;
    printf("INFORME A CHAVE PRIMARIA DA LINHA PARA APAGAR: ");
    scanf("%d", &valorInformado);
    if(!existeChavePrimaria(tabela, valorInformado)){
        printf("O VALOR %d NAO FOI ENCONTRADO!\n", valorInformado);
        return;
    }

    char nomeArquivo[NOME_LIMITE*2];
    sprintf(nomeArquivo, "tabelas/%s.txt", tabela->nome);
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    FILE *arquivoTemp = fopen("tabelas/temp.txt", "w");
    if (arquivoTemp == NULL) {
        printf("Erro ao criar o arquivo temporário\n");
        fclose(arquivo);
        return;
    }

    char linha[NOME_LIMITE * 2];
    int linhasRegistro = tabela->numColunas + 1; // quantidade de linhas de um registro na tabela
    int count = 0;

    // Copiar as linhas para o novo arquivo, exceto a linha a ser apagada
    while(fgets(linha, sizeof(linha), arquivo) != NULL){
        char colunaTeste[NOME_LIMITE];
        int valorTeste;
        sscanf(linha, "%[^:]: %d", colunaTeste, &valorTeste); // Leitura dos dados da linha
        if( ( (strcmp(colunaTeste, tabela->colunaChavePrimaria) != 0) || (valorTeste != valorInformado) ) && (count%linhasRegistro == 0)){
            fputs(linha, arquivoTemp);
        } else {
            count++;
        }
    }

    fclose(arquivo);
    fclose(arquivoTemp);

    // Remover o arquivo original e renomear o temporário
    remove(nomeArquivo);
    rename("tabelas/temp.txt", nomeArquivo);

    printf("--> LINHA DELETADA COM SUCESSO <--\n");
}
