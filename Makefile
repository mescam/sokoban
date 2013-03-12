CC=gcc
CFLAGS=
LDFLAGS=-lallegro -L/usr/local/lib -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lm

all:sokoban

sokoban: prepare main.o graphics.o
	$(CC) $(LDFLAGS) $(CFLAGS) obj/main.o obj/graphics.o -o bin/sokoban

main.o:
	$(CC) $(LDFLAGS) $(CFLAGS) -c src/main.c -o obj/main.o 

graphics.o:
	$(CC) $(LDFLAGS) $(CFLAGS) -c src/graphics.c -o obj/graphics.o

prepare:
	mkdir obj/
