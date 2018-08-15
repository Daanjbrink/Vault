#include "server.h"

int InitServer()
{
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		perr("Error creating socket\n");

	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);
	sa.sin_addr.s_addr = INADDR_ANY;

	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		perr("Error setting socket options\n");

	if(bind(sockfd, (struct sockaddr *) &sa, sizeof(sa)) == -1)
		perr("Error binding socket\n");

	if(listen(sockfd, 10) == -1)
		perr("Error listening on socket\n");
}

void AcceptClients()
{
	while(1){
		// Don't use too much cpu
		Sleep(100);

		// Store client endpoint
		struct sockaddr_in clientaddr;
		int clientlen = sizeof(clientaddr);

		// Wait till one connect
		int clientfd = accept(sockfd, (struct sockaddr *) &clientaddr, &clientlen);

		// Put all data together
		struct clientData *client = malloc(sizeof(struct clientData));
		client->clientfd = clientfd;
		client->sockaddr = clientaddr;
		client->auth = 0;

		// Give it it's own thread
		pthread_create(&client->thread, NULL, HandleConnection, (void *)client);
		pthread_detach(client->thread);
	}
}

void *HandleConnection(void *clientStruct)
{
	// Cast from void * to struct *
	struct clientData *client = (struct clientData *) clientStruct;

	while(1){
		// Don't use too much cpu
		Sleep(100);

		char buffer[256] = {0};

		// If the client shutdown or closes his connection
		if(recv(client->clientfd, buffer, sizeof(buffer)-1, 0) <= 0){
			Close(client);
			break;
		} else {
			parse(client, buffer);
		}
	}
	return NULL;
}

void Close(struct clientData *client)
{
	// Shutdown socket
	close(client->clientfd);
	// Free up client struct
	free(client);
}
