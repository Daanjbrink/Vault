server: main.c server.c server.h utils.c utils.h parser.c parser.h
	gcc -lpthread main.c server.c utils.c parser.c -o server
