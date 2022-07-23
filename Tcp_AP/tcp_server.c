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

static int SD;
static struct sockaddr_in serverIp;

struct client{
	int AD;
	char buf[BUF_SIZE];
	struct sockaddr_in remoteIp;
	pthread_t pthId;
	pthread_mutex_t mutex;
};
static struct client client[MAX_LISTEN]={0, 0, 0, 0, 0};

static int serverInit(uint16_t port, uint32_t ip);
static void findEmptyClt(int *i);
static int acceptNewClt(int i, socklen_t *serverLen);

void *pthFunc(void *arg){
	int ret, num=*(int*)arg;
	int target=!num;
	printf("%s: pthread[%d] is being started...(%s)\n", __func__, num, inet_ntoa(client[num].remoteIp.sin_addr));
	
	/* Set the AD to nonblock*/
	fcntl(client[num].AD, F_SETFL, fcntl(client[num].AD, F_GETFL, 0) | O_NONBLOCK); 
	
	/* Read/Write */
	while(1){
		sleep(1);
		pthread_mutex_lock(&(client[num].mutex));
		ret=recv(client[num].AD, client[num].buf, BUF_SIZE, 0);	
		if(ret>0 && client[target].AD){
			ret=write(client[target].AD, client[num].buf, strlen(client[num].buf));
		}else if(ret>0){
			printf("%s: No client for write()\n", __func__);
		}else if(!ret){
			break;
		}
		memset(client[num].buf, 0, sizeof(client[num].buf));
		pthread_mutex_unlock(&(client[num].mutex));	
	}
	
	/* Shutdown the client */
	pthread_mutex_destroy(&(client[num].mutex));
	close(client[num].AD);
	client[num].AD=0;
	printf("%s: client[%d] was closed\n", __func__, num);	
	return NULL;
}

int main(int argc, char* argv[]){
	int ret, i=0;
	socklen_t serverLen;
	serverLen=sizeof(struct sockaddr);
	
	/* Initializing the server */
	ret=serverInit(7777, 0);
	if(ret){
		goto err;
	}
	
	/* Creating a pthread for new client */
	while(1){
		
		findEmptyClt(&i);
		/*  Accept a connection request  */
		ret=acceptNewClt(i, &serverLen);
		if(ret){
			goto err;
		}
		sleep(1);
	}
	printf("%s: Server is exiting\n", __func__);
	
	/* Shutdown the server */
	close(SD);
	return 0;

err:
	close(SD);
	return errno;
}

int serverInit(uint16_t port, uint32_t ip){
	int ret;
	if(!ip){
		ip=INADDR_ANY;
	}
	
	/* Initialize the socket */
	SD=socket(AF_INET, SOCK_STREAM, 0);
	if(SD<0){
		printf("%s: socket() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	/* Socket seting  */
	serverIp.sin_family=AF_INET;
	serverIp.sin_port=htons(port);
	serverIp.sin_addr.s_addr=htonl(ip);
	memset(serverIp.sin_zero, 0, 8);
	
	/* Assigns an address to the socket socket */
	ret=bind(SD, (struct sockaddr *)&serverIp, sizeof(struct sockaddr));
	if(ret<0){
		printf("%s: bind() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	
	/* Enables to accept connections */
	ret=listen(SD, MAX_LISTEN);
	if(ret<0){
		printf("%s: listen() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	return 0;
}

void findEmptyClt(int *i){
	int currUsrNum=0, timer=0; 
	while(client[(*i)].AD){
		currUsrNum++;
		(*i)++;
		(*i)%=MAX_LISTEN;
		if(currUsrNum>=MAX_LISTEN){
			printf("%s: The server is full, please wait for a minute...(%d)\n", __func__, timer++);
			sleep(2);
		}
	}
}

int acceptNewClt(int i, socklen_t *serverLen){
	int cltNum=i, ret=0;
	
	/* Accept */
	client[cltNum].AD=accept(SD, (struct sockaddr *)&(client[cltNum].remoteIp), serverLen);
	if(client[cltNum].AD<0){
		printf("%s: accept() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	/* Pthread_mutex_init */
	ret=pthread_mutex_init(&(client[cltNum].mutex), NULL);
	if(ret){
		printf("%s: pthread_mutex_init() was failed\n", __func__);
		close(client[cltNum].AD);
		return errno;
	}
	/* Pthread_create */
	ret=pthread_create(&(client[cltNum].pthId), NULL, pthFunc, (void *)&cltNum);
	if(ret){
		printf("%s: pthread_create() was failed\n", __func__);
		close(client[cltNum].AD);
		return errno;
	}
	
	return 0;
}




/**
	int pthread_create (pthread_t * thread, pthread_attr_t * attr, void * (*start_routine)(void *), void * arg) 
*/


/**
	int socket (int namespace, int style, int protocol)  
	struct sockaddr_in { 
		short      sin_family;    // 2 bytes e.g. AF_INET, AF_INET6 
		unsigned short  sin_port;  // 2 bytes e.g. htons(3490) 
		struct in_addr  sin_addr;   // 4 bytes see struct in_addr, below 
		char       sin_zero[8];   // 8 bytes zero this if you want to 
		};
	struct in_addr {
		uint32_t  s_addr;     // address in network byte order 
	};

	int bind (int socket, struct sockaddr *addr, socklen_t length) 

*/


/**
	sudo nmap -sT -O localhost 
*/
