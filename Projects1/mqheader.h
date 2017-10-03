#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h> // messageQ
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define MSIZE (key_t)0100
#define PERMISSION 0666

struct umsg{
	long mtype;
	char mtext[MSIZE+1];
};


