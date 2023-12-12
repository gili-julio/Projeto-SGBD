#ifndef AUXILIARES_H
#define AUXILIARES_H
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

Tabela coletarDadosTabela(char nome[]);

int existeChavePrimaria(Tabela *tabela, int num);

int existeTabela(char nome[]);

int existeValorNaString(char valor[], char string[]);


#endif