#include "files.h"

int upload(struct clientData *client, char *path)
{
	char filepath[256] = {0};
	strcpy(filepath, "testdir/"); // Set base directory

	// Filepath can't be larger than 256-2
	strncat(filepath, &path[1], sizeof(filepath)-1);

	Log("Downloading file from %s: %s\n", client->username, filepath);

	// Prevent path traversal
	if(strncmp(filepath, ".", 1) == 0 || strstr(filepath, "..") != NULL || strncmp(filepath, "/", 1) == 0){
		Log("Fishy upload attempt from %s@%s\n", client->username, ip(client));
		return -1;
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
		if(mkdirR(directory) != 0){
			Log("Error creating directory %s\n", directory);
			return -1;
		}
	}

	// Create the file
	FILE *fp;
	if((fp = fopen(filepath, "wb")) == 0){
		Log("Error creating file\n");
		return -1;
	}

	int received = 0;
	char buffer[4096];
	printf("filepath upload: %s\n", filepath);
	while(1){
		// Receive file
		if((received = recv(client->clientfd, buffer, sizeof(buffer), 0)) <= 0){
			Close(client);
			return -1;
		}

		// Signal ending of transmission
		if(received == 1 && buffer[0] == 0xFF){
			break;
		}

		// Write buffer to file
		fwrite(buffer, 1, received, fp);
	}
	fclose(fp);

	return 0;
}

int download(struct clientData *client, char *path)
{
	char filepath[256] = {0};
	strcat(filepath, "testdir/"); // Set base directory

	// Filepath can't be larger than 256-2
	strncat(filepath, &path[1], sizeof(filepath)-1);

	Log("Uploading file to %s: %s\n", client->username, filepath);

	// Prevent path traversal
	if(strncmp(filepath, ".", 1) == 0 || strstr(filepath, "..") != NULL || strncmp(filepath, "/", 1) == 0){
		Log("Fishy download attempt from %s@%s\n", client->username, ip(client));
		return -1;
	}

	// Create the file
	FILE *fp;
	if((fp = fopen(filepath, "rb")) == 0){
		Log("Error reading file\n");
		return -1;
	}

	int readd = 0;
	char buffer[4096];

	while(1){
		// Read file into buffer
		if((readd = fread(buffer, 1, sizeof(buffer), fp)) <= 0)
			break;

		// Send file
		if(send(client->clientfd, buffer, readd, 0) <= 0){
			Close(client);
			return -1;
		}
	}
	send(client->clientfd, (const void *)0xFF, 1, 0); // Signal end of transmission
	fclose(fp);

	return 0;
}
