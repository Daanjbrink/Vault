#include "server.h"

int main()
{
	// Init the socket
	int sockfd = InitServer();

	// Handle connecting clients and give them their own thread
	AcceptClients();

	// Shouldn't happen
	return 0;
}
