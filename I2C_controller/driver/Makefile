TARGET = main.out
OBJECTS = main.o 
LINK = gcc
CXX = gcc
FLAGS = -std=c99 -g -Wall -c
INCPATH = -I /usr/include

$(TARGET): $(OBJECTS)
	$(LINK) -o $(TARGET) $(OBJECTS)
main.o: main.c
	$(CXX) $(FLAGS) $(INCPATH) -o main.o main.c
clean:
	rm -f *.o 
	rm *.out
