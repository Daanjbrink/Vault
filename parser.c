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

			char username[32];
			char password[32];

			strncpy(username, &buffer[1], sizeof(username)-1);
			strncpy(password, &buffer[1+strlen(username)+1], sizeof(password)-1);

			Log("Login attempt form %s:%d:\n\tUsername: %s\n\tPassword: %s\n", ip(client), port(client), username, password);

			if(strcmp(username, "admin") == 0 && strcmp(password, "admin2") == 0){
				SendC(client, CONFIRM);
				strcpy(client->username, username);
				strcpy(client->password, password);
				client->auth = 1;
				Log("User %s logged in from %s:%d\n", username, ip(client), port(client));
			} else {
				SendC(client, DECLINE);
			}
			break;
		case 0x01:
			if (!client->auth){
				SendC(client, DECLINE);
				break;
			}
			break;
		case 0x02:
			if (!client->auth){
				SendC(client, DECLINE);
				break;
			}

			char filepath[256];

			strncpy(filepath, &buffer[1], sizeof(filepath)-1);

			if(strncmp(filepath, ".", 1) == 0 || strncmp(filepath, "..", 2) == 0 || strncmp(filepath, "/", 1) == 0){
				Log("Fishy upload attempt from %s@%s\n", client->username, ip(client));
				SendC(client, DECLINE);
				break;
			}

			char *dir = strrchr(filepath, '/');
			if(dir != NULL){
				int n = dir - filepath;
				int m = strlen(dir)-1;
				char filename[m+1];
				char directory[n+1];

				strncpy(filename, filepath+n+1, sizeof(filename)-1); // Exclude the /
				strncpy(directory, filepath, sizeof(directory)-1);

				mkdirR(directory);
			}

			FILE *fp;
			if((fp = fopen(filepath, "w")) == 0){
				Log("Error opening upload file\n");
				break;
			}

			int received = 0;
			char buffer[4096];

			while(1){
				// Reset buffer
				memset(buffer, '\0', sizeof(buffer));

				// Receive file
				if((received = recv(client->clientfd, buffer, sizeof(buffer), 0)) <= 0){
					Close(client);
					break;
				}

				if(received == 1)
					if(buffer[0] = 0xFF)
						break;

				// Write buffer to file
				fwrite(buffer, received, 1, fp);
			}

			fclose(fp);
			break;
	}
}
