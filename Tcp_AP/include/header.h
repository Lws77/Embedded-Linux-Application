#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h>

#define MAX_LISTEN 2
#define BUF_SIZE 256
#define SECOND 1000000

struct client{
	int AD;
	char buf[BUF_SIZE];
	struct sockaddr_in remoteIp;
	pthread_t pthId;
	pthread_mutex_t mutex;
};

extern int SD;
extern struct sockaddr_in serverIp;
extern struct client client[MAX_LISTEN];

extern int clientInit(uint16_t port, uint32_t ip);
extern int serverInit(uint16_t port, uint32_t ip);
extern void findEmptyClt(int *i);
extern int acceptNewClt(int i, socklen_t *serverLen);

extern void *pthFunc(void *arg);
