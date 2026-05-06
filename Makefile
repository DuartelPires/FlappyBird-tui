# Compilador e Flags
CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = -lncurses -lpthread

# Pastas
SRCDIR = src
OBJDIR = obj
INCDIR = include

# Ficheiros
# Procura todos os .c dentro de src/
SRCS = $(wildcard $(SRCDIR)/*.c)
# Cria a lista de .o dentro de obj/
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
# Nome do executável final
TARGET = flappybird

# Regra principal
all: $(TARGET)

# Cria o executável ligando os objetos
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compila cada .c num .o dentro da pasta obj/
# Cria a pasta obj/ automaticamente se não existir
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Correr o jogo
run: all
	./$(TARGET)

