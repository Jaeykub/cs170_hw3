COMPILE = g++
FLAGS = -Wall -Werror -ansi -pedantic

all: main grid.o

main: main.cpp grid.o
	$(COMPILE) $(FLAGS) main.cpp grid.o

grid.o: grid.h grid.cpp
	$(COMPILE) $(FLAGS) -c grid.cpp

run:
	$ ./a.out 5 1

clean:
	rm -rf a.out grid.o
