# Makefile для создания статической библиотеки libtag_mu_ncurses.a

# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -O2 -D_XOPEN_SOURCE_EXTENDED
LDFLAGS = -lncursesw

# Имя библиотеки
LIBNAME = libtag_mu_ncurses.a

# Исходные файлы
SRC = tags_mu_ncurses.c
OBJ = tags_mu_ncurses.o

# Цели
all: $(LIBNAME)

# Компиляция объектного файла
$(OBJ): $(SRC) tags_mu_ncurses.h
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

# Создание статической библиотеки
$(LIBNAME): $(OBJ)
	ar rcs $(LIBNAME) $(OBJ)

# Очистка временных файлов
clean:
	rm -f $(OBJ) $(LIBNAME)

.PHONY: all clean
