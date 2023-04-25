CC=g++ --std=c++20
CFLAGS=-Wall -lGL -L../.. -ldojo -I. -Wl,-rpath=../.. $(shell pkg-config --cflags freetype2) -lfreetype
SOURCE=walker.cpp menu.cpp game.cpp main.cpp
OBJ=main.o game.o walker.o menu.o

walker: $(SOURCE)
	rm walker
	$(CC) $(CFLAGS) walker.cpp game.cpp main.cpp menu.cpp -o walker

game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm walker
