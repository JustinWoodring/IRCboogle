#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
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

char svalue[BUFFERSIZE];
int sock, valread;
int search(char * string, char buffer[BUFFERSIZE]);
int openConnection(char * hostname);
