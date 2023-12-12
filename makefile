# Lista dos arquivos fonte
SRCS = funcoes/auxiliares.c funcoes/busca.c funcoes/criar.c funcoes/listar.c home.c

# Lista dos arquivos objetos gerados
OBJS = $(SRCS:.c=.o)

# Nome do executável
EXECUTABLE = home

# Comando do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra

# Regra para compilar os objetos a partir dos fontes
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para o executável
$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) -o $(EXECUTABLE)

# Comando para limpar os objetos e o executável
clean:
	del funcoes\*.o
	del home.o
	del home.exe
