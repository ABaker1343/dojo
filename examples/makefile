CC=g++ --std=c++20
CFLAGS=-Wall -lGL -L../lib/ -ldojo -I. -Wl,-rpath=../lib/ $(shell pkg-config --cflags freetype2) -lfreetype

example: example.cpp
	$(CC) example.cpp -g -o example $(CFLAGS)

clean:
	rm example
