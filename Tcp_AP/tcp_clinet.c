#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>

#define MAX_LISTEN 100
#define BUF_SIZE 256
#define SECOND 1000000

static char buf[BUF_SIZE];
static int i=0;
static int SD;
static struct sockaddr_in serverIp, remoteIp[MAX_LISTEN];

void *pthFunRD(void *arg){
	printf("%s: pthRead\n", __func__);
	while(1){
		usleep(0.5*SECOND);
		memset(buf, 0, BUF_SIZE);
		/* Read from server */
		read(SD, buf, BUF_SIZE);
		printf("%s: buf=%s\n", __func__, buf);
	}
	return NULL;
}

void *pthFunWR(void *arg){
	int ret;
	printf("%s: pthWrite\n", __func__);
	while(1){	
		scanf("%s", buf);
		/* Write to server */
		ret=write(SD, buf, strlen(buf));
	}	
	return NULL;
}

int main(int argc, char* argv[]){
		
	int ret;
	int *rval;
	socklen_t serverLen, remoteLen;
	pthread_t pthWrite, pthRead;
	

	/* Initialize the socket */ 
	SD=socket(AF_INET, SOCK_STREAM, 0);
	if(SD<0){
		printf("%s: socket() was failed, errno=%d\n", __func__, errno);
		return errno;
	}
	
	/* Socket seting  */
	serverIp.sin_family=AF_INET;
	serverIp.sin_port=htons(argc);
	serverIp.sin_addr.s_addr=inet_addr(argv);
	memset(serverIp.sin_zero, 0, 8);
	
	/* Connect with server */
	ret=connect(SD, (struct sockaddr *)&serverIp, sizeof(struct sockaddr));
	if(ret<0){
		printf("%s: bind() was failed, errno=%d\n", __func__, errno);
		goto err;
	}
	/* Create a new thread for read */	
	ret=pthread_create(&pthRead, NULL, pthFunWR, (void *)0);
	if(ret){
		printf("%s: pthread_create() was failed\n", __func__);
		close(SD);
	}
	/* Create a new thread for write */
	ret=pthread_create(&pthWrite, NULL, pthFunRD, (void *)0);
	if(ret){
		printf("%s: pthread_create() was failed\n", __func__);
		close(SD);
	}
	
	ret=pthread_join(pthRead, (void *)&(rval));
	ret=pthread_join(pthWrite, (void *)&(rval));
	
	
	close(SD);
	return 0;

err:
	close(SD);
	return errno;
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
