C=g++
FLAGS=-Wall -lGL

SOURCE=$(wildcard *.cpp) $(wildcard window/*.cpp) $(wildcard gameObjects/*.cpp)
OBJ=$(pathsubst %.cpp, %.o, $(SOURCE)) glad.o

libdojo.so : $(OBJ)
	$(CC) --shared -o $@ $^ $(FLAGS)

%.o : %.cpp %.hpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

window/%.o : window/%.cpp window/headers/%.hpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

gameObjects%.o : gameObjects/%.cpp gameObjects/headers/%.hpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

glad.o : glad.c
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

clean:
	rm $(OBJ)
	rm libdojo.so

printSource:
	echo $(SOURCE)
	echo $(OBJ)
