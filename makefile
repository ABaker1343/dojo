CC=g++ --std=c++20
FLAGS=-Wall -lGL -lglfw

SOURCE=$(wildcard *.cpp) $(wildcard window/*.cpp) $(wildcard gameObjects/*.cpp) $(wildcard gameObjects/objects2D/*.cpp) $(wildcard gameObjects/colliders/*.cpp)
HEADERS=$(wildcard headers/*.hpp) $(wildcard window/headers/*.hpp) $(wildcard gameObjects/headers/*.hpp) $(wildcard gameObjects/headers/objects2D/*.cpp) $(wildcard gameObjects/headers/colliders/*.cpp)
#OBJ=$(pathsubst %.cpp, %.o, $(SOURCE)) glad.o
OBJ=$(SOURCE:.cpp=.o) glad.o

libdojo.so: $(OBJ)
	$(CC) --shared -o $@ $^ $(FLAGS)

window/%.o: window/%.cpp window/headers/%.hpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)
	
gameObjects/%.o: gameObjects/%.cpp gameObjects/headers/%.hpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

gameObjects/objects2D/%.o: gameObjects/objects2D/%.cpp gameObjects/headers/objects2D/%.hpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

gameObjects/colliders/%.o: gameObjects/colliders/%.cpp gameObjects/headers/colliders/%.hpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

glad.o: glad.c
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

%.o: %.cpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

clean:
	rm $(OBJ)
	rm libdojo.so

printSource:
	echo $(SOURCE)
	echo $(OBJ)
