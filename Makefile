CFLAGS = -Wall -std=c99
CC = gcc
OBJ = main.o ghost.o room.o building.o hunter.o evidence.o
LIBS = -lpthread -g

fp: main.o ghost.o room.o building.o hunter.o evidence.o
	$(CC) $(CFLAGS) -o fp main.o ghost.o room.o building.o hunter.o evidence.o $(LIBS)

main.o: main.c defs.h
	$(CC) $(CFLAGS) -c main.c

ghost.o: ghost.c defs.h
	$(CC) $(CFLAGS) -c ghost.c

room.o: room.c defs.h
	$(CC) $(CFLAGS) -c room.c

building.o: building.c defs.h
	$(CC) $(CFLAGS) -c building.c

hunter.o: hunter.c defs.h
	$(CC) $(CFLAGS) -c hunter.c

evidence.o: evidence.c defs.h
	$(CC) $(CFLAGS) -c evidence.c

clean:
	rm -f $(OBJ) fp
