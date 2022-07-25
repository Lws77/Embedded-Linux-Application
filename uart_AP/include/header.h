#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<sys/stat.h>
#include<termios.h>
#include<stdlib.h>
#include<sys/types.h>

#define BUFSIZE 1024
#define WRITER_WAIT_TIME 1

#define DEV_PATH "/dev/ttyUSB0"
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define IS_OPEN(fd)\
	if(fd<0){\
		printf("failed to open. fd=%d\n", fd);\
		return -1;\
	}else{ printf("fd=%d\n", fd); }

#define IS_WR(ret, opt, fd)\
	if(ret<0){ \
		fprintf(stderr, "%s() was failed. errno=%d\n", opt, errno);\
		close(fd);\
		return -1;\
	}else{ printf("ret=%ld\n", ret); }

extern char buf[BUFSIZE];

extern int setSerial(int fd, int nSpeed, int nBits, char nEvent, int nStop, int vmin);
extern int getFileName(int infd, int* outfd, size_t len);
extern int sendFileName(int outfd, int* infd, int argc, char* argv[]);
extern int getContent(int infd, int outfd);