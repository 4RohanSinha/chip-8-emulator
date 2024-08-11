CC = gcc
CFLAGS = $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)
HEADERS := chip8.h ops.h video.h
C_FILE := main.c chip8.c ops.c video.c
OBJ := main.o chip8.o ops.o video.o

chip8: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.c $(HEADERS)
chip8.o: chip8.c $(HEADERS)
ops.o: ops.c $(HEADERS)
video.o: video.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ video.c

#chip8: chip8.c ops.c video.c main.c
#	gcc $(CFLAGS) chip8.c ops.c video.c main.c -o chip8 $(LDFLAGS)
	#gcc chip8.c ops.c video.c main.c $(sdl2-config --cflags) $(sdl2-config --libs) -o chip8

