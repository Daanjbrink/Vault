server: main.c server.c server.h utils.c utils.h parser.c parser.h listdir.c listdir.h files.c files.h
	gcc -pthread main.c server.c utils.c parser.c listdir.c files.c -o server
