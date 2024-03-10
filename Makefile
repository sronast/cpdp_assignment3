CFLAGS =  -std=c++20 -pedantic #this is comment
CC = g++

main.o:  main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm a.out