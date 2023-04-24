CC=g++ --std=c++20
CFLAGS=-Wall -lGL -L.. -ldojo -I. -Wl,-rpath=.. $(shell pkg-config --cflags freetype2) -lfreetype

examples:
	$(CC) example.cpp -g -o example $(CFLAGS)
