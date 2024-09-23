CC = gcc
CFLAGS = -Wall -Wextra

# Lista de archivos fuente
SOURCES = shell.c mkdir.c ls.c rmdir.c touch.c cat.c chmod.c

# Objetos generados a partir de los archivos fuente
OBJECTS = $(SOURCES:.c=.o)

# Nombres de los ejecutables
EXECUTABLES = shell mkdir ls rmdir touch cat chmod

# Regla para compilar los objetos
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para construir todos los ejecutables
all: $(EXECUTABLES)

# Reglas para cada ejecutable
shell: shell.o
	$(CC) $(CFLAGS) $^ -o $@

mkdir: mkdir.o
	$(CC) $(CFLAGS) $^ -o $@

ls: ls.o
	$(CC) $(CFLAGS) $^ -o $@
	
touch: touch.o
	$(CC) $(CFLAGS) $^ -o $@

cat: cat.o
	$(CC) $(CFLAGS) $^ -o $@

rm: rmdir.o
	$(CC) $(CFLAGS) $^ -o $@
	

chmod: chmod.o
	$(CC) $(CFLAGS) $^ -o $@
	
	

# Regla para limpiar los archivos objeto y ejecutables
clean:
	rm -f $(OBJECTS) $(EXECUTABLES)
