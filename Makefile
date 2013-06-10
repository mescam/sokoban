CC=gcc
CFLAGS=
LDFLAGS=-lallegro -L/usr/local/lib -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_audio -lallegro_acodec -lm

all:sokoban

sokoban: prepare main.o graphics.o misc.o level.o level_list.o editor.o
	$(CC) $(LDFLAGS) $(CFLAGS) obj/main.o obj/graphics.o obj/misc.o obj/level.o obj/level_list.o obj/editor.o -o bin/sokoban

main.o:
	$(CC) $(LDFLAGS) $(CFLAGS) -c src/main.c -o obj/main.o 

graphics.o:
	$(CC) $(LDFLAGS) $(CFLAGS) -c src/graphics.c -o obj/graphics.o

misc.o:
	$(CC) $(LDFLAGS) $(CFLAGS) -c src/misc.c -o obj/misc.o

level.o:
	$(CC) $(LDFLAGS) $(CFLAGS) -c src/level.c -o obj/level.o

level_list.o:
	$(CC) $(LDFLAGS) $(CFLAGS) -c src/level_list.c -o obj/level_list.o

editor.o:
	$(CC) $(LDFLAGS) $(CFLAGS) -c src/editor.c -o obj/editor.o

prepare:
	mkdir -p obj/
