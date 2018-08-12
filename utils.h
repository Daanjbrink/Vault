#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "server.h"

#define ip(x) inet_ntoa(x->sockaddr.sin_addr)
#define port(x) ntohs(x->sockaddr.sin_port)

#define CONFIRM 0
#define DECLINE 1

void perr(const char *s);
void Sleep(int i);
void Log(const char *format, ...);
void SendC(struct clientData *client, char reason);

#endif
