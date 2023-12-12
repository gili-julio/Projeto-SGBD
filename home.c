#include "funcoes/criar.h"
#include "funcoes/busca.h"
#include "funcoes/listar.h"

int main(){
    int op;
    while(op != 0){
        printf("--->\n");
        printf("> 0 - Encerrar\n");
        printf("> 1 - Criar uma tabela\n");
        printf("> 2 - Listar todas as tabelas\n");
        printf("> 3 - Criar uma linha na tabela\n");
        printf("> 4 - Listar dados de uma tabela\n");
        printf("> 5 - Procurar valor em uma tabela\n");
        printf("> 6 - Apagar uma linha na tabela\n");
        printf("> 7 - Apagar uma tabela\n");
        printf("---> Deseja fazer: ");
        scanf("%d", &op);
        op = escolhas(op);
    }
    return 0;
}

