#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef ERR_H
#define ERR_H

void printerr(char *msg);
int  checkerr(char *msg,int r);
void exit_err(char *msg,int r);

#endif
