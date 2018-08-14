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

			char filepath[256] = {0};

			// Filepath can't be larger than 256-2
			strncpy(filepath, &buffer[1], sizeof(filepath)-1);

			Log("Upload file: %s: %s\n", client->username, filepath);

			// Prevent path traversal
			if(strncmp(filepath, ".", 1) == 0 || strstr(filepath, "..") != NULL){
				Log("Fishy upload attempt from %s@%s\n", client->username, ip(client));
				SendC(client, DECLINE);
				break;
			}

			// Check if path consists of directories
			char *dir = strrchr(filepath, '/');
			if(dir != NULL){				
				int m = strlen(dir)-1; // Do not count the '/'
				int n = dir - filepath; // Beginning of string till last subdir
				char filename[m+1]; // Null terminator
				char directory[n+1];
				
				memset(filename, 0, sizeof(filename));
				memset(directory, 0, sizeof(directory));

				strncpy(filename, filepath+n+1, sizeof(filename)-1); // filepath + n includes the '/'
				strncpy(directory, filepath, sizeof(directory)-1);

				// Create subdirectories
				mkdirR(directory);
			}

			// Create the file
			FILE *fp;
			if((fp = fopen(filepath, "w")) == 0){
				Log("Error opening upload file\n");
				break;
			}

			// Send confirmation the server is ready for transmission
			SendC(client, CONFIRM);

			int received = 0;
			char buffer[4096];

			while(1){
				// Receive file
				if((received = recv(client->clientfd, buffer, sizeof(buffer), 0)) <= 0){
					Close(client);
					break;
				}

				// Signal ending of transmission
				if(received == 1 && buffer[0] == 0xFF){
					break;
				}

				// Write buffer to file
				fwrite(buffer, received, 1, fp);
			}
			fclose(fp);

			// Notify client the file is received
			SendC(client, CONFIRM);
			break;
	}
}
