CC=g++ --std=c++20

CFLAGS=-Wall $(shell pkg-config --cflags freetype2) -lfreetype -ldojo

SRC=$(wildcard *.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC))

dojoPlacement: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(OBJ)
	rm dojoPlacement

printSouce:
	echo $(SRC)
	echo $(OBJ)
