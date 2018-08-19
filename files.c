#include "files.h"

int upload(struct clientData *client, char *path)
{
	char filepath[256] = {0};
	strcpy(filepath, "testdir/"); // Set base directory

	// Filepath can't be larger than 256-2
	strncpy(filepath, &path[1], sizeof(filepath)-1);

	Log("Upload file from %s: %s\n", client->username, filepath);

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
	if((fp = fopen(filepath, "w")) == 0){
		Log("Error opening upload file\n");
		return -1;
	}

	int received = 0;
	char buffer[4096];

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
		fwrite(buffer, received, 1, fp);
	}
	fclose(fp);
	
	return 0;
}