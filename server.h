#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

struct clientData {
	pthread_t thread;
	int clientfd;
	struct sockaddr_in sockaddr;
	char username[32];
	char auth;
};

#include "utils.h"
#include "parser.h"

#define PORT 5555

int sockfd;

int InitServer();
void AcceptClients();
void *HandleConnection(void *clientStruct);
void Close(struct clientData *client);

#endif
