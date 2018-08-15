#include "listdir.h"
#include "server.h"


// Defines the base directory of the vault in this case "./testdir/"
char basedir[]= "./testdir/";
struct clientData *user;

int callback(const char *filepath, const struct stat *info, const int typeflag, struct FTW *pathinfo){

	send(user->clientfd, filepath, strlen(filepath),0);
	printf("%s\n",filepath+strlen(basedir));
	return 0;
}

char listdir(struct clientData *client){
	
	user = client;
	return nftw(basedir, callback, 15, FTW_PHYS);
	
}
