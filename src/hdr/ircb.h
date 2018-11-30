#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <asm/stat.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <netdb.h>
#include "definitions.h"

int search(char * string, char buffer[BUFFERSIZE]);
int openConnection(char * hostname);