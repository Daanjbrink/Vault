#ifndef FILES_H_INCLUDED
#define FILES_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "server.h"
#include "utils.h"

int upload(struct clientData *client, char *path);
int download(struct clientData *client, char *path);

#endif