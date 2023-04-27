CC=g++ --std=c++20
CFLAGS=-Wall -lGL -L../.. -ldojo -I. -Wl,-rpath=../.. $(shell pkg-config --cflags freetype2) -lfreetype
SOURCE=$(wildcard *.cpp)
OBJ=$(SOURCE:.cpp=.o)

walker: $(SOURCE)
	$(CC) $(CFLAGS) walker.cpp game.cpp main.cpp menu.cpp -o walker

game: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm walker
