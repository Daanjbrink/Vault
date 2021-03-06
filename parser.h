#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "server.h"
#include "utils.h"
#include "listdir.h"
#include "files.h"

void parse(struct clientData *client, char *buffer);

#endif
