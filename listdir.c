#include "listdir.h"
#include "server.h"

// Defines the base directory of the vault
char basedir[] = "./testdir/";
struct clientData *user;
const char *sep = "!";
int callback(const char *filepath, const struct stat *info, const int typeflag, struct FTW *pathinfo){
	send(user->clientfd, filepath+strlen(basedir), strlen(filepath)-strlen(basedir), 0);
	send(user->clientfd, sep, 1, 0);
	return 0;
}

int listdir(struct clientData *client){
	user = client;
	return nftw(basedir, callback, 15, FTW_PHYS);
}
