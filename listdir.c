#include "listdir.h"
#include "server.h"

// Defines the base directory of the vault
char basedir[]= "./testdir/";
struct clientData *user;

int callback(const char *filepath, const struct stat *info, const int typeflag, struct FTW *pathinfo){
	send(user->clientfd, filepath, strlen(filepath),0);
	printf("%s\n",filepath+strlen(basedir));
	return 0;
}

void listdir(struct clientData *client){
	user = client;
	nftw(basedir, callback, 15, FTW_PHYS);
}
