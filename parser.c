#include "parser.h"

void parse(struct clientData *client, char *buffer)
{
	// 00 - auth
	// 01 - list dir
	// 02 - upload
	// 03 - download
	printf("Received from socket %d: %s\nID: 0x%02x\n", client->clientfd, buffer, buffer[0]);
	switch(buffer[0]){
		// Authenticate
		case 0x00:
			if (client->auth){
				SendC(client, DECLINE);
				break;
			}

			char username[32] = {0};
			char password[32] = {0};

			strncpy(username, &buffer[1], sizeof(username)-1);
			strncpy(password, &buffer[1+strlen(username)+1], sizeof(password)-1);

			Log("Login attempt form %s:%d:\n\tUsername: %s\n\tPassword: %s\n", ip(client), port(client), username, password);

			if(strcmp(username, "admin") == 0 && strcmp(password, "admin2") == 0){
				SendC(client, CONFIRM);
				strcpy(client->username, username);
				client->auth = 1;
				Log("User %s logged in from %s:%d\n", username, ip(client), port(client));
			} else {
				SendC(client, DECLINE);
			}
			break;
		// List files
		case 0x01:
			if (!client->auth){
				SendC(client, DECLINE);
				break;
			}
			SendC(client, CONFIRM);

			if(listdir(client) == 0)
				SendC(client, CONFIRM); // Function endend succesfully
			else
				SendC(client, DECLINE);
			break;
		// Upload file to client
		case 0x02:
			if (!client->auth){
				SendC(client, DECLINE);
				break;
			}
			SendC(client, CONFIRM);

			if(upload(client, buffer) == 0)
				SendC(client, CONFIRM); // Function endend succesfully
			else
				SendC(client, DECLINE);
			break;
		// Download file from client
		case 0x03:
			if (!client->auth){
				SendC(client, DECLINE);
				break;
			}
			SendC(client, CONFIRM);

			if(download(client, buffer) == 0)
				SendC(client, CONFIRM); // Function endend succesfully
			else
				SendC(client, DECLINE);
			break;
	}
}
