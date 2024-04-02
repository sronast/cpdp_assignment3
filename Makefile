CFLAGS =-g -std=c++20 -pedantic #this is comment
CC = g++

server: main.o game_server.o game.o classes.o utilities.o
	$(CC) $(CFLAGS) main.o game_server.o game.o classes.o utilities.o -o server
	chmod +x server

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

game_server.o: game_server.cpp utilities.hpp classes.hpp
	$(CC) $(CFLAGS) -c game_server.cpp

game.o: game.cpp 
	$(CC) $(CFLAGS) -c game.cpp
	
classes.o: classes.cpp
	$(CC) $(CFLAGS) -c classes.cpp

utilities.o: utilities.cpp
	$(CC) $(CFLAGS) -c utilities.cpp

clean:
	rm a.out

mymake.o : mymake.cpp
	$(CC) $(CFLAGS) -c mymake.cpp