CC = gcc
CFLAGS =  -Wall -Wextra 

# Lista de archivos fuente
SOURCES = shell.c mkdir.c ls.c rmdir.c touch.c cat.c chmod.c rmfile.c

# Generar una lista de ejecutables basada en los nombres de los archivos fuente
EXECUTABLES = $(SOURCES:.c=)

# Regla para construir todos los ejecutables
all: $(EXECUTABLES)

# Reglas para compilar cada archivo fuente en su propio ejecutable
$(EXECUTABLES): %: %.c
	$(CC) $(CFLAGS) -o $@ $< 

# Regla para limpiar los ejecutables
clean:
	rm -f $(EXECUTABLES)
