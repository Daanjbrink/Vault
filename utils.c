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
	FILE *fp;

	if((fp = fopen("log.txt", "a")) == NULL)
		perr("Error writing to log file");

	va_list list;
	va_start(list, format);
	vfprintf(fp, format, list);
	va_end(list);

	fclose(fp);
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
