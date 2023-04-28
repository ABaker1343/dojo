CC=g++ --std=c++20
FLAGS=-Wall -lGL -lglfw $(shell pkg-config --cflags freetype2)

SOURCE=$(wildcard src/*.cpp) $(wildcard src/window/*.cpp) $(wildcard src/gameObjects/*.cpp) $(wildcard src/gameObjects/objects2D/*.cpp) $(wildcard src/gameObjects/colliders/*.cpp) $(wildcard src/gameObjects/menuItems/*.cpp)

#HEADERS=$(wildcard headers/*.hpp) $(wildcard window/headers/*.hpp) $(wildcard gameObjects/headers/*.hpp) $(wildcard gameObjects/headers/objects2D/*.cpp) $(wildcard gameObjects/headers/colliders/*.hpp) $(wildcard gameObjects/headers/menuItems/*.hpp)

HEADERS=$(pathsubst src/%, include/%, $(SOURCE))
HEADERS=$(pathsubst %.cpp, %.hpp, $(HEADERS))

#OBJ=$(pathsubst %.cpp, %.o, $(SOURCE)) glad.o
OBJ=$(SOURCE:.cpp=.o) src/glad.o

lib/libdojo.so: $(OBJ)
	$(CC) --shared -o $@ $^ $(FLAGS)

src/glad.o: src/glad.c
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

%.o: %.cpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

clean:
	rm $(OBJ)
	rm lib/libdojo.so

printSource:
	echo $(SOURCE)
	echo $(OBJ)

install: lib/libdojo.so
	cp lib/libdojo.so /lib/libdojo.so
	cp -r include /usr/include/dojo

