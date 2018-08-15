#ifndef LISTDIR_H_INCLUDED
#define LISTDIR_H_INCLUDED

#define _XOPEN_SOURCE 500
#include <string.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>

#include "server.h"
#include "utils.h"


char listdir(struct clientData *client);

#endif
