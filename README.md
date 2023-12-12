# Projeto SGBD

O projeto consiste em implementar um Sistema de Gerenciamento de Banco de Dados (SGBD) simplificado baseado no modelo relacional. 

## Índice

- [Sobre](#sobre)
- [Instalação](#instalação)
- [Uso](#uso)

## Sobre

Este projeto visa explorar conceitos abordados na disciplina ITP, não se destinando à criação de um SGBD para produção por sistemas de informação. O SGBD ITP (nome dado ao SGBD simplificado) é capaz de realizar as seguintes operações:

- **Criar uma Tabela**
  - O usuário deve informar o nome da tabela.
  - Os tipos de dados para as colunas podem ser os tipos primitivos em C (char, int, float e double) e strings.
  - Os valores são armazenados em arquivo.
  - Na criação da tabela, solicita-se um nome de coluna para ser a chave primária.
  - A chave primária deve ser obrigatoriamente do tipo inteiro sem sinal.

- **Listar Todas as Tabelas**
  - Mostrar para o usuário as tabelas existentes.

- **Criar uma Nova Tupla (Registro) na Tabela**
  - O usuário deve informar o nome da tabela.
  - O sistema solicita os valores de cada uma das colunas.
  - Verifica a chave primária.
  - Cada tabela deve ter uma e apenas uma chave primária. Se o usuário inserir uma chave que já existe, o sistema emite uma mensagem de erro e não insere o registro.

- **Listar Todos os Dados de uma Tabela**
  - O usuário deve informar qual tabela para listar os dados.
  - Os dados são obtidos a partir do arquivo que armazena o conteúdo da tabela.

- **Pesquisar Valor em uma Tabela**
  - O usuário informa o nome da tabela para realizar a pesquisa.
  - O sistema fornece as colunas disponíveis na tabela, e o usuário seleciona uma delas.
  - O sistema solicita o valor para pesquisa, oferecendo algumas opções:
    - Valores maiores que o valor informado.
    - Valores maiores ou iguais ao valor informado.
    - Valores iguais ao valor informado.
    - Valores menores que o valor informado.
    - Valores menores ou iguais ao valor informado.
    - Valores próximos ao valor informado (se a coluna for do tipo string).

- **Apagar uma Tupla de uma Tabela**
  - O usuário informa o nome da tabela e a chave primária da tupla a ser apagada.

- **Apagar uma Tabela**
  - O usuário fornece o nome da tabela a ser apagada.

## Instalação

 - Instalar o MinGW
 - Ter configurado o comando Make

## Uso

Abra o terminal e siga os passos abaixo:
- make
- ./home

Se quiser limpar os arquivos gerados pelo compilador:
- make clean
