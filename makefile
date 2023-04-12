CC=g++
FLAGS=-Wall -lGL -lglfw

SOURCE=$(wildcard *.cpp) $(wildcard window/*.cpp) $(wildcard gameObjects/*.cpp)
#OBJ=$(pathsubst %.cpp, %.o, $(SOURCE)) glad.o
OBJ=$(SOURCE:.cpp=.o) glad.o

libdojo.so: $(OBJ)
	$(CC) --shared -o $@ $^ $(FLAGS)

%.o: %.cpp
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

glad.o: glad.c
	$(CC) -fPIC -c -o $@ $< $(FLAGS)

clean:
	rm $(OBJ)
	rm libdojo.so

printSource:
	echo $(SOURCE)
	echo $(OBJ)
