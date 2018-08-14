#include "utils.h"

void perr(const char *s)
{
	printf("%s\n", s);
	exit(-1);
}

void Sleep(int i)
{
	usleep(i*1000);
}

void Log(const char *format, ...)
{
	//FILE *fp;

	//if((fp = fopen("log.txt", "a")) == NULL)
		//perr("Error writing to log file");

	va_list list;
	va_start(list, format);
	vprintf(format, list);
	va_end(list);

	//fclose(fp);
}

void SendC(struct clientData *client, char reason)
{
	char msg[1] = {0};
	switch(reason){
		case 0x00:;
			msg[0] = 0x41;
			break;
		case 0x01:;
			msg[0] = 0x42;
			break;
	}
	send(client->clientfd, msg, sizeof(msg), 0);
}

int mkdirR(const char *path)
{
    const size_t len = strlen(path);
    char _path[256];
    char *p;

    errno = 0;

    if (len > sizeof(_path)-1) {
        errno = ENAMETOOLONG;
        return -1;
    }
    strcpy(_path, path);

    for(p = _path + 1; *p; p++){
        if(*p == '/'){
            *p = '\0';
            if(mkdir(_path, S_IRWXU) != 0)
                if (errno != EEXIST)
                    return -1;
            *p = '/';
        }
    }

    if(mkdir(_path, S_IRWXU) != 0)
        if (errno != EEXIST)
            return -1;

    return 0;
}
